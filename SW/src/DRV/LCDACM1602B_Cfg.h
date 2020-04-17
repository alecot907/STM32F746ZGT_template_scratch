#ifndef __LCDACM1602B_CFG_H
#define __LCDACM1602B_CFG_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "stm32f7xx.h"


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/* GPIO CFG */
#define LCDACM1602B_VO_PORT			(GPIOD)
#define LCDACM1602B_VO_PIN			(0xFU)
#define LCDACM1602B_V0_MODE			(GPIO_MODER_OUTPUT)
#define LCDACM1602B_V0_OUTTYPE	(GPIO_OTYPER_PUSHPULL)
#define LCDACM1602B_V0_SPEEED		(GPIO_OSPEEDR_LOW)
#define LCDACM1602B_V0_PULLTYPE	(GPIO_POPDR_NO)
#define LCDACM1602B_V0_ALTFUNC	(GPIO_AFR_0)

#define LCDACM1602B_RS_PORT			(GPIOD)
#define LCDACM1602B_RS_PIN			(14U)
#define LCDACM1602B_RS_MODE			(GPIO_MODER_OUTPUT)
#define LCDACM1602B_RS_OUTTYPE	(GPIO_OTYPER_PUSHPULL)
#define LCDACM1602B_RS_SPEEED		(GPIO_OSPEEDR_LOW)
#define LCDACM1602B_RS_PULLTYPE	(GPIO_POPDR_NO)
#define LCDACM1602B_RS_ALTFUNC	(GPIO_AFR_0)

#define LCDACM1602B_E_PORT			(GPIOA)
#define LCDACM1602B_E_PIN				(6U)
#define LCDACM1602B_E_MODE			(GPIO_MODER_OUTPUT)
#define LCDACM1602B_E_OUTTYPE		(GPIO_OTYPER_PUSHPULL)
#define LCDACM1602B_E_SPEEED		(GPIO_OSPEEDR_LOW)
#define LCDACM1602B_E_PULLTYPE	(GPIO_POPDR_NO)
#define LCDACM1602B_E_ALTFUNC		(GPIO_AFR_0)

#define LCDACM1602B_DB4_PORT			(GPIOF)
#define LCDACM1602B_DB4_PIN				(12U)
#define LCDACM1602B_DB4_MODE			(GPIO_MODER_OUTPUT)
#define LCDACM1602B_DB4_OUTTYPE		(GPIO_OTYPER_PUSHPULL)
#define LCDACM1602B_DB4_SPEEED		(GPIO_OSPEEDR_LOW)
#define LCDACM1602B_DB4_PULLTYPE	(GPIO_POPDR_NO)
#define LCDACM1602B_DB4_ALTFUNC		(GPIO_AFR_0)

#define LCDACM1602B_DB5_PORT			(GPIOF)
#define LCDACM1602B_DB5_PIN				(13U)
#define LCDACM1602B_DB5_MODE			(GPIO_MODER_OUTPUT)
#define LCDACM1602B_DB5_OUTTYPE		(GPIO_OTYPER_PUSHPULL)
#define LCDACM1602B_DB5_SPEEED		(GPIO_OSPEEDR_LOW)
#define LCDACM1602B_DB5_PULLTYPE	(GPIO_POPDR_NO)
#define LCDACM1602B_DB5_ALTFUNC		(GPIO_AFR_0)

#define LCDACM1602B_DB6_PORT			(GPIOF)
#define LCDACM1602B_DB6_PIN				(14U)
#define LCDACM1602B_DB6_MODE			(GPIO_MODER_OUTPUT)
#define LCDACM1602B_DB6_OUTTYPE		(GPIO_OTYPER_PUSHPULL)
#define LCDACM1602B_DB6_SPEEED		(GPIO_OSPEEDR_LOW)
#define LCDACM1602B_DB6_PULLTYPE	(GPIO_POPDR_NO)
#define LCDACM1602B_DB6_ALTFUNC		(GPIO_AFR_0)

#define LCDACM1602B_DB7_PORT			(GPIOF)
#define LCDACM1602B_DB7_PIN				(15U)
#define LCDACM1602B_DB7_MODE			(GPIO_MODER_OUTPUT)
#define LCDACM1602B_DB7_OUTTYPE		(GPIO_OTYPER_PUSHPULL)
#define LCDACM1602B_DB7_SPEEED		(GPIO_OSPEEDR_LOW)
#define LCDACM1602B_DB7_PULLTYPE	(GPIO_POPDR_NO)
#define LCDACM1602B_DB7_ALTFUNC		(GPIO_AFR_0)



#define LCDACM1602B_MAX_CHARPERLINE	(16)

/* DISPLAY CFG */
#define ENTRYMODE_DISPSHIFT_CFG	(LOW)  //I/D=1: Increment			I/D=0: Decrement
#define ENTRYMODE_CURSDIR_CFG		(HIGH) //S=1: The display is shifted		S=0: The display is not shifted

#define DISPLAY_BRINK_CFG				(LOW)	//B=1: Brink on		B=0: Brink off
#define DISPLAY_CURSOR_CFG			(LOW)	//C=1: Cursor on		C=0: Cursor off
#define DISPLAY_ONOFF_CFG				(HIGH) //D=1: Display on		D=0: Display off

#define CURSOR_SHIFTLR_CFG			(LOW)	//R/L=1: Shift to the Right			R/L=0: Shift to the Left
#define CURSOR_SHIFT_CFG				(LOW)	//S/C=0: Cursor Shift (RAM unchanged)		S/C=1: Display Shift (RAM unchanged)

#define	FUNCTION_FONTS_CFG			(LOW)	//F=0: 5x7 dots		F=1: 5x10 dots
#define	FUNCTION_LINES_CFG			(HIGH) //0: 1 line display    1: 2 lines display

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


#endif /* __LCDACM1602B_CFG_H */
