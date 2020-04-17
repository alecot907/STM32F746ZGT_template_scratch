/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include <stdarg.h>
#include <stdio.h>

#include "Usart_Drv.h"
#include "Usart_DrvDefs.h"
#include "Clock_Cfg.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
typedef struct 
{	
	uint8_t tx_buff[MAX_BYTE_TXBUFF];
	uint16_t tx_CurrIdx;  	// current index inside the buffer
	uint16_t tx_StartIdx;	// Initial index inside the buffer
	uint16_t tx_EndIdx;		// Final index inside the buffer
	uint16_t tx_TempIdx;  	// temporary index
	uint16_t tx_size;		// Final index inside the buffer
	
	uint8_t *data_ptr;
} Usart_t;

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
static Usart_t UsartObj[USART_LIST_TOTAL];

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* UsartBaudCalc */
/**************************************************************************************/
static ErrorStatus UsartBaudCalc(USART_LIST_t uart_list, uint32_t *BaudReg);

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* Usart_Drv_Init */
/**************************************************************************************/
ErrorStatus Usart_Drv_Init (void)
{
	ErrorStatus result = SUCCESS;
	uint32_t reg_temp;
	
	for (USART_LIST_t uart_list = USART_LIST_START; uart_list < USART_LIST_TOTAL; uart_list++)
	{
		/* Clock source selection */
		MODIFY_REG(RCC->DCKCFGR2, 0x03U << ((Usart_Drv_Regs[uart_list].UsartNumber - 0x1U) * 0x2U),
							Usart_Drv_Regs[uart_list].DCKCFGR2_ClockSelection << ((Usart_Drv_Regs[uart_list].UsartNumber - 0x1U) * 0x2U));
		
		/* Enable peripheral clock */
		switch (Usart_Drv_Regs[uart_list].UsartNumber)
		{
			case 0x1U:
				SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);
				break;
			case 0x2U:
				SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);
				break;
			case 0x3U:
				SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);
				break;
			case 0x4U:
				SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART4EN);
				break;
			case 0x5U:
				SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART5EN);
				break;
			case 0x6U:
				SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART6EN);
				break;
			case 0x7U:
				SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART7EN);
				break;
			case 0x8U:
				SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART7EN);
				break;
		}
		
		/* Disable Peripheral */
		CLEAR_BIT(Usart_Drv_Regs[uart_list].port->CR1, USART_CR1_UE);
		
		/* Configuration */
		reg_temp = (((0x0U & 0x1U) << USART_CR1_M_Pos) | ((0x0U & 0x2U) << (28U - 1U))) | // 8 data bits hard coded
								(Usart_Drv_Regs[uart_list].CR1_EndOfBlock_Interrupt << USART_CR1_EOBIE_Pos) |
								(Usart_Drv_Regs[uart_list].CR1_Oversapling << USART_CR1_OVER8_Pos) |
								(Usart_Drv_Regs[uart_list].CR1_CharMatch_Interrupt << USART_CR1_CMIE_Pos) |
								(Usart_Drv_Regs[uart_list].CR1_ParityEnable << USART_CR1_PCE_Pos) |
								(Usart_Drv_Regs[uart_list].CR1_ParityType << USART_CR1_PS_Pos) |
								(Usart_Drv_Regs[uart_list].CR1_PE_Interrupt << USART_CR1_PEIE_Pos) |
								(Usart_Drv_Regs[uart_list].CR1_TXEmpty_Interrupt << USART_CR1_TXEIE_Pos) |
								(Usart_Drv_Regs[uart_list].CR1_TXComplete_Interrupt << USART_CR1_TCIE_Pos) |
								(Usart_Drv_Regs[uart_list].CR1_RXNotEmpty_Interrupt << USART_CR1_RXNEIE_Pos) |
								(Usart_Drv_Regs[uart_list].CR1_Idle_Interrupt << USART_CR1_IDLEIE_Pos) |
								(1 << USART_CR1_TE_Pos) |
								(1 << USART_CR1_RE_Pos);
		MODIFY_REG(Usart_Drv_Regs[uart_list].port->CR1, 0xFFFFFFFF, reg_temp);
		
		reg_temp = (Usart_Drv_Regs[uart_list].CR2_MSBPosition << USART_CR2_MSBFIRST_Pos) |
								(Usart_Drv_Regs[uart_list].CR2_BinaryDataInversion << USART_CR2_DATAINV_Pos) |
								(Usart_Drv_Regs[uart_list].CR2_TXLevelInversion << USART_CR2_TXINV_Pos) |
								(Usart_Drv_Regs[uart_list].CR2_RXLevelInversion << USART_CR2_RXINV_Pos) |
								(Usart_Drv_Regs[uart_list].CR2_Swap_TX_RX << USART_CR2_SWAP_Pos) |
								(Usart_Drv_Regs[uart_list].CR2_StopBits << USART_CR2_STOP_Pos) |
								(Usart_Drv_Regs[uart_list].CR2_Clock << USART_CR2_CLKEN_Pos) |
								(Usart_Drv_Regs[uart_list].CR2_ClockPolarity << USART_CR2_CPOL_Pos) |
								(Usart_Drv_Regs[uart_list].CR2_ClockPhase << USART_CR2_CPHA_Pos) |
								(Usart_Drv_Regs[uart_list].CR2_LastBitClockPulse << USART_CR2_LBCL_Pos);
		MODIFY_REG(Usart_Drv_Regs[uart_list].port->CR2, 0xFFFFFFFF, reg_temp);
		
		/* Set baud rate */
		result &= UsartBaudCalc(uart_list, &reg_temp);
		MODIFY_REG(Usart_Drv_Regs[uart_list].port->BRR, 0xFFFFFFFF, reg_temp);	
		
		/* Initialize final index of Obj (needed if you use usart TX on interrupt) */
		UsartObj[uart_list].tx_EndIdx = MAX_BYTE_TXBUFF - 0x1U;
		
		/* Enable Peripheral */
		SET_BIT(Usart_Drv_Regs[uart_list].port->CR1, USART_CR1_UE);
	};
		
	return result;
}


/**************************************************************************************/
/* Usart_Drv_Int */
/**************************************************************************************/
void Usart_Drv_Int (void)
{
	for (USART_LIST_t uart_list = USART_LIST_START; uart_list < USART_LIST_TOTAL; uart_list++)
	{		
		/* Configure and enable the USART2 IRQ channel in the NVIC. */
		NVIC_SetPriority (Usart_Drv_Regs[uart_list].Int_number, (Usart_Drv_Regs[uart_list].Int_priority << __NVIC_PRIO_BITS) - 1UL);
		NVIC_EnableIRQ(Usart_Drv_Regs[uart_list].Int_number);
	}
}

/**************************************************************************************/
/* UsartBaudCalc */
/**************************************************************************************/
static ErrorStatus UsartBaudCalc(USART_LIST_t uart_list, uint32_t *BaudReg)
{
	uint32_t clock;
	uint32_t usartdiv = 0U;
	uint32_t PPREx;
	
	/* clock calculation */
	switch (Usart_Drv_Regs[uart_list].DCKCFGR2_ClockSelection)
	{
		case USART_CLOCKSEL_APBx:		// APBx clock (PCLKx)
			/* Select if source is APB1 or APB2 */
			if( (0x1U == Usart_Drv_Regs[uart_list].UsartNumber) || (0x6U == Usart_Drv_Regs[uart_list].UsartNumber) )
			{
				PPREx = RCC_CFGR_PPRE2_CFG;
			}
			else
			{
				PPREx = RCC_CFGR_PPRE1_CFG;
			}
		
			switch (PPREx)
			{
				case (RCC_CFGR_PPRE1_DIV2 >> RCC_CFGR_PPRE1_Pos):
					clock = SystemCoreClock / 2U;
					break;
				case (RCC_CFGR_PPRE1_DIV4 >> RCC_CFGR_PPRE1_Pos):
					clock = SystemCoreClock / 4U;
					break;
				case (RCC_CFGR_PPRE1_DIV8 >> RCC_CFGR_PPRE1_Pos):
					clock = SystemCoreClock / 8U;
					break;
				case (RCC_CFGR_PPRE1_DIV16 >> RCC_CFGR_PPRE1_Pos):
					clock = SystemCoreClock / 16U;
					break;
				default:
					clock = SystemCoreClock;
					break;
			}
			switch (RCC_CFGR_HPRE_CFG)
			{
				case (RCC_CFGR_HPRE_DIV2 >> RCC_CFGR_HPRE_Pos):
					clock = clock / 2U;
					break;
				case (RCC_CFGR_HPRE_DIV4 >> RCC_CFGR_HPRE_Pos):
					clock = clock / 4U;
					break;
				case (RCC_CFGR_HPRE_DIV8 >> RCC_CFGR_HPRE_Pos):
					clock = clock / 8U;
					break;
				case (RCC_CFGR_HPRE_DIV16 >> RCC_CFGR_HPRE_Pos):
					clock = clock / 16U;
					break;
				case (RCC_CFGR_HPRE_DIV64 >> RCC_CFGR_HPRE_Pos):
					clock = clock / 64U;
					break;
				case (RCC_CFGR_HPRE_DIV128 >> RCC_CFGR_HPRE_Pos):
					clock = clock / 128U;
					break;
				case (RCC_CFGR_HPRE_DIV256 >> RCC_CFGR_HPRE_Pos):
					clock = clock / 256U;
					break;
				case (RCC_CFGR_HPRE_DIV512 >> RCC_CFGR_HPRE_Pos):
					clock = clock / 512U;
					break;
				default:
					clock = clock;
					break;
			}
			break;
		case USART_CLOCKSEL_SYSCLK:  // System clock
			clock = SystemCoreClock;
			break;
		case USART_CLOCKSEL_HSI:  // HSI clock
			clock = HSI_VALUE;
			break;
		case USART_CLOCKSEL_LSE:  // LSE clock
			clock = LSE_VALUE;
			break;
		default:
			break;
	}
	
	if (USART_OVERSAMPLING16 == Usart_Drv_Regs[uart_list].CR1_Oversapling)
	{
		usartdiv = clock / (Usart_Drv_Regs[uart_list].BRR_BaudRate);
		*BaudReg = usartdiv;
	}
	else
	{
		/*	– BRR[2:0] = USARTDIV[3:0] shifted 1 bit to the right.
			– BRR[3] must be kept cleared.
			– BRR[15:4] = USARTDIV[15:4] */
		usartdiv = 0x2U * clock / (Usart_Drv_Regs[uart_list].BRR_BaudRate);
		*BaudReg = ((usartdiv & 0xFU) >> 0x1U) |
							((usartdiv & 0xFFF0) << 0x4U);
	}
	
	/* USARTDIV must be greater than or equal to 0d16 */
	if ( (usartdiv <= 0x10) || (usartdiv >= 0x0000FFFF) )
	{
		return ERROR;
	}
	
	return SUCCESS;
}


/**************************************************************************************/
/* UsartData_TXpoll */
/**************************************************************************************/
ErrorStatus UsartData_TXpoll (USART_LIST_t UartInstance, const uint8_t *data, const uint16_t size)
{	
	if ( (data == NULL) || (size == 0) || (size > MAX_BYTE_TXBUFF))
	{
		return ERROR;
	}
	else
	{
		UsartObj[UartInstance].tx_CurrIdx = 0x00U;
		UsartObj[UartInstance].tx_size = size;

		/* Transmit all data */
		while (UsartObj[UartInstance].tx_CurrIdx < UsartObj[UartInstance].tx_size)
		{
			/* Wait until TX register is empty (ready to be written) */
			while (!READ_BIT(Usart_Drv_Regs[UartInstance].port->ISR, USART_ISR_TXE));
			Usart_Drv_Regs[UartInstance].port->TDR = (*data) & 0xFF;
			data++;
			UsartObj[UartInstance].tx_CurrIdx++;
		}
			/* Wait until transfer is complete (TX shadow register empty) */
		while(!READ_BIT(Usart_Drv_Regs[UartInstance].port->ISR, USART_ISR_TC));
	}
	
	return SUCCESS;
}


/**************************************************************************************/
/* UsartString_TXpoll */
/**************************************************************************************/
ErrorStatus UsartString_TXpoll (USART_LIST_t UartInstance, const char *str, ...)
{
	if (str == NULL)
	{
		return ERROR;
	}
	else
	{
		*UsartObj[UartInstance].data_ptr = NULL;
		/* Create a ponter to the not declared parameters of the function */
		va_list arg;
		/* Initialize arg so that it points to the area immediatly after the last parameter (&str += 4) */
		va_start(arg, str);
		/* compose the total string of str + additional arguments inside dbg_tx_buff */
		vsprintf((char *) UsartObj[UartInstance].tx_buff, str, arg);
		/* Release arg */
		va_end(arg);
		UsartObj[UartInstance].data_ptr = UsartObj[UartInstance].tx_buff;
		
			/* Transmit all data */
		while (*UsartObj[UartInstance].data_ptr != '\0')
		{
			/* Wait until TX register is empty (ready to be written) */
			while (!READ_BIT(Usart_Drv_Regs[UartInstance].port->ISR, USART_ISR_TXE));
			Usart_Drv_Regs[UartInstance].port->TDR = (*UsartObj[UartInstance].data_ptr);
			UsartObj[UartInstance].data_ptr++;
		}
		/* Wait until transfer is complete (TX shadow register empty) */
		while(!READ_BIT(Usart_Drv_Regs[UartInstance].port->ISR, USART_ISR_TC));
	}
	
	return SUCCESS;
}

/**************************************************************************************/
/* UsartString_RXpoll */
/**************************************************************************************/
void UsartString_RXpoll (USART_LIST_t UartInstance, uint8_t *data)
{
	/* Used in polling */
	
	if (READ_BIT(Usart_Drv_Regs[UartInstance].port->ISR, USART_ISR_RXNE))
	{
		*data = (uint8_t) (Usart_Drv_Regs[UartInstance].port->RDR & USART2_RXMASK);
	}
}

/**************************************************************************************/
/* UsartData_InitTXint */
/**************************************************************************************/
ErrorStatus UsartData_InitTXint (USART_LIST_t UartInstance, const uint8_t *data, const uint16_t size)
{
	/* Send first byte manually, then interrupt sends the rest */
	if ( (data == NULL) || (size == 0) || (size > MAX_BYTE_TXBUFF))
	{
		return ERROR;
	}
	else
	{
		UsartObj[UartInstance].tx_size = size;
		
		if (UsartObj[UartInstance].tx_size > MAX_BYTE_TXBUFF)
		{
			UsartObj[UartInstance].tx_size = MAX_BYTE_TXBUFF;
		}
		
		/* Prepare new data to be sent appending them to the previous ones */
		UsartObj[UartInstance].tx_StartIdx = (UsartObj[UartInstance].tx_EndIdx + 1) % MAX_BYTE_TXBUFF;
		UsartObj[UartInstance].tx_TempIdx = UsartObj[UartInstance].tx_StartIdx;
		/* Copy new TX data inside TX buffer */
		for (uint8_t i = 0; i < UsartObj[UartInstance].tx_size; i++)
		{
			UsartObj[UartInstance].tx_buff[UsartObj[UartInstance].tx_TempIdx] = *(data + i);
			UsartObj[UartInstance].tx_TempIdx = (UsartObj[UartInstance].tx_TempIdx + 1) % MAX_BYTE_TXBUFF;
			/* If OVERRUN detected, stop filling the buffer (remaining data are not transmitted) */
			if ((UsartObj[UartInstance].tx_TempIdx < UsartObj[UartInstance].tx_StartIdx) && (UsartObj[UartInstance].tx_TempIdx >= UsartObj[UartInstance].tx_CurrIdx))
			{
				UsartObj[UartInstance].tx_size = i;
			}
		}
		UsartObj[UartInstance].tx_EndIdx = (UsartObj[UartInstance].tx_size + UsartObj[UartInstance].tx_EndIdx) % MAX_BYTE_TXBUFF;
		
		/* If TX interrupt is disabled, enable it and send first byte */
		if (!READ_BIT(Usart_Drv_Regs[UartInstance].port->CR1, USART_CR1_TXEIE))
		{
			UsartObj[UartInstance].tx_CurrIdx = UsartObj[UartInstance].tx_StartIdx;
			CLEAR_BIT(Usart_Drv_Regs[UartInstance].port->CR1, USART_CR1_TXEIE);
			UsartData_TXpoll(UartInstance, &UsartObj[UartInstance].tx_buff[UsartObj[UartInstance].tx_StartIdx], 0x1U);
			SET_BIT(Usart_Drv_Regs[UartInstance].port->CR1, USART_CR1_TXEIE);
		}
	}
	
	return SUCCESS;
}


/**************************************************************************************/
/* UsartData_TXint */
/**************************************************************************************/
// This function must be called inside the interrupt routine
void UsartData_TXint (USART_LIST_t UartInstance)
{
	if (READ_BIT(Usart_Drv_Regs[UartInstance].port->ISR, USART_ISR_TXE)) 
	{
		/* If last byte was transmitted, stop communication */
		if (UsartObj[UartInstance].tx_CurrIdx == UsartObj[UartInstance].tx_EndIdx)
		{
			CLEAR_BIT(Usart_Drv_Regs[UartInstance].port->CR1, USART_CR1_TXEIE);
			SET_BIT(Usart_Drv_Regs[UartInstance].port->ICR, USART_ICR_TCCF);
		}
		else
		{
			/* If there are still data to be trnsmitted, send them. Note: 
			the first byte was transmitted by the user */
			UsartObj[UartInstance].tx_CurrIdx = (UsartObj[UartInstance].tx_CurrIdx + 1) % MAX_BYTE_TXBUFF;
			Usart_Drv_Regs[UartInstance].port->TDR = UsartObj[UartInstance].tx_buff[UsartObj[UartInstance].tx_CurrIdx] & 0xFF;
		}
	}
}

