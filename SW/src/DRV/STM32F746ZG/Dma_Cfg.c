/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Dma_Cfg.h"
#include <string.h>
#include "Adc_Cfg.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


const Dma_Drv_Regs_t Dma_Drv_Regs[DMA_LIST_TOTAL] = 
{
	{
		DMA2_Stream0,
		
		DMA_CHAN_SEL_0,                          //SxCR_ChanSelection;
		DMA_BURST_TRANS_SINGLE,                  //SxCR_MemBurstTransfer;
		DMA_BURST_TRANS_SINGLE,                  //SxCR_PeriphBurstTransfer;
		DMA_CURR_TARGET_MEM0,                    //SxCR_CurrentTarget;
		DMA_DISABLED,                            //SxCR_DoubleBufferMode;
		DMA_PRIORITY_LEVEL_LOW,                  //SxCR_PriorityLevel;
		DMA_PERIPH_INCR_SIZE_PSIZE,              //SxCR_PeriphIncrOffsetSize;
		DMA_DATA_SIZE_HALFWORD,            //SxCR_MemoryDataSize;
		DMA_DATA_SIZE_HALFWORD,            //SxCR_PeriphDataSize;
		DMA_INCR_MODE_INCR,                //SxCR_MemIncrMode;
		DMA_INCR_MODE_FIXED,                     //SxCR_PeriphIncrMode;
		DMA_ENABLED,                       //SxCR_CircularMode;
		DMA_DATA_TRANS_DIR_P2M,                  //SxCR_DataTransDirection;
		DMA_PERIPH_FLOW_CONTR_DMA,               //SxCR_PeriphFlowController;
		DMA_DISABLED,                       //SxCR_TransComplete_Interrupt;
		DMA_DISABLED,                            //SxCR_TransHalf_Interrupt;
		DMA_DISABLED,                            //SxCR_TransError_Interrupt;
		DMA_DISABLED,                            //SxCR_DirectModeError_Interrupt;

		0x0002U,
		&(ADC1->DR),  															// SxPAR_PeriphAddress
		(volatile uint32_t *) ADC1_DMA_Samples,			// SxM0AR_Mem0Address
		NULL,																				// SxM1AR_Mem1Address
		
		DMA_DISABLED,
		DMA_DISABLED,
		DMA_FIFO_TH_FULL
	}
};

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/**************************************************************************************/
/* DMA2_Stream0_IRQHandler */
/**************************************************************************************/
void DMA2_Stream0_IRQHandler (void)
{
	if (READ_BIT(DMA2->LISR, DMA_LISR_TCIF0))
	{
		SET_BIT(DMA2->LIFCR, DMA_LIFCR_CTCIF0);
		READ_REG(ADC1->DR);
	}
}
