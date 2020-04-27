#ifndef __DMA_DRVDEFS_H
#define __DMA_DRVDEFS_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "stm32f7xx.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

typedef enum {
	DMA_DISABLED = 0x0U,
	DMA_ENABLED
} DMA_ENABLE_DISABLE_t;
typedef enum {
	DMA_CHAN_SEL_0 = 0x0U,
	DMA_CHAN_SEL_1,
	DMA_CHAN_SEL_2,
	DMA_CHAN_SEL_3,
	DMA_CHAN_SEL_4,
	DMA_CHAN_SEL_5,
	DMA_CHAN_SEL_6,
	DMA_CHAN_SEL_7
} DMA_CHAN_SEL_t;
typedef enum {
	DMA_BURST_TRANS_SINGLE = 0x0U,
	DMA_BURST_TRANS_INCR4,
	DMA_BURST_TRANS_INCR8,
	DMA_BURST_TRANS_INCR16
} DMA_BURST_TRANS_t;
typedef enum {
	DMA_CURR_TARGET_MEM0 = 0x0U,
	DMA_CURR_TARGET_MEM1
} DMA_CURR_TARGET_t;
typedef enum {
	DMA_PRIORITY_LEVEL_LOW = 0x0U,
	DMA_PRIORITY_LEVEL_MEDIUM,
	DMA_PRIORITY_LEVEL_HIGH,
	DMA_PRIORITY_LEVEL_VERYHIGH
} DMA_PRIORITY_LEVEL_t;
typedef enum {
	DMA_PERIPH_INCR_SIZE_PSIZE = 0x0U,
	DMA_PERIPH_INCR_SIZE_FIXED4
} DMA_PERIPH_INCR_SIZE_t;
typedef enum {
	DMA_DATA_SIZE_BYTE = 0x0U,
	DMA_DATA_SIZE_HALFWORD,
	DMA_DATA_SIZE_WORD,
	DMA_DATA_SIZE_RESERVED
} DMA_DATA_SIZE_t;
typedef enum {
	DMA_INCR_MODE_FIXED = 0x0U,
	DMA_INCR_MODE_INCR
} DMA_INCR_MODE_t;
typedef enum {
	DMA_DATA_TRANS_DIR_P2M = 0x0U,
	DMA_DATA_TRANS_DIR_M2P,
	DMA_DATA_TRANS_DIR_M2M,
	DMA_DATA_TRANS_DIR_RESERVED
} DMA_DATA_TRANS_DIR_t;
typedef enum {
	DMA_PERIPH_FLOW_CONTR_DMA = 0x0U,
	DMA_PERIPH_FLOW_CONTR_PERIPH
} DMA_PERIPH_FLOW_CONTR_t;
typedef enum {
	DMA_FIFO_STATUS_0_1DIV4 = 0x0U,
	DMA_FIFO_STATUS_1DIV4_1DIV2,
	DMA_FIFO_STATUS_1DIV2_3DIV4,
	DMA_FIFO_STATUS_3DIV4_FULL,
	DMA_FIFO_STATUS_EMPTY,
	DMA_FIFO_STATUS_FULL,
	NO_MEANING0,
	NO_MEANING1
} DMA_FIFO_STATUS_t;
typedef enum {
	DMA_FIFO_TH_1DIV4 = 0x0U,
	DMA_FIFO_TH_1DIV2,
	DMA_FIFO_TH_3DIV4,
	DMA_FIFO_TH_FULL
} DMA_FIFO_TH_t;



typedef struct
{
	DMA_Stream_TypeDef				*DmaStream;
	
	DMA_CHAN_SEL_t						SxCR_ChanSelection;
	DMA_BURST_TRANS_t					SxCR_MemBurstTransfer;
	DMA_BURST_TRANS_t					SxCR_PeriphBurstTransfer;
	DMA_CURR_TARGET_t					SxCR_CurrentTarget;
	DMA_ENABLE_DISABLE_t			SxCR_DoubleBufferMode;
	DMA_PRIORITY_LEVEL_t			SxCR_PriorityLevel;
	DMA_PERIPH_INCR_SIZE_t		SxCR_PeriphIncrOffsetSize;
	DMA_DATA_SIZE_t						SxCR_MemoryDataSize;
	DMA_DATA_SIZE_t						SxCR_PeriphDataSize;
	DMA_INCR_MODE_t						SxCR_MemIncrMode;
	DMA_INCR_MODE_t						SxCR_PeriphIncrMode;
	DMA_ENABLE_DISABLE_t			SxCR_CircularMode;
	DMA_DATA_TRANS_DIR_t			SxCR_DataTransDirection;
	DMA_PERIPH_FLOW_CONTR_t		SxCR_PeriphFlowController;
	DMA_ENABLE_DISABLE_t			SxCR_TransComplete_Interrupt;
	DMA_ENABLE_DISABLE_t			SxCR_TransHalf_Interrupt;
	DMA_ENABLE_DISABLE_t			SxCR_TransError_Interrupt;
	DMA_ENABLE_DISABLE_t			SxCR_DirectModeError_Interrupt;
	
	uint16_t									SxNDTR_NumberDataTransfer;
	volatile uint32_t					*SxPAR_PeriphAddress;
	volatile uint32_t					*SxM0AR_Mem0Address;
	volatile uint32_t					*SxM1AR_Mem1Address;
	
	DMA_ENABLE_DISABLE_t			SxFCR_FifoError_Interrupt;
	DMA_ENABLE_DISABLE_t			SxFCR_DirectMode;
	DMA_FIFO_TH_t							SxFCR_FifoThSel;
} Dma_Drv_Regs_t;


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


#endif /* __DMA_DRVDEFS_H */
