/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include <math.h>
#include "stm32f7xx.h"
#include "Timer_Cfg.h"
#include "Timer_Drv.h"


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
volatile static uint32_t sys_ticks_my = 0ULL;

const TimerBasic_Drv_Regs_t TimerBasic_Drv_Regs[TIMERBASIC_LIST_TOTAL] = 
{
	{
		TIM7,
		1U,
		30,  // 100 kHz
		24,
		
		TIMER_ENABLE,
		TIMER_ENABLE,
		TIMER_DISABLE,
		TIMER_UNDER_OVER_FLOW,
		TIMER_DISABLE,
		
		TIMER_MASTER_MODE_RESET,
		
		TIMER_DISABLE,
		TIMER_ENABLE,
	}	
};

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


#include "Clock_Drv.h"
/**************************************************************************************/
/* TIM7_IRQHandler */
/**************************************************************************************/
void TIM7_IRQHandler (void)
{
	CLEAR_BIT(TIM7->SR, TIM_SR_UIF);

	sys_ticks_my++;
}

/**************************************************************************************/
/* Delay_us */
/**************************************************************************************/
void Delay_us (uint32_t delay)
{
	/* Resolution of 10 us */
	uint32_t delay_10us = (delay < 10UL) ? 1UL : delay/10UL;
	
	Timer_Drv_Start(TIMERBASIC_7);
	
	uint32_t time_start = sys_ticks_my;
	
	while ((sys_ticks_my - time_start) < delay_10us);
	
	Timer_Drv_Stop(TIMERBASIC_7);
	
	sys_ticks_my = 0UL;
}


