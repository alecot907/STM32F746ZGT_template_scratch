#ifndef __TIMER_CFG_H
#define __TIMER_CFG_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Timer_DrvDefs.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

typedef enum
{
	TIMERBASIC_7,
	
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
