#ifndef __TIMER_CFG_H
#define __TIMER_CFG_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Timer_DrvDefs.h"


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/* Pin configuration */
#define TIM14_PWM_PORT			((GPIO_TypeDef *) (GPIOF))
#define TIM14_PWM_PIN				(0x9U)
#define TIM14_PWM_MODE			((Gpio_Drv_Moder_t) GPIO_MODER_ALTFUNC)
#define TIM14_PWM_OUTTYPE		((Gpio_Drv_Otyper_t) GPIO_OTYPER_PUSHPULL)
#define TIM14_PWM_SPEEED		((Gpio_Drv_Ospeedr_t) GPIO_OSPEEDR_VERYHIGH)
#define TIM14_PWM_PULLTYPE	((Gpio_Drv_Popdr_t) GPIO_POPDR_NO)
#define TIM14_PWM_ALTFUNC		((Gpio_Drv_Afr_t) GPIO_AFR_9)



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

typedef enum
{
	TIMERBASIC_7,
	TIMEGENERAL_14_PWM,
	
	TIMERBASIC_NOF
}TIMERBASIC_LIST_t;

#define TIMERBASIC_LIST_START	(TIMERBASIC_7)
#define TIMERBASIC_LIST_TOTAL	(TIMERBASIC_NOF)

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* TIM7_IRQHandler */
/**************************************************************************************/
void TIM7_IRQHandler (void);

/**************************************************************************************/
/* Delay_us */
/**************************************************************************************/
void Delay_us (uint32_t delay);


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
extern const TimerBasic_Drv_Regs_t TimerBasic_Drv_Regs[TIMERBASIC_LIST_TOTAL];

#endif /* __TIMER_CFG_H */
