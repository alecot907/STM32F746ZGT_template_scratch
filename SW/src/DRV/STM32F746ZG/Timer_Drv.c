/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Timer_Drv.h"

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
void Timer_Drv_Init (TIMER_LIST_t timer_list)
{
	/* Clock source is from APB1 */
	uint16_t reg_temp;
	TIM_TypeDef *timer = Timer_Drv_Regs[timer_list].timer;
	
	switch ((uint32_t) timer)
	{
		case TIM1_BASE:
			SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);
			break;
		case TIM2_BASE:
			SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);
			break;
		case TIM3_BASE:
			SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);
			break;
		case TIM4_BASE:
			SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM4EN);
			break;
		case TIM5_BASE:
			SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM5EN);
			break;
		case TIM6_BASE:
			SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM6EN);
			break;
		case TIM7_BASE:
			SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM7EN);
			break;
		case TIM8_BASE:
			SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM8EN);
			break;
		case TIM9_BASE:
			SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM9EN);
			break;
		case TIM10_BASE:
			SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM10EN);
			break;
		case TIM11_BASE:
			SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM11EN);
			break;
		case TIM12_BASE:
			SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM12EN);
			break;
		case TIM13_BASE:
			SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM13EN);
			break;
		case TIM14_BASE:
			SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM14EN);
			break;
		default:
			break;
	}
	
	reg_temp = (Timer_Drv_Regs[timer_list].CR1_UifRemap << TIM_CR1_UIFREMAP_Pos) |
							(Timer_Drv_Regs[timer_list].CR1_AutoReload << TIM_CR1_ARPE_Pos) |
							(Timer_Drv_Regs[timer_list].CR1_OnePulseMode << TIM_CR1_OPM_Pos) |
							(Timer_Drv_Regs[timer_list].CR1_UpdateReqSource << TIM_CR1_URS_Pos) |
							(Timer_Drv_Regs[timer_list].CR1_UpdateDisable << TIM_CR1_UDIS_Pos);
	MODIFY_REG(timer->CR1, 0xFFFF, reg_temp);
	
	reg_temp = (Timer_Drv_Regs[timer_list].CR2_MasterMode << TIM_CR2_MMS_Pos);
	MODIFY_REG(timer->CR2, 0xFFFF, reg_temp);
	
	reg_temp = (Timer_Drv_Regs[timer_list].DIER_UpdateDmaReq << TIM_DIER_UDE_Pos) |
							(Timer_Drv_Regs[timer_list].DIER_CaptCompInterrupt << TIM_DIER_CC1IE_Pos) |
							(Timer_Drv_Regs[timer_list].DIER_UpdateInterrupt << TIM_DIER_UIE_Pos);
	MODIFY_REG(timer->DIER, 0xFFFF, reg_temp);
	
	if (TIMER_CAPT_COMP_OUT == Timer_Drv_Regs[timer_list].CCMR1_CaptComp1Selection)
	{
		reg_temp = ((Timer_Drv_Regs[timer_list].CCMR1_OutComp1Mode & 0x7U) << TIM_CCMR1_OC1M_Pos) | (((Timer_Drv_Regs[timer_list].CCMR1_OutComp1Mode >> 0x3U) & 0x1U) << 16U) |
								(Timer_Drv_Regs[timer_list].CCMR1_OutComp1FastEnable << TIM_CCMR1_OC1FE_Pos) |
								(Timer_Drv_Regs[timer_list].CCMR1_OutComp1PreloadEnable << TIM_CCMR1_OC1PE_Pos) |
								(Timer_Drv_Regs[timer_list].CCMR1_CaptComp1Selection << TIM_CCMR1_CC1S_Pos);
	}
	else
	{
		reg_temp = ((Timer_Drv_Regs[timer_list].CCMR1_InCaptureFilter) << TIM_CCMR1_IC1F_Pos) |
								(Timer_Drv_Regs[timer_list].CCMR1_InCapturePrescaler << TIM_CCMR1_IC1PSC_Pos) |
								(Timer_Drv_Regs[timer_list].CCMR1_CaptComp1Selection << TIM_CCMR1_CC1S_Pos);
	}
	MODIFY_REG(timer->CCMR1, 0xFFFF, reg_temp);
	

	if (TIMER_CAPT_COMP_OUT == Timer_Drv_Regs[timer_list].CCMR1_CaptComp1Selection)
	{
		reg_temp = (0x00U << TIM_CCER_CC1NP_Pos) |
								(Timer_Drv_Regs[timer_list].CCER_CaptComp1OutPolOut << TIM_CCER_CC1P_Pos) |
								(Timer_Drv_Regs[timer_list].CCER_CaptComp1OutEnable << TIM_CCER_CC1E_Pos);
	}
	else
	{
		reg_temp = (0x01U << TIM_CCER_CC1NP_Pos) |
								(Timer_Drv_Regs[timer_list].CCER_CaptComp1OutPolIn << TIM_CCER_CC1P_Pos) |
								(Timer_Drv_Regs[timer_list].CCER_CaptComp1OutEnable << TIM_CCER_CC1E_Pos);
	}
	MODIFY_REG(timer->CCER, 0xFFFF, reg_temp);

	/* set prescaler */
	MODIFY_REG(timer->PSC, 0xFFFF, Timer_Drv_Regs[timer_list].PSC_PreScaler - 0x1U);
	MODIFY_REG(timer->ARR, 0xFFFF, Timer_Drv_Regs[timer_list].ARR_AutoReloadValue - 0x1U);
	WRITE_REG(timer->CCR1, Timer_Drv_Regs[timer_list].CCR1_CaptCompValue);
	
	
	/* Update generation */
	SET_BIT(timer->EGR, TIM_EGR_UG);
}

/**************************************************************************************/
/* Timer_Drv_Start */
/**************************************************************************************/
void Timer_Drv_Start (TIMER_LIST_t timer_list)
{
	TIM_TypeDef *timer = Timer_Drv_Regs[timer_list].timer;
	
	SET_BIT(timer->CR1, TIM_CR1_CEN);
}

/**************************************************************************************/
/* Timer_Drv_Stop */
/**************************************************************************************/
void Timer_Drv_Stop (TIMER_LIST_t timer_list)
{
	TIM_TypeDef *timer = Timer_Drv_Regs[timer_list].timer;
	
	CLEAR_BIT(timer->CR1, TIM_CR1_CEN);
}


/**************************************************************************************/
/* Timer_Drv_Stop */
/**************************************************************************************/
void Timer_Drv_PwmSetDuty (TIMER_LIST_t timer_list, uint16_t duty)
{
	uint16_t ccr1_reg, duty_sat;
	
	duty_sat = (duty > 1000U) ? 1000U : duty;
	
	ccr1_reg = (duty_sat * Timer_Drv_Regs[timer_list].timer->ARR) / 1000U;
	
	WRITE_REG(Timer_Drv_Regs[timer_list].timer->CCR1, ccr1_reg);
}

/**************************************************************************************/
/* Timer_Drv_Int */
/**************************************************************************************/
void Timer_Drv_Int (TIMER_LIST_t timer_list)
{
	IRQn_Type 	Int_number;
	uint32_t		Int_priority;
	
	Int_priority = Timer_Drv_Regs[timer_list].Int_Priority;
	
	if ( (TIMER_ENABLE == Timer_Drv_Regs[timer_list].DIER_UpdateInterrupt ) || 
			(TIMER_ENABLE == Timer_Drv_Regs[timer_list].DIER_CaptCompInterrupt) )
	{
		TIM_TypeDef *timer = Timer_Drv_Regs[timer_list].timer;
		
		switch ((uint32_t) timer)
		{
			case TIM6_BASE:
				Int_number = TIM6_DAC_IRQn;
				break;
			case TIM7_BASE:
				Int_number = TIM7_IRQn;
				break;
			case TIM11_BASE:
				Int_number = TIM1_TRG_COM_TIM11_IRQn;
				break;
			case TIM13_BASE:
				Int_number = TIM8_UP_TIM13_IRQn;
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

