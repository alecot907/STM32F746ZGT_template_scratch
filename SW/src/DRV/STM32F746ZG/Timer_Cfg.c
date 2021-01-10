/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include <math.h>
#include "stm32f7xx.h"
#include "Timer_Cfg.h"
#include "Timer_Drv.h"
#include "Adc_Drv.h"
#include "Gpio_Drv.h"

#include "AcquireInput.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
volatile static uint64_t sys_ticks_us_my = 0ULL;

const Timer_Drv_Regs_t Timer_Drv_Regs[TIMER_LIST_TOTAL] = 
{
	{
		TIM7,
		1U,
		72U,  	// 72MHz / 72
		50000U,	// 1us*50000 = 50 ms
		0U,
		
		TIMER_DISABLE,
		TIMER_ENABLE,
		TIMER_DISABLE,
		TIMER_UNDER_OVER_FLOW,
		TIMER_DISABLE,
		
		TIMER_MASTER_MODE_RESET,
		
		TIMER_DISABLE,
		TIMER_DISABLE,
		TIMER_ENABLE,
		
		TIMER_OUT_COMP_FROZEN,
		TIMER_DISABLE,
		TIMER_DISABLE,
		TIMER_CAPT_COMP_OUT,
		TIMER_IN_CAPT_FILTER_F0,
		TIMER_IN_CAPT_PRE_ALL,
		
		TIMER_CAPT_COMP_OUTPOL_OUT_ACTHIGH,
		TIMER_CAPT_COMP_OUTPOL_IN_NONINVRIS,
		TIMER_DISABLE
	},
	{
		TIM14,
		4U,
		1U,
		36000U,
		0U,
		
		TIMER_ENABLE,
		TIMER_ENABLE,
		TIMER_DISABLE,
		TIMER_UNDER_OVER_FLOW,
		TIMER_DISABLE,
		
		TIMER_MASTER_MODE_RESET,
		
		TIMER_DISABLE,
		TIMER_DISABLE,
		TIMER_DISABLE,
		
		TIMER_OUT_COMP_PWM1,
		TIMER_ENABLE,
		TIMER_DISABLE,
		TIMER_CAPT_COMP_OUT,
		TIMER_IN_CAPT_FILTER_F0,
		TIMER_IN_CAPT_PRE_ALL,
		
		TIMER_CAPT_COMP_OUTPOL_OUT_ACTHIGH,
		TIMER_CAPT_COMP_OUTPOL_IN_NONINVRIS,
		TIMER_ENABLE
	},
	{
		TIM6,
		4U,
		150U,  // 72MHz/150 = 480000
		4800U, // 480000*4800 = 0.01  ->100Hz
		0U,
		
		TIMER_ENABLE,
		TIMER_ENABLE,
		TIMER_DISABLE,
		TIMER_UNDER_OVER_FLOW,
		TIMER_DISABLE,
		
		TIMER_MASTER_MODE_UPDATE,
		
		TIMER_ENABLE,
		TIMER_DISABLE,
		TIMER_ENABLE,
		
		TIMER_OUT_COMP_FROZEN,
		TIMER_DISABLE,
		TIMER_DISABLE,
		TIMER_CAPT_COMP_OUT,
		TIMER_IN_CAPT_FILTER_F0,
		TIMER_IN_CAPT_PRE_ALL,
		
		TIMER_CAPT_COMP_OUTPOL_OUT_ACTHIGH,
		TIMER_CAPT_COMP_OUTPOL_IN_NONINVRIS,
		TIMER_DISABLE
	},
	{
		TIM11,
		4U,
		72U,  	// 72MHz/72 = 1MHz
		100U, // 1MHz*100 = 100us
		80U,  // 20us of pin high
		
		TIMER_DISABLE,
		TIMER_ENABLE,
		TIMER_ENABLE,				// One pulse mode
		TIMER_MORE_SOURCES,   // CR1_UpdateReqSource
		TIMER_DISABLE,
		
		TIMER_MASTER_MODE_RESET,
		
		TIMER_DISABLE,
		TIMER_DISABLE,   			// DIER_CaptCompInterrupt
		TIMER_DISABLE,  			// DIER_UpdateInterrupt
		
		TIMER_OUT_COMP_PWM1,
		TIMER_ENABLE,
		TIMER_DISABLE,
		TIMER_CAPT_COMP_OUT,
		TIMER_IN_CAPT_FILTER_F0,
		TIMER_IN_CAPT_PRE_ALL,
		
		TIMER_CAPT_COMP_OUTPOL_OUT_ACTLOW,
		TIMER_CAPT_COMP_OUTPOL_IN_NONINVRIS,
		TIMER_ENABLE
	},
	{
		TIM13,
		4U,
		72U,		// 72MHz/72 = 1MHz  1us resolution
		65000U,  // 65ms
		0U,
		
		TIMER_DISABLE,
		TIMER_ENABLE,
		TIMER_ENABLE,	// One pulse mode
		TIMER_UNDER_OVER_FLOW,
		TIMER_DISABLE,
		
		TIMER_MASTER_MODE_RESET,
		
		TIMER_DISABLE,
		TIMER_ENABLE,   // DIER_CaptCompInterrupt
		TIMER_DISABLE,  // DIER_UpdateInterrupt
		
		TIMER_OUT_COMP_FROZEN,
		TIMER_DISABLE,
		TIMER_DISABLE,
		TIMER_CAPT_COMP_IN_TI1,
		TIMER_IN_CAPT_FILTER_F0,
		TIMER_IN_CAPT_PRE_ALL,

		TIMER_CAPT_COMP_OUTPOL_OUT_ACTHIGH,
		TIMER_CAPT_COMP_OUTPOL_IN_NONINVBOTH,
		TIMER_ENABLE
	}
};

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/



/**************************************************************************************/
/* TIM7_IRQHandler */
/**************************************************************************************/
void TIM7_IRQHandler (void)
{
	CLEAR_BIT(TIM7->SR, TIM_SR_UIF);
	
	/* Sum 50 ms  */
	sys_ticks_us_my += 50000ULL;
}

/**************************************************************************************/
/* GetTime_us */
/**************************************************************************************/
uint64_t GetTime_us (void)
{
	__disable_irq();
	uint32_t ticks = sys_ticks_us_my + (READ_REG(TIM7->CNT) & 0xFFFF);
  __enable_irq();
	
	return ticks;
}

/**************************************************************************************/
/* Delay_us */
/**************************************************************************************/
void Delay_us (uint32_t delay_us)
{	
	sys_ticks_us_my = 0ULL;
	
	CLEAR_BIT(TIM7->CNT, TIM_CNT_CNT);
	
	Timer_Drv_Start(TIMERBASIC_7_DELAY);
	
	while (GetTime_us() < delay_us);
	
	Timer_Drv_Stop(TIMERBASIC_7_DELAY);
}

/**************************************************************************************/
/* TIM6_DAC_IRQHandler */
/**************************************************************************************/
void TIM6_DAC_IRQHandler (void)
{
	CLEAR_BIT(TIM6->SR, TIM_SR_UIF);
	
	/* TODO I want to disable this interupt and trigger a start of
	ocnversion automatically using TRGO of timer. DOESN'T WORK?!?!? */
	Adc_Drv_Start(ADC_LIST_1);
}

/**************************************************************************************/
/* TIM8_UP_TIM13_IRQHandler */
/**************************************************************************************/
void TIM8_UP_TIM13_IRQHandler (void)
{
	CLEAR_BIT(TIM13->SR, TIM_SR_CC1IF | TIM_SR_UIF);

	/* Input capture of sonic sensor echo signal */
	SonicSensor_Obj.rising_falling_measure[SonicSensor_Obj.rising_falling_state++] = READ_REG(TIM13->CCR1);
}


