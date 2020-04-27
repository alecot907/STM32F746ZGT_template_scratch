/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Adc_Drv.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

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
/* Adc_drv_Init */
/**************************************************************************************/
void Adc_Drv_Init (ADC_LIST_t adc_list)
{
	uint32_t reg_temp;
	ADC_TypeDef *adc_module = Adc_Drv_Regs[adc_list].Adc;
	
	/* Initialize RCC peripheral clock */
	switch ((uint32_t) adc_module)
	{
		case ADC1_BASE:
			SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);
			break;
		case ADC2_BASE:
			SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC2EN);
			break;
		case ADC3_BASE:
			SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC3EN);
			break;
		default:
			break;
	}
	
	/* Set CR1 register */
	reg_temp = (Adc_Drv_Regs[adc_list].CR1_Overrrun_Interrupt << ADC_CR1_OVRIE_Pos) |
							(Adc_Drv_Regs[adc_list].CR1_Resolution << ADC_CR1_RES_Pos) |
							(Adc_Drv_Regs[adc_list].CR1_DiscontChannels << ADC_CR1_DISCNUM_Pos) |
							(Adc_Drv_Regs[adc_list].CR1_DiscontMode << ADC_CR1_DISCEN_Pos) |
							(Adc_Drv_Regs[adc_list].CR1_ScanMode << ADC_CR1_SCAN_Pos) |
							(Adc_Drv_Regs[adc_list].CR1_EndOfConv_Interrupt << ADC_CR1_EOCIE_Pos);
	MODIFY_REG(adc_module->CR1, 0xFFFFFFFF, reg_temp);

	/* Set CR2 register */
	reg_temp = (Adc_Drv_Regs[adc_list].CR2_ExtTrigger << ADC_CR2_EXTEN_Pos) |
							(Adc_Drv_Regs[adc_list].CR2_ExtTriggerSelection << ADC_CR2_EXTSEL_Pos) |
							(Adc_Drv_Regs[adc_list].CR2_DataAlignement << ADC_CR2_ALIGN_Pos) |
							(Adc_Drv_Regs[adc_list].CR2_EndOfConvSelection << ADC_CR2_EOCS_Pos) |
							(Adc_Drv_Regs[adc_list].CR2_DmaDisableSelection << ADC_CR2_DDS_Pos) |
							(Adc_Drv_Regs[adc_list].CR2_Dma << ADC_CR2_DMA_Pos) |
							(Adc_Drv_Regs[adc_list].CR2_ConvMode << ADC_CR2_CONT_Pos);
	MODIFY_REG(adc_module->CR2, 0xFFFFFFFF, reg_temp);
	
	/* Set SMPRx registers */
	reg_temp = (Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR1_SMP18_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR1_SMP17_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR1_SMP16_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR1_SMP15_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR1_SMP14_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR1_SMP13_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR1_SMP12_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR1_SMP11_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR1_SMP10_Pos);
	MODIFY_REG(adc_module->SMPR1, 0xFFFFFFFF, reg_temp);
	
	reg_temp = (Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR2_SMP9_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR2_SMP8_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR2_SMP7_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR2_SMP6_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR2_SMP5_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR2_SMP4_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR2_SMP3_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR2_SMP2_Pos) |
							(Adc_Drv_Regs[adc_list].SMPRx_SamplingTime << ADC_SMPR2_SMP1_Pos);
	MODIFY_REG(adc_module->SMPR2, 0xFFFFFFFF, reg_temp);
	
	/* Set SQRx registers */
	reg_temp = (Adc_Drv_Regs[adc_list].SQR1_RegularChanSeqLength << ADC_SQR1_L_Pos) |
							(Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq16 << ADC_SQR1_SQ16_Pos) |
							(Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq15 << ADC_SQR1_SQ15_Pos) |
							(Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq14 << ADC_SQR1_SQ14_Pos) |
							(Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq13 << ADC_SQR1_SQ13_Pos);
	MODIFY_REG(adc_module->SQR1, 0xFFFFFFFF, reg_temp);
	
	reg_temp = (Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq12 << ADC_SQR2_SQ12_Pos) |
							(Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq11 << ADC_SQR2_SQ11_Pos) |
							(Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq10 << ADC_SQR2_SQ10_Pos) |
							(Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq9 << ADC_SQR2_SQ9_Pos) |
							(Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq8 << ADC_SQR2_SQ8_Pos) |
							(Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq7 << ADC_SQR2_SQ7_Pos);
	MODIFY_REG(adc_module->SQR2, 0xFFFFFFFF, reg_temp);
	
	reg_temp = (Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq6 << ADC_SQR3_SQ6_Pos) |
							(Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq5 << ADC_SQR3_SQ5_Pos) |
							(Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq4 << ADC_SQR3_SQ4_Pos) |
							(Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq3 << ADC_SQR3_SQ3_Pos) |
							(Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq2 << ADC_SQR3_SQ2_Pos) |
							(Adc_Drv_Regs[adc_list].SQRx_ChanNumberSeq1 << ADC_SQR3_SQ1_Pos);
	MODIFY_REG(adc_module->SQR3, 0xFFFFFFFF, reg_temp);
	
	/* Set Common registers */
	reg_temp = (Adc_Drv_RegsCommon.CCR_TempSens << ADC_CCR_TSVREFE_Pos) |
							(Adc_Drv_RegsCommon.CCR_VBatt << ADC_CCR_VBATE_Pos) |
							(Adc_Drv_RegsCommon.CCR_Prescaler << ADC_CCR_ADCPRE_Pos);
	MODIFY_REG(ADC->CCR, 0xFFFFFFFF, reg_temp);
	
	
}


/**************************************************************************************/
/* Adc_Drv_Start */
/**************************************************************************************/
void Adc_Drv_Start (ADC_LIST_t adc_list)
{
	ADC_TypeDef *adc_module = Adc_Drv_Regs[adc_list].Adc;

	if (!(READ_REG(adc_module->CR2) & ADC_CR2_ADON))
	{
		/* Wake up from power down mode */
		SET_BIT(adc_module->CR2, ADC_CR2_ADON);
	}
	
	/* Start regular conversions */
	SET_BIT(adc_module->CR2, ADC_CR2_SWSTART);	
}

/**************************************************************************************/
/* Adc_Drv_Stop */
/**************************************************************************************/
void Adc_Drv_Stop (ADC_LIST_t adc_list)
{
	ADC_TypeDef *adc_module = Adc_Drv_Regs[adc_list].Adc;

	/* Power down mode */
	CLEAR_BIT(adc_module->CR2, ADC_CR2_ADON);
}


/**************************************************************************************/
/* Adc_Drv_Int */
/**************************************************************************************/
void Adc_Drv_Int (ADC_LIST_t adc_list)
{
//	uint32_t		Int_priority;
	
	if ( (ADC_ENABLED == Adc_Drv_Regs[adc_list].CR1_EndOfConv_Interrupt) ||  (ADC_ENABLED == Adc_Drv_Regs[adc_list].CR1_Overrrun_Interrupt))
	{				
//		NVIC_SetPriority (ADC_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
		NVIC_EnableIRQ(ADC_IRQn);
	}
}


/**************************************************************************************/
/* Adc_Drv_Read */
/**************************************************************************************/
/* This function must be used in:
	- single mode
	- Interrupt EOC disabled
	- Scan mode disabled
*/
uint16_t Adc_Drv_Read (ADC_LIST_t adc_list, ADC_CHAN_NUMB_SEQ_t channel)
{
	ADC_TypeDef *adc_module = Adc_Drv_Regs[adc_list].Adc;
	
	/* Select desired channel */
	MODIFY_REG(adc_module->SQR3, 0x1F, channel);
	
	/* Start conversion */
	SET_BIT(adc_module->CR2, ADC_CR2_SWSTART);	
	
	/* Wait for EOC */
	while(!(READ_REG(adc_module->SR) & ADC_SR_EOC));
	
	/* Return converted value */
	return READ_REG(adc_module->DR);
}




