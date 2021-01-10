/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include <stdarg.h>
#include <stdio.h>

#include "LCDACM1602B_Drv.h"
#include "LCDACM1602B_Cfg.h"
#include "Gpio_Drv.h"
#include "Clock_Drv.h"
#include "Timer_Drv.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/* Bits order: DB7 ... DB0 */
#define CMD_CLEARDISPLAY		(0x1)
#define CMD_CURSOR_HOME			(0x1 << 1)
#define CMD_ENTRY_MODE			((0x1 << 2) | (ENTRYMODE_CURSDIR_CFG << 1) | (ENTRYMODE_DISPSHIFT_CFG))
#define CMD_DISPLAY_CTRL		((0x1 << 3) | (DISPLAY_ONOFF_CFG << 2) | (DISPLAY_CURSOR_CFG << 1) | (DISPLAY_BRINK_CFG))
#define CMD_CURSOR_SHIFT		((0x1 << 4) | (CURSOR_SHIFT_CFG << 3) | (CURSOR_SHIFTLR_CFG << 2))
#define CMD_FUNCTION_SET		((0x1 << 5) | (FUNCTION_LINES_CFG << 3) | (FUNCTION_FONTS_CFG << 2))
#define CMD_CURSOR_ADDR			(0x1 << 7)

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
static char LCDACM1602B_Drv_TXbuffer[LCDACM1602B_MAX_CHARPERLINE];

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* LCDACM1602B_Drv_SetCursor */
/**************************************************************************************/
static void LCDACM1602B_Drv_SetCursor (uint8_t row, uint8_t col);

/**************************************************************************************/
/* LCDACM1602B_Drv_SendCmd */
/**************************************************************************************/
static void LCDACM1602B_Drv_SendCmd (uint8_t bits);

/**************************************************************************************/
/* LCDACM1602B_Drv_Send4bits */
/**************************************************************************************/
static void LCDACM1602B_Drv_Send4bits (uint8_t bits);

/**************************************************************************************/
/* LCDACM1602B_Drv_EnablePulse */
/**************************************************************************************/
static void LCDACM1602B_Drv_EnablePulse (void);


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* LCDACM1602B_Drv_Init */
/**************************************************************************************/
void LCDACM1602B_Drv_Init (void)
{
	// according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
	Delay_us(50000UL);
	
	Gpio_Drv_SetPin(LCDACM1602B_E_PORT, LCDACM1602B_E_PIN, LOW);
	Gpio_Drv_SetPin(LCDACM1602B_RS_PORT, LCDACM1602B_RS_PIN, LOW);
	
	/* Init procedure for 4-bits mode */
	LCDACM1602B_Drv_Send4bits(0x3U);
	Delay_us(4200UL);
	LCDACM1602B_Drv_Send4bits(0x3U);
	Delay_us(150UL);
	LCDACM1602B_Drv_Send4bits(0x3U);
	LCDACM1602B_Drv_Send4bits(0x2U);
	
	/* Set mode(only 4-bit with this driver), lines, dots */
	LCDACM1602B_Drv_SendCmd(CMD_FUNCTION_SET);
	
	/* Set display state, cursor and brink */
	LCDACM1602B_Drv_SendCmd(CMD_DISPLAY_CTRL);
	Delay_us(2000UL);
	
	/* Clear Display */
	LCDACM1602B_Drv_SendCmd(CMD_CLEARDISPLAY);
	
	/* Set cursor moving direction, display shift */
	LCDACM1602B_Drv_SendCmd(CMD_ENTRY_MODE);
}

/**************************************************************************************/
/* LCDACM1602B_Drv_SendWrite */
/**************************************************************************************/
void LCDACM1602B_Drv_SendString (LCDACM1602B_ROW_t row, uint8_t col, char *str, ...)
{
	char *string_ptr = NULL;
	/* Create a ponter to the not declared parameters of the function */
	va_list arg;
	/* Initialize arg so that it points to the area immediatly after the last parameter (&str += 4) */
	va_start(arg, str);
	/* compose the total string of str + additional arguments inside dbg_tx_buff */
	vsprintf(LCDACM1602B_Drv_TXbuffer, str, arg);
	/* Release arg */
	va_end(arg);
	/* Initialize pointer to string */
	string_ptr = LCDACM1602B_Drv_TXbuffer;
	
	LCDACM1602B_Drv_SetCursor(row, col);
	
	Gpio_Drv_SetPin(LCDACM1602B_RS_PORT, LCDACM1602B_RS_PIN, HIGH);
	
	while ((*string_ptr) != '\0')
	{
		LCDACM1602B_Drv_Send4bits((*string_ptr) >> 4);
		LCDACM1602B_Drv_Send4bits((*string_ptr));
		string_ptr++;
	}
}

/**************************************************************************************/
/* LCDACM1602B_Drv_SetCursor */
/**************************************************************************************/
// row must be 0 or 1 in 2-lines mode
static void LCDACM1602B_Drv_SetCursor (uint8_t row, uint8_t col)
{
	/* When N is 0 (1-line display), AAAAAAA can be 00H to 0FH (27 considering shift).
		When N is 1 (2-line display), AAAAAAA can be 00H to 0FH (27 considering shift) 
		for the first line, and 40H to 4FH (67 considering shift) for the second line.
		When display shift operation is performed, the DDRAM address shifts. See Figure 6. */
	uint8_t temp_bits;
	
	temp_bits = (FUNCTION_LINES_CFG ? ((row << 6) | col) : col);
		
	LCDACM1602B_Drv_SendCmd(CMD_CURSOR_ADDR | temp_bits);
}

/**************************************************************************************/
/* LCDACM1602B_Drv_SendCmd */
/**************************************************************************************/
static void LCDACM1602B_Drv_SendCmd (uint8_t bits)
{
	Gpio_Drv_SetPin(LCDACM1602B_RS_PORT, LCDACM1602B_RS_PIN, LOW);

	LCDACM1602B_Drv_Send4bits((bits >> 4) & 0x0F);
	LCDACM1602B_Drv_Send4bits(bits & 0x0F);
}

/**************************************************************************************/
/* LCDACM1602B_Drv_Send4bits */
/**************************************************************************************/
static void LCDACM1602B_Drv_Send4bits (uint8_t bits)
{
	Gpio_Drv_SetPin(LCDACM1602B_DB7_PORT, LCDACM1602B_DB7_PIN, (LOGIC_STATE_t) ((bits >> 0x03) & 0x01));
	Gpio_Drv_SetPin(LCDACM1602B_DB6_PORT, LCDACM1602B_DB6_PIN, (LOGIC_STATE_t) ((bits >> 0x02) & 0x01));
	Gpio_Drv_SetPin(LCDACM1602B_DB5_PORT, LCDACM1602B_DB5_PIN, (LOGIC_STATE_t) ((bits >> 0x01) & 0x01));
	Gpio_Drv_SetPin(LCDACM1602B_DB4_PORT, LCDACM1602B_DB4_PIN, (LOGIC_STATE_t) (bits & 0x01));
	LCDACM1602B_Drv_EnablePulse();
}

/**************************************************************************************/
/* LCDACM1602B_Drv_EnablePulse */
/**************************************************************************************/
static void LCDACM1602B_Drv_EnablePulse (void)
{
	Gpio_Drv_SetPin(LCDACM1602B_E_PORT, LCDACM1602B_E_PIN, LOW);
	Delay_us(1);
	Gpio_Drv_SetPin(LCDACM1602B_E_PORT, LCDACM1602B_E_PIN, HIGH);
	Delay_us(1);  // enable pulse must be >450ns
	Gpio_Drv_SetPin(LCDACM1602B_E_PORT, LCDACM1602B_E_PIN, LOW);
	Delay_us(100);  // commands need > 37us to settle
}
