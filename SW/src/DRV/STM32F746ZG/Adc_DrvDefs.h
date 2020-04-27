#ifndef __ADC_DRVDEFS_H
#define __ADC_DRVDEFS_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "stm32f7xx.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

typedef enum {
	ADC_DISABLED = 0x0U,
	ADC_ENABLED
} ADC_ENABLE_DISABLE_t;
typedef enum {
	ADC_RESOLUTION12 = 0x0U,
	ADC_RESOLUTION10,
	ADC_RESOLUTION8,
	ADC_RESOLUTION6
} ADC_RESOLUTION_t;
typedef enum {
	ADC_DISCONT_CHANNELS1 = 0x0U,
	ADC_DISCONT_CHANNELS2,
	ADC_DISCONT_CHANNELS3,
	ADC_DISCONT_CHANNELS4,
	ADC_DISCONT_CHANNELS5,
	ADC_DISCONT_CHANNELS6,
	ADC_DISCONT_CHANNELS7,
	ADC_DISCONT_CHANNELS8
} ADC_DISCONT_CHANNELS_t;
typedef enum {
	ADC_EXT_TRIGGER_DISABLED = 0x0U,
	ADC_EXT_TRIGGER_RIS_EDGE,
	ADC_EXT_TRIGGER_FALL_EDGE,
	ADC_EXT_TRIGGER_RISFALL_EDGE
} ADC_EXT_TRIGGER_t;
typedef enum {
	ADC_EXT_TRIGGER_SEL_TIM1CH1 = 0x0U,
	ADC_EXT_TRIGGER_SEL_TIM1CH2,
	ADC_EXT_TRIGGER_SEL_TIM1CH3,
	ADC_EXT_TRIGGER_SEL_TIM2CH2,
	ADC_EXT_TRIGGER_SEL_TIM5TRGO,
	ADC_EXT_TRIGGER_SEL_TIM4CH4,
	ADC_EXT_TRIGGER_SEL_TIM3CH4,
	ADC_EXT_TRIGGER_SEL_TIM8TRGO,
	ADC_EXT_TRIGGER_SEL_TIM8TRGO_2,
	ADC_EXT_TRIGGER_SEL_TIM1TRGO,
	ADC_EXT_TRIGGER_SEL_TIM1TRGO_2,
	ADC_EXT_TRIGGER_SEL_TIM2TRGO,
	ADC_EXT_TRIGGER_SEL_TIM4TRGO,
	ADC_EXT_TRIGGER_SEL_TIM6TRGO,
	ADC_EXT_TRIGGER_SEL_RESERVED,
	ADC_EXT_TRIGGER_SEL_EXTLINE11
} ADC_EXT_TRIGGER_SEL_t;
typedef enum {
	ADC_DATA_ALIGNMENT_RIGHT = 0x0U,
	ADC_DATA_ALIGNMENT_LEFT
} ADC_DATA_ALIGNMENT_t;
typedef enum {
	ADC_EOC_SEL_EOSEQ = 0x0U,
	ADC_EOC_SEL_EOSINGLE
} ADC_EOC_SEL_t;
typedef enum {
	ADC_DMA_DISABLE_SEL_NO = 0x0U,  // No new DMA request is issued after the last transfer (as configured in the DMA controller)
	ADC_DMA_DISABLE_SEL_YES					// DMA requests are issued as long as data are converted and DMA=1
} ADC_DMA_DISABLE_SEL_t;
typedef enum {
	ADC_CONV_MODE_SINGLE = 0x0U,
	ADC_CONV_MODE_CONT
} ADC_CONV_MODE_t;
typedef enum {
	ADC_SAMPL_TIME_CYCLES_3 = 0x0U,
	ADC_SAMPL_TIME_CYCLES_15,
	ADC_SAMPL_TIME_CYCLES_28,
	ADC_SAMPL_TIME_CYCLES_56,
	ADC_SAMPL_TIME_CYCLES_84,
	ADC_SAMPL_TIME_CYCLES_112,
	ADC_SAMPL_TIME_CYCLES_144,
	ADC_SAMPL_TIME_CYCLES_480
} ADC_SAMPL_TIME_SEL_t;
typedef enum {
	ADC_REG_CH_SEQ_LENGTH_1 = 0x0U,
	ADC_REG_CH_SEQ_LENGTH_2,
	ADC_REG_CH_SEQ_LENGTH_3,
	ADC_REG_CH_SEQ_LENGTH_4,
	ADC_REG_CH_SEQ_LENGTH_5,
	ADC_REG_CH_SEQ_LENGTH_6,
	ADC_REG_CH_SEQ_LENGTH_7,
	ADC_REG_CH_SEQ_LENGTH_8,
	ADC_REG_CH_SEQ_LENGTH_9,
	ADC_REG_CH_SEQ_LENGTH_10,
	ADC_REG_CH_SEQ_LENGTH_11,
	ADC_REG_CH_SEQ_LENGTH_12,
	ADC_REG_CH_SEQ_LENGTH_13,
	ADC_REG_CH_SEQ_LENGTH_14,
	ADC_REG_CH_SEQ_LENGTH_15,
	ADC_REG_CH_SEQ_LENGTH_16
} ADC_REG_CH_SEQ_LENGTH_t;
typedef enum {
	ADC_CHAN_NUMB_SEQ_0 = 0x0U,
	ADC_CHAN_NUMB_SEQ_1,
	ADC_CHAN_NUMB_SEQ_2,
	ADC_CHAN_NUMB_SEQ_3,
	ADC_CHAN_NUMB_SEQ_4,
	ADC_CHAN_NUMB_SEQ_5,
	ADC_CHAN_NUMB_SEQ_6,
	ADC_CHAN_NUMB_SEQ_7,
	ADC_CHAN_NUMB_SEQ_8,
	ADC_CHAN_NUMB_SEQ_9,
	ADC_CHAN_NUMB_SEQ_10,
	ADC_CHAN_NUMB_SEQ_11,
	ADC_CHAN_NUMB_SEQ_12,
	ADC_CHAN_NUMB_SEQ_13,
	ADC_CHAN_NUMB_SEQ_14,
	ADC_CHAN_NUMB_SEQ_15,
	ADC_CHAN_NUMB_SEQ_16,
	ADC_CHAN_NUMB_SEQ_17,
	ADC_CHAN_NUMB_SEQ_18
} ADC_CHAN_NUMB_SEQ_t;
typedef enum {
	ADC_PRESCALER_DIV2 = 0x0U,
	ADC_PRESCALER_DIV4,
	ADC_PRESCALER_DIV6,
	ADC_PRESCALER_DIV8
} ADC_PRESCALER_t;


typedef struct
{
	ADC_TypeDef 						*Adc;
	
	ADC_ENABLE_DISABLE_t		CR1_Overrrun_Interrupt;
	ADC_RESOLUTION_t				CR1_Resolution;
	ADC_DISCONT_CHANNELS_t	CR1_DiscontChannels;
	ADC_ENABLE_DISABLE_t		CR1_DiscontMode;
	ADC_ENABLE_DISABLE_t		CR1_ScanMode;
	ADC_ENABLE_DISABLE_t		CR1_EndOfConv_Interrupt;
	
	ADC_EXT_TRIGGER_t				CR2_ExtTrigger;
	ADC_EXT_TRIGGER_SEL_t		CR2_ExtTriggerSelection;
	ADC_DATA_ALIGNMENT_t		CR2_DataAlignement;
	ADC_EOC_SEL_t						CR2_EndOfConvSelection;
	ADC_DMA_DISABLE_SEL_t		CR2_DmaDisableSelection;
	ADC_ENABLE_DISABLE_t		CR2_Dma;
	ADC_CONV_MODE_t					CR2_ConvMode;
	
	ADC_SAMPL_TIME_SEL_t		SMPRx_SamplingTime;  // all channels have the same sampling time (maybe this must be changed)
	
	ADC_REG_CH_SEQ_LENGTH_t SQR1_RegularChanSeqLength;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq16;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq15;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq14;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq13;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq12;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq11;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq10;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq9;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq8;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq7;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq6;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq5;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq4;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq3;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq2;
	ADC_CHAN_NUMB_SEQ_t			SQRx_ChanNumberSeq1;
	
} Adc_Drv_Regs_t;


typedef struct
{
	ADC_ENABLE_DISABLE_t		CCR_TempSens;
	ADC_ENABLE_DISABLE_t		CCR_VBatt;
	ADC_PRESCALER_t					CCR_Prescaler;
	
} Adc_Drv_RegsCommon_t;


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


#endif /* __ADC_DRVDEFS_H */
