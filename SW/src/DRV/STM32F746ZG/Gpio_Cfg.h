#ifndef __GPIO_CFG_H
#define __GPIO_CFG_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Gpio_DrvDefs.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

typedef enum
{
	GPIO_LED_GREEN,
	GPIO_LED_BLUE,
	GPIO_LED_RED,
	GPIO_BUTTONUSR,
	
	GPIO_USART2_TX,
	GPIO_USART2_RX,
	
	GPIO_LCDACM1602B_VO,
	GPIO_LCDACM1602B_RS,
	GPIO_LCDACM1602B_E,
	GPIO_LCDACM1602B_DB4,
	GPIO_LCDACM1602B_DB5,
	GPIO_LCDACM1602B_DB6,
	GPIO_LCDACM1602B_DB7,
	
	GPIO_TIM14_PWM,
	GPIO_TIM11_SONICTRIG,
	GPIO_TIM13_SONICECHO,
	
	GPIO_ADC_POT,
	GPIO_LIGHT_POT,
	
	GPIO_NOF
}GPIO_LIST_t;

#define GPIO_LIST_START	(GPIO_LED_GREEN)
#define GPIO_LIST_TOTAL	(GPIO_NOF)


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* LEDs */
#define LED_GREEN_PORT			(GPIOB)
#define LED_GREEN_PIN				(0x0U)
#define LED_GREEN_MODE			(GPIO_MODER_OUTPUT)
#define LED_GREEN_OUTTYPE		(GPIO_OTYPER_PUSHPULL)
#define LED_GREEN_SPEEED		(GPIO_OSPEEDR_LOW)
#define LED_GREEN_PULLTYPE	(GPIO_POPDR_NO)
#define LED_GREEN_ALTFUNC		(GPIO_AFR_0)

#define LED_BLUE_PORT				(GPIOB)
#define LED_BLUE_PIN				(0x7U)
#define LED_BLUE_MODE				(GPIO_MODER_OUTPUT)
#define LED_BLUE_OUTTYPE		(GPIO_OTYPER_PUSHPULL)
#define LED_BLUE_SPEEED			(GPIO_OSPEEDR_LOW)
#define LED_BLUE_PULLTYPE		(GPIO_POPDR_NO)
#define LED_BLUE_ALTFUNC		(GPIO_AFR_0)

#define LED_RED_PORT				(GPIOB)
#define LED_RED_PIN					(0xEU)
#define LED_RED_MODE				(GPIO_MODER_OUTPUT)
#define LED_RED_OUTTYPE			(GPIO_OTYPER_PUSHPULL)
#define LED_RED_SPEEED			(GPIO_OSPEEDR_LOW)
#define LED_RED_PULLTYPE		(GPIO_POPDR_NO)
#define LED_RED_ALTFUNC			(GPIO_AFR_0)

/* Button */
#define BUTTONUSR_PORT				(GPIOC)
#define BUTTONUSR_PIN					(0xDU)
#define BUTTONUSR_MODE				(GPIO_MODER_INPUT)
#define BUTTONUSR_OUTTYPE			(GPIO_OTYPER_PUSHPULL)
#define BUTTONUSR_SPEEED			(GPIO_OSPEEDR_LOW)
#define BUTTONUSR_PULLTYPE		(GPIO_POPDR_NO)
#define BUTTONUSR_ALTFUNC			(GPIO_AFR_0)

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
extern const Gpio_Drv_Regs_t Gpio_Drv_Regs[GPIO_LIST_TOTAL];


#endif /* __GPIO_CFG_H */
