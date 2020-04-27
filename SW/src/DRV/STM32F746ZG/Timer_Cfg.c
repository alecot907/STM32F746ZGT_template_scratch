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

const Timer_Drv_Regs_t Timer_Drv_Regs[TIMER_LIST_TOTAL] = 
{
	{
		TIM7,
		1U,
		TIMER_ENABLE,
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
		
		TIMER_CAPT_COMP_OUT,
		TIMER_OUT_COMP_FROZEN,
		TIMER_DISABLE,
		TIMER_DISABLE,
		
		TIMER_CAPT_COMP_OUTPOL_ACTHIGH,
		TIMER_DISABLE
	},
	
	{
		TIM14,
		4U,
		TIMER_DISABLE,
		1U,
		36000U,
		
		TIMER_ENABLE,
		TIMER_ENABLE,
		TIMER_DISABLE,
		TIMER_UNDER_OVER_FLOW,
		TIMER_DISABLE,
		
		TIMER_MASTER_MODE_RESET,
		
		TIMER_DISABLE,
		TIMER_DISABLE,
		
		TIMER_CAPT_COMP_OUT,
		TIMER_OUT_COMP_PWM1,
		TIMER_ENABLE,
		TIMER_DISABLE,
		
		TIMER_CAPT_COMP_OUTPOL_ACTHIGH,
		TIMER_ENABLE
	},
	
//	{
//		TIM6,
//		4U,
//		TIMER_ENABLE,
//		300,  // 50 Hz
//		4800,
//		
//		TIMER_ENABLE,
//		TIMER_ENABLE,
//		TIMER_DISABLE,
//		TIMER_UNDER_OVER_FLOW,
//		TIMER_DISABLE,
//		
//		TIMER_MASTER_MODE_UPDATE,
//		
//		TIMER_ENABLE,
//		TIMER_DISABLE,
//		
//		TIMER_CAPT_COMP_OUT,
//		TIMER_OUT_COMP_FROZEN,
//		TIMER_DISABLE,
//		TIMER_DISABLE,
//		
//		TIMER_CAPT_COMP_OUTPOL_ACTHIGH,
//		TIMER_DISABLE
//	}
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
	
	Timer_Drv_Start(TIMERBASIC_7_DELAYUS);
	
	uint32_t time_start = sys_ticks_my;
	
	while ((sys_ticks_my - time_start) < delay_10us);
	
	Timer_Drv_Stop(TIMERBASIC_7_DELAYUS);
	
	sys_ticks_my = 0UL;
}


/**************************************************************************************/
/* TIM6_DAC_IRQHandler */
/**************************************************************************************/
void TIM6_DAC_IRQHandler (void)
{
	CLEAR_BIT(TIM6->SR, TIM_SR_UIF);
}
