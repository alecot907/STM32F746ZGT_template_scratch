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
	TIMER_CAPT_COMP_IN_TI2,
	TIMER_CAPT_COMP_IN_TI1,
	TIMER_CAPT_COMP_IN_TRC
} TIMER_CAPT_COMP_SEL_t;
typedef enum
{
	TIMER_OUT_COMP_FROZEN = 0x00U,
	TIMER_OUT_COMP_CH1_ACTIVEONMATCH,
	TIMER_OUT_COMP_CH1_INACTIVEONMATCH,
	TIMER_OUT_COMP_TOGGLE,
	TIMER_OUT_COMP_FORCE_INACTIVE,
	TIMER_OUT_COMP_FORCE_ACTIVE,
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
	TIMER_CAPT_COMP_OUTPOL_ACTHIGH = 0x0U,
	TIMER_CAPT_COMP_OUTPOL_ACTLOW
} TIMER_CAPT_COMP_OUTPOL_t;



typedef struct
{
	TIM_TypeDef 							*timer;
	uint32_t		 							Int_Priority;
	TIMER_ENABLE_DISABLE_t		Int_Enable;
	uint32_t		 							PSC_PreScaler;				/* Timer Clock frequency prescaler1 */
	uint32_t		 							ARR_AutoReloadValue;	/* Timer Clock frequency prescaler2 (threshold for overflow) */
	
	TIMER_ENABLE_DISABLE_t 		CR1_UifRemap;
	TIMER_ENABLE_DISABLE_t 		CR1_AutoReload;
	TIMER_ENABLE_DISABLE_t 		CR1_OnePulseMode;
	TIMER_UPDATE_REQ_SOURCE_t	CR1_UpdateReqSource;
	TIMER_ENABLE_DISABLE_t		CR1_UpdateDisable;
	
	TIMER_MASTER_MODE_t				CR2_MasterMode;
	
	TIMER_ENABLE_DISABLE_t		DIER_UpdateDmaReq;
	TIMER_ENABLE_DISABLE_t		DIER_UpdateInterrupt;
	
	TIMER_CAPT_COMP_SEL_t			CCMR1_CaptComp1Selection;
	TIMER_OUT_COMP_MODE_t			CCMR1_OutComp1Mode;
	TIMER_ENABLE_DISABLE_t		CCMR1_OutComp1PreloadEnable;
	TIMER_ENABLE_DISABLE_t		CCMR1_OutComp1FastEnable;

	TIMER_CAPT_COMP_OUTPOL_t	CCER_CaptComp1OutPol;
	TIMER_ENABLE_DISABLE_t		CCER_CaptComp1OutEnable;
} Timer_Drv_Regs_t;


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


#endif /* __TIMER_DRVDEFS_H */
