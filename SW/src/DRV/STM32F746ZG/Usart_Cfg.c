/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "stm32f7xx.h"
#include "Usart_Drv.h"
#include "Usart_Cfg.h"
#include "Usart.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

const Usart_Drv_Regs_t Usart_Drv_Regs[USART_LIST_TOTAL] = 
{
	{
		0x2U,
		USART2,
		UART2_RX_PIN,
		USART2_IRQn,
		6UL,
		
		USART_CLOCKSEL_APBx,
		
		USART_DISABLED,
		USART_OVERSAMPLING16,
		USART_DISABLED,
		USART2_PARITYENABLE_CFG,
		USART_PARITYSEL_EVEN,
		USART_DISABLED,
		USART_DISABLED,
		USART_DISABLED,
		USART_ENABLED, /* RX interrupt */
		USART_DISABLED,
		
		USART_MSBFIRST_LSB,
		USART_DATAINV_STANDARD,
		USART_DATAINV_INVERTED,
		USART_DATAINV_INVERTED,
		USART_DATAINV_STANDARD,
		USART_STOPBITS_1,
		USART_DISABLED,
		USART_CLOCKPOL_STEADYLOW,
		USART_CLOCKPHASE_1ST,
		USART_LBCL_NO,
		
		USART_BAUD_9600
	}
};

static uint8_t Usart2_RXbuff;

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/**************************************************************************************/
/* USART2_IRQHandler */
/**************************************************************************************/
void USART2_IRQHandler (void)
{
	/* RX management */
	if (READ_BIT(USART2->ISR, USART_ISR_RXNE))
	{
		Usart2_RXbuff = USART2->RDR & USART2_RXMASK;
		
		Usart2RX_mng(&Usart2_RXbuff);
	}
	/* TX management using Interrupt */
	else if (READ_BIT(USART2->CR1, USART_CR1_TXEIE))  
	{
		UsartData_TXint(USART2_DBG_CMD);
	}
}
