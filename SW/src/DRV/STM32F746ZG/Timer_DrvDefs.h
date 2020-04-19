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



typedef struct
{
	TIM_TypeDef 							*timer;
	uint32_t		 							Int_priority;
	uint32_t		 							PreScaler;				/* Timer Clock frequency prescaler1 */
	uint32_t		 							AutoReloadValue;	/* Timer Clock frequency prescaler2 (threshold for overflow) */
	
	TIMER_ENABLE_DISABLE_t 		CR1_UifRemap;
	TIMER_ENABLE_DISABLE_t 		CR1_AutoReload;
	TIMER_ENABLE_DISABLE_t 		CR1_OnePulseMode;
	TIMER_UPDATE_REQ_SOURCE_t	CR1_UpdateReqSource;
	TIMER_ENABLE_DISABLE_t		CR1_UpdateDisable;
	
	TIMER_MASTER_MODE_t				CR2_MasterMode;
	
	TIMER_ENABLE_DISABLE_t		DIER_UpdateDmaReq;
	TIMER_ENABLE_DISABLE_t		DIER_UpdateInterrupt;
} TimerBasic_Drv_Regs_t;


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


#endif /* __TIMER_DRVDEFS_H */
