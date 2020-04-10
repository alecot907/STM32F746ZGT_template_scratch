/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "ClockDrv.h"
#include "RtcDrv.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#define SYS_TICK_PRIORITY					(0)
#define SYS_TICK_SUBPRIORITY			(0)

/* User configurable parameters. Hint: to set these parameters, use the program STM32CubeMX */
#define PWR_CR1_VOS_CFG						(PWR_CR1_VOS_0) /* 01: Scale 3 mode;  10: Scale 2 mode;  11: Scale 1 mode (reset value) */
#define FLASH_ACR_LATENCY_CFG			(FLASH_ACR_LATENCY_2WS)  /* IMPORTANT!!!  See datasheet "Read access latency" to choose this parameter */

#define RCC_PLLCFGR_PLLSRC_CFG		(RCC_PLLCFGR_PLLSRC_HSE)
#define RCC_PLLCFGR_PLLQ_CFG			(RCC_PLLCFGR_PLLQ_1 | RCC_PLLCFGR_PLLQ_0)
#define RCC_PLLCFGR_PLLP_CFG			(0 << RCC_PLLCFGR_PLLP_Pos)
#define RCC_PLLCFGR_PLLN_CFG			(72 << RCC_PLLCFGR_PLLN_Pos)
#define RCC_PLLCFGR_PLLM_CFG			(RCC_PLLCFGR_PLLM_2)

#define RCC_CFGR_PPRE2_CFG				(0 << RCC_CFGR_PPRE2_Pos) /* AHB clock not divided */
#define RCC_CFGR_PPRE1_CFG				(RCC_CFGR_PPRE1_2)  			/* AHB clock divided by 2 */
#define RCC_CFGR_HPRE_CFG					(0 << RCC_CFGR_HPRE_Pos)  /* system clock not divided */
#define RCC_CFGR_SW_CFG						(RCC_CFGR_SW_1)  					/* PLL selected as system clock */

#define RCC_DCKCFGR2_CK48MSEL_CFG (0 << RCC_DCKCFGR2_CK48MSEL_Pos)  /* 48MHz clock from PLL is selected */


#define RCC_BDCR_LSEDRV_CFG  			(0 << RCC_BDCR_LSEDRV_Pos) 	/* High driving capability */
#define RCC_BDCR_LSEBYP_CFG   		(0 << RCC_BDCR_LSEBYP_Pos) 	/* LSE oscillator not bypassed */


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#ifndef OS_USE
volatile uint32_t sys_ticks = 0UL;
#endif

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* SysTick_Init */
/**************************************************************************************/
ErrorStatus SysTick_Init (void)
{
	/* Update systick. Note: after these calls, SystemCoreClock contains the updated value of SYSCLK */
	SystemCoreClockUpdate();
	if ( SysTick_Config(SystemCoreClock / 1000) >0 )
	{
		return ERROR;
	}
	
	return SUCCESS;
}

/**************************************************************************************/
/* ClockDrv_Init */
/**************************************************************************************/
ErrorStatus ClockDrv_Init(void)
{	
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), SYS_TICK_PRIORITY, SYS_TICK_SUBPRIORITY));
	
	if ( ERROR == SysTick_Init() )
	{
		return ERROR;
	}
	
	/* Enable Power clock */
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);
	/* Enable System configuration controller */
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
	/* Enable access to RTC and backup registers */
  SET_BIT(PWR->CR1, PWR_CR1_DBP);
	MODIFY_REG(RCC->BDCR, RCC_BDCR_LSEDRV, RCC_BDCR_LSEDRV_CFG);

	/* Select the Regulator voltage scaling */
	MODIFY_REG(PWR->CR1, PWR_CR1_VOS, PWR_CR1_VOS_CFG);
	/* Check if Regulator voltage scaling was correctly written */
	if ( PWR_CR1_VOS_CFG != READ_BIT(PWR->CR1, PWR_CR1_VOS) )
	{
		return ERROR;
	}	
	
	/* HSE oscillator source clock selection, enable and wait for stabilization */
	SET_BIT(RCC->CR, RCC_CR_HSEBYP);  // this setting could be parametrical
	SET_BIT(RCC->CR, RCC_CR_HSEON);
	while( !READ_BIT(RCC->CR, RCC_CR_HSERDY) );
		
	/* LSE oscillator source clock selection, enable and wait for stabilization */
	MODIFY_REG(RCC->BDCR, RCC_BDCR_LSEBYP, RCC_BDCR_LSEBYP_CFG);
	SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);
	while ( !READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY) );
	
	/* PLL disable and wait for PLL stabilization (this operation must be executed after HSE stabilization) */
	CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
	while ( READ_BIT(RCC->CR, RCC_CR_PLLRDY) );	

	/* PLL configuration */
	SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_CFG);
	MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, RCC_PLLCFGR_PLLM_CFG);
	MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, RCC_PLLCFGR_PLLN_CFG);
	MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLP, RCC_PLLCFGR_PLLP_CFG);
	MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ, RCC_PLLCFGR_PLLQ_CFG);
	
	/* PLL enable and wait for PLL stabilization (this operation must be executed after HSE stabilization) */
	SET_BIT(RCC->CR, RCC_CR_PLLON);
	while ( !READ_BIT(RCC->CR, RCC_CR_PLLRDY) );	
		
	/* If to-be-set latency is more than already-set latency, set new latency (Increasing the CPU frequency)  */
	if ( FLASH_ACR_LATENCY_CFG > (READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY)) )
	{
		MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_CFG);
		if ( FLASH_ACR_LATENCY_CFG != READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY) )
		{
		return ERROR;
		}	
		/* If at least one wait state of latency is needed, enable the Prefetch */
		if (READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY) > FLASH_ACR_LATENCY_0WS)
		{
			SET_BIT(FLASH->ACR, FLASH_ACR_PRFTEN);
		}
	}
	
	/* APB1 prescaler */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_CFG);    //111
	/* APB2 prescaler */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, (RCC_CFGR_PPRE2_CFG));   //111
	/* AHB prescaler */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_CFG);

	/* SYSCLK source mux selection (this operation must be executed after PLL enable */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_CFG);	
	while ( (RCC_CFGR_SW_CFG << 2) != READ_BIT(RCC->CFGR, RCC_CFGR_SWS) );
	
	/* If to-be-set latency is less than already-set latency, set new latency (Decreasing the CPU frequency)  */
	if ( FLASH_ACR_LATENCY_CFG < (READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY)) )
	{
		MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_CFG);
		if ( FLASH_ACR_LATENCY_CFG != READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY) )
		{
		return ERROR;
		}
		/* If at least one wait state of latency is needed, enable the Prefetch */
		if (READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY) > FLASH_ACR_LATENCY_0WS)
		{
			SET_BIT(FLASH->ACR, FLASH_ACR_PRFTEN);
		}
	}
	
	if ( ERROR == SysTick_Init() )
	{
		return ERROR;
	}
	
	/* CLK48 clock seurce selection */
	MODIFY_REG(RCC->DCKCFGR2, RCC_DCKCFGR2_CK48MSEL, RCC_DCKCFGR2_CK48MSEL_CFG);
		
	return SUCCESS;
}


/**************************************************************************************/
/* SysTick_Handler */
/**************************************************************************************/
#ifndef OS_USE
void SysTick_Handler(void)
{
	sys_ticks++;
}
#endif
