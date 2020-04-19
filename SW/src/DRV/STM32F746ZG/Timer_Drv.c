/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Timer_Drv.h"
#include "Timer_DrvDefs.h"
#include "Timer_Cfg.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* Timer_Drv_Init */
/**************************************************************************************/
void Timer_Drv_Init (TIMERBASIC_LIST_t timer_list)
{
	/* Clock source is from APB1 */
	uint16_t reg_temp;
	TIM_TypeDef *timer = TimerBasic_Drv_Regs[timer_list].timer;
	
	switch ((uint32_t) timer)
	{
		case (TIM6_BASE):
			SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM6EN);
			break;
		case (TIM7_BASE):
			SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM7EN);
			break;
		case (TIM14_BASE):
			SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM14EN);
			break;
		default:
			break;
	}
	
	reg_temp = (TimerBasic_Drv_Regs[timer_list].CR1_UifRemap << TIM_CR1_UIFREMAP_Pos) |
							(TimerBasic_Drv_Regs[timer_list].CR1_AutoReload << TIM_CR1_ARPE_Pos) |
							(TimerBasic_Drv_Regs[timer_list].CR1_OnePulseMode << TIM_CR1_OPM_Pos) |
							(TimerBasic_Drv_Regs[timer_list].CR1_UpdateReqSource << TIM_CR1_URS_Pos) |
							(TimerBasic_Drv_Regs[timer_list].CR1_UpdateDisable << TIM_CR1_UDIS_Pos);
	MODIFY_REG(timer->CR1, 0xFFFF, reg_temp);
	
	reg_temp = (TimerBasic_Drv_Regs[timer_list].CR2_MasterMode << TIM_CR2_MMS_Pos);
	MODIFY_REG(timer->CR2, 0xFFFF, reg_temp);
	
	reg_temp = (TimerBasic_Drv_Regs[timer_list].DIER_UpdateDmaReq << TIM_DIER_UDE_Pos) |
							(TimerBasic_Drv_Regs[timer_list].DIER_UpdateInterrupt << TIM_DIER_UIE_Pos);
	MODIFY_REG(timer->DIER, 0xFFFF, reg_temp);
	
	reg_temp = (TimerBasic_Drv_Regs[timer_list].CCMR1_CaptComp1Selection << TIM_CCMR1_CC1S_Pos) |
							(TimerBasic_Drv_Regs[timer_list].CCMR1_OutComp1FastEnable << TIM_CCMR1_OC1FE_Pos) |
							(TimerBasic_Drv_Regs[timer_list].CCMR1_OutComp1PreloadEnable << TIM_CCMR1_OC1PE_Pos) |
							((TimerBasic_Drv_Regs[timer_list].CCMR1_OutComp1Mode & 0x7U) << TIM_CCMR1_OC1M_Pos) | (((TimerBasic_Drv_Regs[timer_list].CCMR1_OutComp1Mode >> 0x3U) & 0x1U) << 16U);
	MODIFY_REG(timer->CCMR1, 0xFFFF, reg_temp);
	

	reg_temp = (TimerBasic_Drv_Regs[timer_list].CCER_CaptComp1OutPol << TIM_CCER_CC1P_Pos) |
							(TimerBasic_Drv_Regs[timer_list].CCER_CaptComp1OutEnable << TIM_CCER_CC1E_Pos);
	MODIFY_REG(timer->CCER, 0xFFFF, reg_temp);

	/* set prescaler */
	MODIFY_REG(timer->PSC, 0xFFFF, TimerBasic_Drv_Regs[timer_list].PSC_PreScaler - 0x1U);
	MODIFY_REG(timer->ARR, 0xFFFF, TimerBasic_Drv_Regs[timer_list].ARR_AutoReloadValue - 0x1U);
	
	/* Update generation */
	SET_BIT(timer->EGR, TIM_EGR_UG);
}

/**************************************************************************************/
/* Timer_Drv_Start */
/**************************************************************************************/
void Timer_Drv_Start (TIMERBASIC_LIST_t timer_list)
{
	TIM_TypeDef *timer = TimerBasic_Drv_Regs[timer_list].timer;
	
	SET_BIT(timer->CR1, TIM_CR1_CEN);
}

/**************************************************************************************/
/* Timer_Drv_Stop */
/**************************************************************************************/
void Timer_Drv_Stop (TIMERBASIC_LIST_t timer_list)
{
	TIM_TypeDef *timer = TimerBasic_Drv_Regs[timer_list].timer;
	
	CLEAR_BIT(timer->CR1, TIM_CR1_CEN);
}


/**************************************************************************************/
/* Timer_Drv_Stop */
/**************************************************************************************/
void Timer_Drv_PwmSetDuty (TIMERBASIC_LIST_t timer_list, uint16_t duty)
{
	uint16_t ccr1_reg, duty_sat;
	
	duty_sat = (duty > 1000U) ? 1000U : duty;
	
	ccr1_reg = (duty_sat * TimerBasic_Drv_Regs[timer_list].timer->ARR) / 1000U;
	
	WRITE_REG(TimerBasic_Drv_Regs[timer_list].timer->CCR1, ccr1_reg);
}

/**************************************************************************************/
/* Timer_Drv_Int */
/**************************************************************************************/
void Timer_Drv_Int (TIMERBASIC_LIST_t timer_list)
{
	IRQn_Type 	Int_number;
	uint32_t		Int_priority;
	
	if (TIMER_ENABLE == TimerBasic_Drv_Regs[timer_list].Int_Enable)
	{
		TIM_TypeDef *timer = TimerBasic_Drv_Regs[timer_list].timer;
		
		switch ((uint32_t) timer)
		{
			case TIM6_BASE:
				Int_number = TIM6_DAC_IRQn;
				Int_priority = TimerBasic_Drv_Regs[timer_list].Int_Priority;
				break;
			case TIM7_BASE:
				Int_number = TIM7_IRQn;
				Int_priority = TimerBasic_Drv_Regs[timer_list].Int_Priority;
				break;
			case TIM14_BASE:
				break;
			default:
				break;
		}
		
		NVIC_SetPriority (Int_number, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
		NVIC_EnableIRQ(Int_number);
	}
}

