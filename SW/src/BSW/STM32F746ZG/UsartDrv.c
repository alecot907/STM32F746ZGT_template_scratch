/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "UsartDrv.h"
#include "ClockDrv.h"
#include "GpioDrv.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/* Pin configuration */
#define UART2_TX_SPEED_CFG	(GPIO_OSPEEDER_OSPEEDR5)  // very high speed
#define UART2_RX_SPEED_CFG	(GPIO_OSPEEDER_OSPEEDR6)  // very high speed


#define USART2_CLKSELECTION_CFG		(0b00)	// 00: APB1 clock (PCLK1)	01: System clock	10: HSI clock		11: LSE clock
#define USART2_OVERSAMPLING_CFG		(0)  // 0: Oversampling by 16		1: Oversampling by 8
#define USART2_PARITYENABLE_CFG		(0)  // 0: Parity control disabled		1: Parity control enabled
#define USART2_PARITYSEL_CFG			(0)  // 0: Even parity		1: Odd parity
#define USART2_PEINTENABLE_CFG		(0)  // 0: Interrupt is inhibited		1: Interrupt is enabled
#define USART2_TXINTENABLE_CFG		(0)  // 0: Interrupt is inhibited		1: Interrupt is enabled
#define USART2_TCINTENABLE_CFG		(0)  // 0: Interrupt is inhibited		1: Interrupt is enabled
#define USART2_RXINTENABLE_CFG		(1)  // 0: Interrupt is inhibited		1: Interrupt is enabled
#define USART2_IDLEINTENABLE_CFG	(0)  // 0: Interrupt is inhibited		1: Interrupt is enabled

#define USART2_MSBPOS_CFG					(0)  // 0: data bit 0 first		1: MSB (bit 7/8/9) first
#define USART2_DATAINV_CFG				(0)  // 0: (1=H, 0=L)		1: (1=L, 0=H)
#define USART2_TXINV_CFG					(1)  // 0: (VDD =1/idle, Gnd=0/mark)	1: (VDD =0/mark, Gnd=1/idle)
#define USART2_RXINV_CFG					(1)  // 0: (VDD =1/idle, Gnd=0/mark)	1: (VDD =0/mark, Gnd=1/idle)
#define USART2_SWAP_CFG						(0)  // 0: RX/TX standard		1: RX/TX swapped
#define USART2_STOPBIT_CFG				(0b00)  //00: 1 stop bit		01: 0.5 stop bit		10: 2 stop bits		11: 1.5 stop bits

#define USART2_DMAT_CFG						(0)  // 0: DMA mode is disabled for transmission    1: DMA mode is enabled for transmission
#define USART2_DMAR_CFG						(0)  // 0: DMA mode is disabled for reception    1: DMA mode is enabled for reception
#define USART2_ERRORINT_CFG				(0)  // 0: Interrupt is inhibited    1: Interrupt is enabled

#if (USART2_PARITYENABLE_CFG == 0)
	#define USART2_RXMASK						(0xFF)
#else
	#define USART2_RXMASK						(0x7F)
#endif


#define MAX_BYTE_TXBUFF							(128U)  // maximum bytes to TX in the buffers
#define MAX_BYTE_RXBUFF							(128U)  // maximum bytes to RX in the buffers

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
static char tx_buff[MAX_BYTE_TXBUFF];
static char rx_buff[MAX_BYTE_RXBUFF];


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* UsartBaudCalc */
/**************************************************************************************/
static ErrorStatus UsartBaudCalc(uint32_t BaudRate, uint32_t *BaudReg);

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* UsartDrv_GpioInit */
/**************************************************************************************/
void UsartDrv_GpioInit(void)
{
	/* USART 2 (PD5: TX   PD6:RX) */
	// Alternate function mode
	MODIFY_REG(GPIOD->MODER, (GPIO_MODER_MODER5 | GPIO_MODER_MODER6), (GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1));
	// TX pin speed configuration
	MODIFY_REG(GPIOD->OSPEEDR, (GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6), (UART2_TX_SPEED_CFG | UART2_RX_SPEED_CFG)); // TX pin speed
	// alternate function selection
	MODIFY_REG(GPIOD->AFR[0], (GPIO_AFRL_AFRL5 | GPIO_AFRL_AFRL6), ((0b0111 << GPIO_AFRL_AFRL5_Pos) | 0b0111 << GPIO_AFRL_AFRL6_Pos)); // AF7
	// Rx Pullup
	SET_BIT(GPIOD->PUPDR, GPIO_PUPDR_PUPDR6_0);
}

/**************************************************************************************/
/* UsartDrv_Init */
/**************************************************************************************/
ErrorStatus UsartDrv_Init (uint32_t BaudRate)
{
	ErrorStatus result = SUCCESS;
	uint32_t reg_temp;
		
	/* USART 2 */
	/* Clock source selection */
	MODIFY_REG(RCC->DCKCFGR2, RCC_DCKCFGR2_USART2SEL, USART2_CLKSELECTION_CFG << RCC_DCKCFGR2_USART2SEL_Pos);
	/* Enable peripheral clock */
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);
	
	/* Disable Peripheral */
	CLEAR_BIT(USART2->CR1, USART_CR1_UE);
	
	/* Configuration */
	reg_temp = (((0b00 & 0b01) << USART_CR1_M_Pos) | ((0b00 & 0b10) << (28-1))) |
						  (USART2_OVERSAMPLING_CFG << USART_CR1_OVER8_Pos) |
							(USART2_PARITYENABLE_CFG << USART_CR1_PCE_Pos) |
							(USART2_PARITYSEL_CFG << USART_CR1_PS_Pos) |
							(USART2_PEINTENABLE_CFG << USART_CR1_PEIE_Pos) |
							(USART2_TXINTENABLE_CFG << USART_CR1_TXEIE_Pos) |
							(USART2_TCINTENABLE_CFG << USART_CR1_TCIE_Pos) |
							(USART2_RXINTENABLE_CFG << USART_CR1_RXNEIE_Pos) |
							(USART2_IDLEINTENABLE_CFG << USART_CR1_IDLEIE_Pos) |
							(1 << USART_CR1_TE_Pos) |
							(1 << USART_CR1_RE_Pos);
	MODIFY_REG(USART2->CR1, 0xFFFFFFFF, reg_temp);
	
	reg_temp = (USART2_MSBPOS_CFG << USART_CR2_MSBFIRST_Pos) |
							(USART2_DATAINV_CFG << USART_CR2_DATAINV_Pos) |
							(USART2_TXINV_CFG << USART_CR2_TXINV_Pos) |
							(USART2_RXINV_CFG << USART_CR2_RXINV_Pos) |
							(USART2_SWAP_CFG << USART_CR2_SWAP_Pos) |
							(USART2_STOPBIT_CFG << USART_CR2_STOP_Pos);
	MODIFY_REG(USART2->CR2, 0xFFFFFFFF, reg_temp);
	
	reg_temp = (USART2_DMAT_CFG << USART_CR3_DMAT_Pos) |
							(USART2_DMAR_CFG << USART_CR3_DMAR_Pos) |
							(USART2_ERRORINT_CFG << USART_CR3_EIE_Pos);							
	MODIFY_REG(USART2->CR3, 0xFFFFFFFF, reg_temp);
	
	result = UsartBaudCalc(BaudRate, &reg_temp);
	MODIFY_REG(USART2->BRR, 0xFFFFFFFF, reg_temp);	
		
	/* Enable Peripheral */
	SET_BIT(USART2->CR1, USART_CR1_UE);
	
	return result;
}


/**************************************************************************************/
/* UsartDrv_Int */
/**************************************************************************************/
void UsartDrv_Int (void)
{
	/* Read data */
	USART2->RDR;
		
	/* EXTI RX interrupt enable */
	SET_BIT(EXTI->IMR, EXTI_IMR_IM6);
	SET_BIT(EXTI->RTSR, EXTI_RTSR_TR6);
	
	/* Configure and enable the USART2 IRQ channel in the NVIC. */
	NVIC_SetPriority (USART2_IRQn, (6UL << __NVIC_PRIO_BITS) - 1UL);
	NVIC_EnableIRQ(USART2_IRQn);
}

/**************************************************************************************/
/* UsartBaudCalc */
/**************************************************************************************/
static ErrorStatus UsartBaudCalc(uint32_t BaudRate, uint32_t *BaudReg)
{
	uint32_t clock;
	uint32_t usartdiv = 0U;
	
	/* clock calculation */
	switch (USART2_CLKSELECTION_CFG)
	{
		case 0b00:  // APB1 clock (PCLK1)
			switch (RCC_CFGR_PPRE2_CFG >> RCC_CFGR_PPRE2_Pos)
			{
				case 0b100:
					clock = SystemCoreClock / 2;
					break;
				case 0b101:
					clock = SystemCoreClock / 4;
					break;
				case 0b110:
					clock = SystemCoreClock / 5;
					break;
				case 0b111:
					clock = SystemCoreClock / 16;
					break;
				default:
					clock = SystemCoreClock;
					break;
			}
			switch (RCC_CFGR_PPRE1_CFG >> RCC_CFGR_PPRE1_Pos)
			{
				case 0b100:
					clock = clock / 2;
					break;
				case 0b101:
					clock = clock / 4;
					break;
				case 0b110:
					clock = clock / 5;
					break;
				case 0b111:
					clock = clock / 16;
					break;
				default:
					clock = clock;
					break;
			}
			break;
		case 0b01:  // System clock
			clock = SystemCoreClock;
			break;
		case 0b10:  // HSI clock
			clock = HSI_VALUE;
			break;
		case 0b11:  // LSE clock
			clock = LSE_VALUE;
			break;
		default:
			break;
	}
	
	if (0 == USART2_OVERSAMPLING_CFG)
	{
		usartdiv = clock / (BaudRate);
		*BaudReg = usartdiv;
	}
	else
	{
		/*	– BRR[2:0] = USARTDIV[3:0] shifted 1 bit to the right.
			– BRR[3] must be kept cleared.
			– BRR[15:4] = USARTDIV[15:4] */
		usartdiv = 2 * clock / (BaudRate);
		*BaudReg = ((usartdiv & 0b1111) >> 1) |
							((usartdiv & 0xFFF0) << 4);
	}
	
	/* USARTDIV must be greater than or equal to 0d16 */
	if ( (usartdiv <= 0x10) || (usartdiv >= 0x0000FFFF) )
	{
		return ERROR;
	}
	
	return SUCCESS;
}


/**************************************************************************************/
/* Usart2data_TXpoll */
/**************************************************************************************/
ErrorStatus Usart2data_TXpoll (uint8_t *data, uint32_t size)
{	
	if ( (data == NULL) || (size == 0))
	{
		return ERROR;
	}
	else
	{
		uint32_t size_cnt = 0;
		uint8_t *data_ptr = NULL;

		data_ptr = data;
		/* Transmit all data */
		while (size_cnt < size)
		{
			/* Wait until TX register is empty (ready to be written) */
			while (!READ_BIT(USART2->ISR, USART_ISR_TXE));
			USART2->TDR = (*data_ptr) & 0xFF;
			data_ptr++;
			size_cnt++;
		}
			/* Wait until transfer is complete (TX shadow register empty) */
		while(!READ_BIT(USART2->ISR, USART_ISR_TC));
	}
	
	return SUCCESS;
}

/**************************************************************************************/
/* Usart2string_TXpoll */
/**************************************************************************************/
ErrorStatus Usart2string_TXpoll (const char *str, ...)
{	
	if (str == NULL)
	{
		return ERROR;
	}
	else
	{
		char *data_ptr = NULL;
		/* Create a ponter to the not declared parameters of the function */
		static va_list arg;
		/* Initialize arg so that it points to the area immediatly after the last parameter (&str += 4) */
		va_start(arg, str);
		/* compose the total string of str + additional arguments inside dbg_tx_buff */
		vsprintf(tx_buff, str, arg);
		/* Release arg */
		va_end(arg);
		data_ptr = tx_buff;
		
			/* Transmit all data */
		while (*data_ptr != '\0')
		{
			/* Wait until TX register is empty (ready to be written) */
			while (!READ_BIT(USART2->ISR, USART_ISR_TXE));
			USART2->TDR = (uint8_t)(*data_ptr);
			data_ptr++;
		}
		/* Wait until transfer is complete (TX shadow register empty) */
		while(!READ_BIT(USART2->ISR, USART_ISR_TC));
	}
	
	return SUCCESS;
}

/**************************************************************************************/
/* Usart2string_RXpoll */
/**************************************************************************************/
ErrorStatus Usart2string_RXpoll (uint8_t *data, uint32_t size)
{
	/* Used in polling */
	uint8_t *data_ptr = data;
	
	if (READ_BIT(USART2->ISR, USART_ISR_RXNE))
	{
		*data_ptr = (uint8_t) (USART2->RDR & USART2_RXMASK);
	}
	
	return SUCCESS;
}


/**************************************************************************************/
/* USART2_IRQHandler */
/**************************************************************************************/
void USART2_IRQHandler (void)
{
	/* Used in interrupt */

	if (READ_BIT(USART2->ISR, USART_ISR_RXNE))
	{
		rx_buff[0] = (uint8_t) (USART2->RDR & USART2_RXMASK);
		
//		/* TEST */
//		static GPIO_STATE led = GPIO_LOW;
//		led = (GPIO_STATE) !led;
//		GpioDrv_SetPin(LED_BLUE_PORT, LED_BLUE_PIN, led);
//		Usart2string_TXpoll("\nHo ricevuto: %d", data_rx);
//		static uint32_t rx_test;
//		rx_test++;
	}
}

