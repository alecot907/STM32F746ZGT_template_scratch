/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Dma_Drv.h"

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
/* Dma_Drv_Init */
/**************************************************************************************/
void Dma_Drv_Init (DMA_LIST_t dma_list)
{
	uint32_t reg_temp;
	DMA_Stream_TypeDef *DmaStream = Dma_Drv_Regs[dma_list].DmaStream;
	
	/* Enable peripheral clock */
	if ( ((uint32_t) DmaStream > DMA1_BASE) && ((uint32_t) DmaStream < DMA2_BASE) )
	{
		SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA1EN);
	}
	if ( (uint32_t) DmaStream > DMA2_BASE )
	{
		SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2EN);
	}
	
	/* Set CR register */
	reg_temp = (Dma_Drv_Regs[dma_list].SxCR_ChanSelection << DMA_SxCR_CHSEL_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_MemBurstTransfer << DMA_SxCR_MBURST_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_PeriphBurstTransfer << DMA_SxCR_PBURST_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_CurrentTarget << DMA_SxCR_CT_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_DoubleBufferMode << DMA_SxCR_DBM_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_PriorityLevel << DMA_SxCR_PL_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_PeriphIncrOffsetSize << DMA_SxCR_PINCOS_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_MemoryDataSize << DMA_SxCR_MSIZE_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_PeriphDataSize << DMA_SxCR_PSIZE_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_MemIncrMode << DMA_SxCR_MINC_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_PeriphIncrMode << DMA_SxCR_PINC_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_CircularMode << DMA_SxCR_CIRC_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_DataTransDirection << DMA_SxCR_DIR_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_PeriphFlowController << DMA_SxCR_PFCTRL_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_TransComplete_Interrupt << DMA_SxCR_TCIE_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_TransHalf_Interrupt << DMA_SxCR_HTIE_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_TransError_Interrupt << DMA_SxCR_TEIE_Pos) |
							(Dma_Drv_Regs[dma_list].SxCR_DirectModeError_Interrupt << DMA_SxCR_DMEIE_Pos);
	MODIFY_REG(DmaStream->CR, 0xFFFFFFFF, reg_temp);
	
	/* Set NDTR register */
	reg_temp = (Dma_Drv_Regs[dma_list].SxNDTR_NumberDataTransfer << DMA_SxNDT_Pos);
	MODIFY_REG(DmaStream->NDTR, 0xFFFFFFFF, reg_temp);
	
	/* Set PAR register */
	reg_temp = ((uint32_t)Dma_Drv_Regs[dma_list].SxPAR_PeriphAddress << DMA_SxPAR_PA_Pos);
	MODIFY_REG(DmaStream->PAR, 0xFFFFFFFF, reg_temp);
	
	/* Set M0AR register */
	reg_temp = ((uint32_t)Dma_Drv_Regs[dma_list].SxM0AR_Mem0Address << DMA_SxM0AR_M0A_Pos);
	MODIFY_REG(DmaStream->M0AR, 0xFFFFFFFF, reg_temp);
	
	/* Set M1AR register */
	reg_temp = ((uint32_t)Dma_Drv_Regs[dma_list].SxM1AR_Mem1Address << DMA_SxM1AR_M1A_Pos);
	MODIFY_REG(DmaStream->M1AR, 0xFFFFFFFF, reg_temp);
	
	/* Set M1AR register */
	reg_temp = (Dma_Drv_Regs[dma_list].SxFCR_FifoError_Interrupt << DMA_SxFCR_FEIE_Pos) |
							(Dma_Drv_Regs[dma_list].SxFCR_DirectMode << DMA_SxFCR_DMDIS_Pos) |
							(Dma_Drv_Regs[dma_list].SxFCR_FifoThSel << DMA_SxFCR_FTH_Pos);
	MODIFY_REG(DmaStream->FCR, 0xFFFFFFFF, reg_temp);
}


/**************************************************************************************/
/* Dma_Drv_Start */
/**************************************************************************************/
void Dma_Drv_Start (DMA_LIST_t dma_list)
{
	DMA_Stream_TypeDef *DmaStream = Dma_Drv_Regs[dma_list].DmaStream;
	
	SET_BIT(DmaStream->CR, DMA_SxCR_EN);
}

/**************************************************************************************/
/* Dma_Drv_Int */
/**************************************************************************************/
void Dma_Drv_Int (DMA_LIST_t dma_list)
{
//	uint32_t		Int_priority;
	
	if ( (DMA_ENABLED == Dma_Drv_Regs[dma_list].SxCR_TransComplete_Interrupt) || 
				(DMA_ENABLED == Dma_Drv_Regs[dma_list].SxCR_DirectModeError_Interrupt) ||
				(DMA_ENABLED == Dma_Drv_Regs[dma_list].SxCR_TransError_Interrupt) ||
				(DMA_ENABLED == Dma_Drv_Regs[dma_list].SxCR_TransHalf_Interrupt) ||
				(DMA_ENABLED == Dma_Drv_Regs[dma_list].SxFCR_FifoError_Interrupt) )
	{
		switch ((uint32_t) Dma_Drv_Regs[dma_list].DmaStream)
		{
			case DMA1_Stream0_BASE:
				NVIC_EnableIRQ(DMA1_Stream0_IRQn);
//				NVIC_SetPriority (DMA1_Stream0_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			case DMA1_Stream1_BASE:
				NVIC_EnableIRQ(DMA1_Stream1_IRQn);
//				NVIC_SetPriority (DMA1_Stream1_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			case DMA1_Stream2_BASE:
				NVIC_EnableIRQ(DMA1_Stream2_IRQn);
//				NVIC_SetPriority (DMA1_Stream2_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			case DMA1_Stream3_BASE:
				NVIC_EnableIRQ(DMA1_Stream3_IRQn);
//				NVIC_SetPriority (DMA1_Stream3_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			case DMA1_Stream4_BASE:
				NVIC_EnableIRQ(DMA1_Stream4_IRQn);
//				NVIC_SetPriority (DMA1_Stream4_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			case DMA1_Stream5_BASE:
				NVIC_EnableIRQ(DMA1_Stream5_IRQn);
//				NVIC_SetPriority (DMA1_Stream5_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			case DMA1_Stream6_BASE:
				NVIC_EnableIRQ(DMA1_Stream6_IRQn);
//				NVIC_SetPriority (DMA1_Stream6_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			case DMA1_Stream7_BASE:
				NVIC_EnableIRQ(DMA1_Stream7_IRQn);
//				NVIC_SetPriority (DMA1_Stream7_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			case DMA2_Stream0_BASE:
				NVIC_EnableIRQ(DMA2_Stream0_IRQn);
//				NVIC_SetPriority (DMA2_Stream0_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			case DMA2_Stream1_BASE:
				NVIC_EnableIRQ(DMA2_Stream1_IRQn);
//				NVIC_SetPriority (DMA2_Stream1_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			case DMA2_Stream2_BASE:
				NVIC_EnableIRQ(DMA2_Stream2_IRQn);
//				NVIC_SetPriority (DMA2_Stream2_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			case DMA2_Stream3_BASE:
				NVIC_EnableIRQ(DMA2_Stream3_IRQn);
//				NVIC_SetPriority (DMA2_Stream3_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			case DMA2_Stream4_BASE:
				NVIC_EnableIRQ(DMA2_Stream4_IRQn);
//				NVIC_SetPriority (DMA2_Stream4_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			case DMA2_Stream5_BASE:
				NVIC_EnableIRQ(DMA2_Stream5_IRQn);
//				NVIC_SetPriority (ADC_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			case DMA2_Stream6_BASE:
				NVIC_EnableIRQ(DMA2_Stream6_IRQn);
//				NVIC_SetPriority (DMA2_Stream6_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			case DMA2_Stream7_BASE:
				NVIC_EnableIRQ(DMA2_Stream7_IRQn);
//				NVIC_SetPriority (DMA2_Stream7_IRQn, (Int_priority << __NVIC_PRIO_BITS) - 1UL);
				break;
			default:
				break;
		}
	}
}


