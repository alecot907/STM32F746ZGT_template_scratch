/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Gpio_Cfg.h"
#include "Usart_Cfg.h"
#include "LCDACM1602B_Cfg.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

const Gpio_Drv_Regs_t Gpio_Drv_Regs[GPIO_LIST_TOTAL] = 
{
	{LED_GREEN_PORT, 				LED_GREEN_PIN, 				LED_GREEN_MODE, 			LED_GREEN_OUTTYPE, 				LED_GREEN_SPEEED, 			LED_GREEN_PULLTYPE, 			LED_GREEN_ALTFUNC},
	{LED_BLUE_PORT, 				LED_BLUE_PIN, 				LED_BLUE_MODE, 				LED_BLUE_OUTTYPE, 				LED_BLUE_SPEEED, 				LED_BLUE_PULLTYPE, 				LED_BLUE_ALTFUNC},
	{LED_RED_PORT, 					LED_RED_PIN, 					LED_RED_MODE, 				LED_RED_OUTTYPE, 					LED_RED_SPEEED, 				LED_RED_PULLTYPE, 				LED_RED_ALTFUNC},
	{BUTTONUSR_PORT, 				BUTTONUSR_PIN, 				BUTTONUSR_MODE, 			BUTTONUSR_OUTTYPE, 				BUTTONUSR_SPEEED, 			BUTTONUSR_PULLTYPE, 			BUTTONUSR_ALTFUNC},

	{UART2_TX_PORT, 				UART2_TX_PIN, 				UART2_TX_MODE, 				UART2_TX_OUTTYPE, 				UART2_TX_SPEEED, 				UART2_TX_PULLTYPE, 				UART2_TX_ALTFUNC},
	{UART2_RX_PORT, 				UART2_RX_PIN, 				UART2_RX_MODE, 				UART2_RX_OUTTYPE, 				UART2_RX_SPEEED, 				UART2_RX_PULLTYPE, 				UART2_RX_ALTFUNC},

	{LCDACM1602B_VO_PORT, 	LCDACM1602B_VO_PIN, 	LCDACM1602B_V0_MODE, 	LCDACM1602B_V0_OUTTYPE, 	LCDACM1602B_V0_SPEEED, 	LCDACM1602B_V0_PULLTYPE, 	LCDACM1602B_V0_ALTFUNC},
	{LCDACM1602B_RS_PORT, 	LCDACM1602B_RS_PIN, 	LCDACM1602B_RS_MODE, 	LCDACM1602B_RS_OUTTYPE, 	LCDACM1602B_RS_SPEEED, 	LCDACM1602B_RS_PULLTYPE, 	LCDACM1602B_RS_ALTFUNC},
	{LCDACM1602B_E_PORT, 		LCDACM1602B_E_PIN, 		LCDACM1602B_E_MODE, 	LCDACM1602B_E_OUTTYPE, 		LCDACM1602B_E_SPEEED, 	LCDACM1602B_E_PULLTYPE, 	LCDACM1602B_E_ALTFUNC},
	{LCDACM1602B_DB4_PORT, 	LCDACM1602B_DB4_PIN, 	LCDACM1602B_DB4_MODE,	LCDACM1602B_DB4_OUTTYPE, 	LCDACM1602B_DB4_SPEEED, LCDACM1602B_DB4_PULLTYPE, LCDACM1602B_DB4_ALTFUNC},
	{LCDACM1602B_DB5_PORT, 	LCDACM1602B_DB5_PIN, 	LCDACM1602B_DB5_MODE,	LCDACM1602B_DB5_OUTTYPE, 	LCDACM1602B_DB5_SPEEED, LCDACM1602B_DB5_PULLTYPE, LCDACM1602B_DB5_ALTFUNC},
	{LCDACM1602B_DB6_PORT, 	LCDACM1602B_DB6_PIN, 	LCDACM1602B_DB6_MODE,	LCDACM1602B_DB6_OUTTYPE, 	LCDACM1602B_DB6_SPEEED, LCDACM1602B_DB6_PULLTYPE, LCDACM1602B_DB6_ALTFUNC},
	{LCDACM1602B_DB7_PORT, 	LCDACM1602B_DB7_PIN, 	LCDACM1602B_DB7_MODE,	LCDACM1602B_DB7_OUTTYPE, 	LCDACM1602B_DB7_SPEEED, LCDACM1602B_DB7_PULLTYPE, LCDACM1602B_DB7_ALTFUNC},
};

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
