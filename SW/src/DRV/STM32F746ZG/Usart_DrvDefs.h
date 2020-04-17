#ifndef __USART_DRVDEFS_H
#define __USART_DRVDEFS_H

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

typedef enum {
	USART_CLOCKSEL_APBx = 0x0U,  // APB1: uart2,3,4,5,7,8   APB2: uart1,6
	USART_CLOCKSEL_SYSCLK,
	USART_CLOCKSEL_HSI,
	USART_CLOCKSEL_LSE
} USART_CLOCKSEL_t;

typedef enum {
	USART_DISABLED = 0x0U,
	USART_ENABLED
} USART_ENABLE_DISABLE_t;
typedef enum {
	USART_OVERSAMPLING16 = 0x0U,
	USART_OVERSAMPLING8
} USART_OVERSAMPLING_t;
typedef enum {
	USART_PARITYSEL_EVEN = 0x0U,
	USART_PARITYSEL_ODD
} USART_PARITYSEL_t;
typedef enum {
	USART_MSBFIRST_LSB = 0x0U,
	USART_MSBFIRST_MSB
} USART_MSBFIRST_t;
typedef enum {
	USART_DATAINV_STANDARD = 0x0U,
	USART_DATAINV_INVERTED
} USART_DATAINV_t;
typedef enum {
	USART_STOPBITS_1 = 0x0U,
	USART_STOPBITS_0_5,
	USART_STOPBITS_2,
	USART_STOPBITS_1_5
} USART_STOPBITS_t;
typedef enum {
	USART_CLOCKPOL_STEADYLOW = 0x0U,
	USART_CLOCKPOL_STEADYHIGH
} USART_CLOCKPOL_t;
typedef enum {
	USART_CLOCKPHASE_1ST = 0x0U,
	USART_CLOCKPHASE_2ND
} USART_CLOCKPHASE_t;
typedef enum {
	USART_LBCL_NO = 0x0U,
	USART_LBCL_YES
} USART_LBCL_t;
typedef enum {
	USART_BAUD_9600			= 9600,
	USART_BAUD_19200		= 19200,
	USART_BAUD_38400		= 38400,
	USART_BAUD_57600		= 57600,
	USART_BAUD_115200		= 115200,
	USART_BAUD_230400		= 230400,
	USART_BAUD_460800		= 460800,
	USART_BAUD_921600		= 921600,
	USART_BAUD_13500000	= 13500000,
	USART_BAUD_27000000	= 27000000
} USART_BAUD_t;


typedef struct
{
	uint8_t									UsartNumber;
	USART_TypeDef 					*port;
	uint8_t									RX_pin;
	IRQn_Type								Int_number;
	uint32_t								Int_priority;
	
	USART_CLOCKSEL_t				DCKCFGR2_ClockSelection;
	
	USART_ENABLE_DISABLE_t	CR1_EndOfBlock_Interrupt;
	USART_OVERSAMPLING_t		CR1_Oversapling;
	USART_ENABLE_DISABLE_t	CR1_CharMatch_Interrupt;
	USART_ENABLE_DISABLE_t	CR1_ParityEnable;
	USART_PARITYSEL_t				CR1_ParityType;
	USART_ENABLE_DISABLE_t	CR1_PE_Interrupt;
	USART_ENABLE_DISABLE_t	CR1_TXEmpty_Interrupt;
	USART_ENABLE_DISABLE_t	CR1_TXComplete_Interrupt;
	USART_ENABLE_DISABLE_t	CR1_RXNotEmpty_Interrupt;
	USART_ENABLE_DISABLE_t	CR1_Idle_Interrupt;
	
	USART_MSBFIRST_t				CR2_MSBPosition;
	USART_DATAINV_t					CR2_BinaryDataInversion;
	USART_DATAINV_t					CR2_TXLevelInversion;
	USART_DATAINV_t					CR2_RXLevelInversion;
	USART_DATAINV_t					CR2_Swap_TX_RX;
	USART_STOPBITS_t				CR2_StopBits;
	USART_ENABLE_DISABLE_t	CR2_Clock;
	USART_CLOCKPOL_t				CR2_ClockPolarity;
	USART_CLOCKPHASE_t			CR2_ClockPhase;
	USART_LBCL_t						CR2_LastBitClockPulse;
	
	USART_BAUD_t						BRR_BaudRate;
} Usart_Drv_Regs_t;


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


#endif /* __USART_DRVDEFS_H */
