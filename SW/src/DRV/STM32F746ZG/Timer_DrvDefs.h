#ifndef __TIMER_DRVDEFS_H
#define __TIMER_DRVDEFS_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "stm32f7xx.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

typedef enum
{
	TIMER_DISABLE = 0x0U,
	TIMER_ENABLE
} TIMER_ENABLE_DISABLE_t;
typedef enum
{
	TIMER_MORE_SOURCES = 0x0U,
	TIMER_UNDER_OVER_FLOW
} TIMER_UPDATE_REQ_SOURCE_t;
typedef enum
{
	TIMER_MASTER_MODE_RESET = 0x0U,
	TIMER_MASTER_MODE_ENABLE,
	TIMER_MASTER_MODE_UPDATE
} TIMER_MASTER_MODE_t;

typedef enum
{
	TIMER_CAPT_COMP_OUT = 0x0U,
	TIMER_CAPT_COMP_IN_TI1,
	TIMER_CAPT_COMP_IN_TI2,
	TIMER_CAPT_COMP_IN_TRC
} TIMER_CAPT_COMP_SEL_t;
typedef enum
{
	TIMER_OUT_COMP_FROZEN = 0x00U,  //The comparison between the output compare register TIMx_CCR1 and the counter TIMx_CNT has no effect on the outputs
	TIMER_OUT_COMP_CH1_ACTIVEONMATCH,
	TIMER_OUT_COMP_CH1_INACTIVEONMATCH,
	TIMER_OUT_COMP_TOGGLE,  // OC1REF toggles when TIMx_CNT = TIMx_CCR1.
	TIMER_OUT_COMP_FORCE_INACTIVE,  // OC1REF is forced low
	TIMER_OUT_COMP_FORCE_ACTIVE,  // OC1REF is forced high
	TIMER_OUT_COMP_PWM1,  //channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else it is inactive
	TIMER_OUT_COMP_PWM2,  //  channel 1 is inactive as long as TIMx_CNT<TIMx_CCR1 else it is active
	TIMER_OUT_COMP_RETRIGGERABLE1,
	TIMER_OUT_COMP_RETRIGGERABLE2,
	TIMER_OUT_COMP_RESERVED10,
	TIMER_OUT_COMP_RESERVED21,
	TIMER_OUT_COMP_COMB_PWM1,
	TIMER_OUT_COMP_COMB_PWM2,
	TIMER_OUT_COMP_RESERVED14,
	TIMER_OUT_COMP_RESERVED15
} TIMER_OUT_COMP_MODE_t;
typedef enum
{
	TIMER_CAPT_COMP_OUTPOL_OUT_ACTHIGH = 0x0U,
	TIMER_CAPT_COMP_OUTPOL_OUT_ACTLOW
} TIMER_CAPT_COMP_OUTPOL_OUT_t;
typedef enum
{
	TIMER_IN_CAPT_FILTER_F0 = 0x0U,
	TIMER_IN_CAPT_FILTER_F1,
	TIMER_IN_CAPT_FILTER_F2,
	TIMER_IN_CAPT_FILTER_F3,
	TIMER_IN_CAPT_FILTER_F4,
	TIMER_IN_CAPT_FILTER_F5,
	TIMER_IN_CAPT_FILTER_F6,
	TIMER_IN_CAPT_FILTER_F7,
} TIMER_IN_CAPT_FILTER_t;
typedef enum
{
	TIMER_IN_CAPT_PRE_ALL = 0x0U,
	TIMER_IN_CAPT_PRE_2,
	TIMER_IN_CAPT_PRE_4,
	TIMER_IN_CAPT_PRE_8
} TIMER_IN_CAPT_PRE_t;
typedef enum
{
	TIMER_CAPT_COMP_OUTPOL_IN_NONINVRIS = 0x0U,  // Circuit is sensitive to TI1FP1 rising edge (capture mode), TI1FP1 is not inverted
	TIMER_CAPT_COMP_OUTPOL_IN_INVFAL,  //  Circuit is sensitive to TI1FP1 falling edge (capture mode), TI1FP1 is inverted
	TIMER_CAPT_COMP_OUTPOL_IN_RES,
	TIMER_CAPT_COMP_OUTPOL_IN_NONINVBOTH // Circuit is sensitive to both TI1FP1 rising and falling edges (capture mode), TI1FP1 is not inverted.
} TIMER_CAPT_COMP_OUTPOL_IN_t;



typedef struct
{
	TIM_TypeDef 										*timer;
	uint32_t		 										Int_Priority;
	uint32_t		 										PSC_PreScaler;				/* Timer Clock frequency prescaler1 */
	uint32_t		 										ARR_AutoReloadValue;	/* Timer Clock frequency prescaler2 (threshold for overflow) */
	uint32_t		 										CCR1_CaptCompValue;
	
	TIMER_ENABLE_DISABLE_t 					CR1_UifRemap;
	TIMER_ENABLE_DISABLE_t 					CR1_AutoReload;
	TIMER_ENABLE_DISABLE_t 					CR1_OnePulseMode;
	TIMER_UPDATE_REQ_SOURCE_t				CR1_UpdateReqSource;
	TIMER_ENABLE_DISABLE_t					CR1_UpdateDisable;
	
	TIMER_MASTER_MODE_t							CR2_MasterMode;
	
	TIMER_ENABLE_DISABLE_t					DIER_UpdateDmaReq;
	TIMER_ENABLE_DISABLE_t					DIER_CaptCompInterrupt;
	TIMER_ENABLE_DISABLE_t					DIER_UpdateInterrupt;
	
	TIMER_OUT_COMP_MODE_t						CCMR1_OutComp1Mode;
	TIMER_ENABLE_DISABLE_t					CCMR1_OutComp1PreloadEnable;
	TIMER_ENABLE_DISABLE_t					CCMR1_OutComp1FastEnable;
	TIMER_CAPT_COMP_SEL_t						CCMR1_CaptComp1Selection;
	TIMER_IN_CAPT_FILTER_t					CCMR1_InCaptureFilter;
	TIMER_IN_CAPT_PRE_t							CCMR1_InCapturePrescaler;
	
	TIMER_CAPT_COMP_OUTPOL_OUT_t		CCER_CaptComp1OutPolOut;
	TIMER_CAPT_COMP_OUTPOL_IN_t			CCER_CaptComp1OutPolIn;
	TIMER_ENABLE_DISABLE_t					CCER_CaptComp1OutEnable;
} Timer_Drv_Regs_t;


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


#endif /* __TIMER_DRVDEFS_H */
