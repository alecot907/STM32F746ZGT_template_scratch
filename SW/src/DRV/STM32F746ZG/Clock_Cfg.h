#ifndef __CLOCK_CFG_H
#define __CLOCK_CFG_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "stm32f7xx.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#define SYS_TICK_PRIORITY					(0x0U)
#define SYS_TICK_SUBPRIORITY			(0x0U)


/* User configurable parameters. Hint: to set these parameters, use the program STM32CubeMX */

#define PWR_CR1_VOS_CFG						(PWR_CR1_VOS_0 >> PWR_CR1_VOS_Pos) /* 01: Scale 3 mode;  10: Scale 2 mode;  11: Scale 1 mode (reset value) */
#define FLASH_ACR_LATENCY_CFG			(FLASH_ACR_LATENCY_2WS >> FLASH_ACR_LATENCY_Pos)  /* IMPORTANT!!!  See datasheet "Read access latency" to choose this parameter */

#define RCC_PLLCFGR_PLLSRC_CFG		(RCC_PLLCFGR_PLLSRC_HSE >> RCC_PLLCFGR_PLLSRC_Pos)
#define RCC_PLLCFGR_PLLQ_CFG			((RCC_PLLCFGR_PLLQ_0 | RCC_PLLCFGR_PLLQ_1) >> RCC_PLLCFGR_PLLQ_Pos)
#define RCC_PLLCFGR_PLLP_CFG			(0x0U >> RCC_PLLCFGR_PLLP_Pos)
#define RCC_PLLCFGR_PLLN_CFG			(72U)
#define RCC_PLLCFGR_PLLM_CFG			(RCC_PLLCFGR_PLLM_2 >> RCC_PLLCFGR_PLLM_Pos)

#define RCC_CFGR_PPRE1_CFG				(RCC_CFGR_PPRE1_DIV2 >> RCC_CFGR_PPRE1_Pos) /* AHB clock divided by 2 (APB1) */
#define RCC_CFGR_PPRE2_CFG				(RCC_CFGR_PPRE2_DIV1 >> RCC_CFGR_PPRE2_Pos) /* AHB clock not divided (APB2 )*/
#define RCC_CFGR_HPRE_CFG					(RCC_CFGR_HPRE_DIV1 >> RCC_CFGR_HPRE_Pos)  /* system clock not divided */

#define RCC_CFGR_SW_CFG						(RCC_CFGR_SW_1 >> RCC_CFGR_SW_Pos)  					/* PLL selected as system clock */

#define RCC_BDCR_LSEDRV_CFG  			(0x0U) 	/* High driving capability */
#define RCC_BDCR_LSEBYP_CFG   		(0x0U) 	/* LSE oscillator not bypassed */


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


#endif /* __CLOCK_CFG_H */
