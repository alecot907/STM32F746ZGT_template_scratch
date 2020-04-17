/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Clock_Drv.h"
#include "Clock_Cfg.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


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
/* Clock_Drv_Init */
/**************************************************************************************/
ErrorStatus Clock_Drv_Init(void)
{	
//	Clock_Drv_SystickInt();
	
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
	MODIFY_REG(RCC->BDCR, RCC_BDCR_LSEDRV, RCC_BDCR_LSEDRV_CFG << RCC_BDCR_LSEDRV_Pos);

	/* Select the Regulator voltage scaling */
	MODIFY_REG(PWR->CR1, PWR_CR1_VOS, PWR_CR1_VOS_CFG << PWR_CR1_VOS_Pos);
	/* Check if Regulator voltage scaling was correctly written */
	if ( (PWR_CR1_VOS_CFG << PWR_CR1_VOS_Pos) != READ_BIT(PWR->CR1, PWR_CR1_VOS) )
	{
		return ERROR;
	}	
	
	/* HSE oscillator source clock selection, enable and wait for stabilization */
	SET_BIT(RCC->CR, (RCC_CR_HSEBYP | RCC_CR_HSEON));
	while( !READ_BIT(RCC->CR, RCC_CR_HSERDY) );
		
	/* LSE oscillator source clock selection, enable and wait for stabilization */
	MODIFY_REG(RCC->BDCR, RCC_BDCR_LSEBYP, RCC_BDCR_LSEBYP_CFG << RCC_BDCR_LSEBYP_Pos);
	SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);
	while ( !READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY) );
	
	/* PLL disable and wait for PLL stabilization (this operation must be executed after HSE stabilization) */
	CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
	while ( READ_BIT(RCC->CR, RCC_CR_PLLRDY) );	

	/* PLL configuration */
	SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_CFG << RCC_PLLCFGR_PLLSRC_HSE_Pos);
	MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, RCC_PLLCFGR_PLLM_CFG << RCC_PLLCFGR_PLLM_Pos);
	MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, RCC_PLLCFGR_PLLN_CFG << RCC_PLLCFGR_PLLN_Pos);
	MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLP, (RCC_PLLCFGR_PLLP_CFG << RCC_PLLCFGR_PLLP_Pos));
	MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ, RCC_PLLCFGR_PLLQ_CFG << RCC_PLLCFGR_PLLQ_Pos);
	
	/* PLL enable and wait for PLL stabilization (this operation must be executed after HSE stabilization) */
	SET_BIT(RCC->CR, RCC_CR_PLLON);
	while ( !READ_BIT(RCC->CR, RCC_CR_PLLRDY) );	
		
	/* If to-be-set latency is more than already-set latency, set new latency (Increasing the CPU frequency)  */
	if ( (FLASH_ACR_LATENCY_CFG << FLASH_ACR_LATENCY_Pos) > (READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY)) )
	{
		MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_CFG << FLASH_ACR_LATENCY_Pos);
		if ( (FLASH_ACR_LATENCY_CFG << FLASH_ACR_LATENCY_Pos) != READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY) )
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
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_CFG << RCC_CFGR_PPRE1_Pos);
	/* APB2 prescaler */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_CFG << RCC_CFGR_PPRE2_Pos);
	/* AHB prescaler */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_CFG << RCC_CFGR_HPRE_Pos);

	/* SYSCLK source mux selection (this operation must be executed after PLL enable */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_CFG << RCC_CFGR_SW_Pos);	
	while ( ((RCC_CFGR_SW_CFG << RCC_CFGR_SW_Pos) << 2) != READ_BIT(RCC->CFGR, RCC_CFGR_SWS) );
	
	/* If to-be-set latency is less than already-set latency, set new latency (Decreasing the CPU frequency)  */
	if ( (FLASH_ACR_LATENCY_CFG << FLASH_ACR_LATENCY_Pos) < (READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY)) )
	{
		MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, (FLASH_ACR_LATENCY_CFG << FLASH_ACR_LATENCY_Pos));
		if ( (FLASH_ACR_LATENCY_CFG << FLASH_ACR_LATENCY_Pos) != READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY) )
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
	
		
	return SUCCESS;
}


/**************************************************************************************/
/* Clock_Drv_SystickInt */
/**************************************************************************************/
void Clock_Drv_SystickInt(void)
{
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), SYS_TICK_PRIORITY, SYS_TICK_SUBPRIORITY));
}

/**************************************************************************************/
/* SysTick_Handler */
/**************************************************************************************/
#ifndef OS_USE
void SysTick_Handler(void)
{
	sys_ticks++;
}

/**************************************************************************************/
/* Delay_ms */
/**************************************************************************************/
void Delay_ms (uint32_t	delay_ms)
{
	uint32_t TimeInit;
	
	TimeInit = sys_ticks;
	
	while( (sys_ticks - TimeInit) < delay_ms);
}
#endif
