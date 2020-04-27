#ifndef __USART_CFG_H
#define __USART_CFG_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include <stdint.h>
#include "Usart_DrvDefs.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#define MAX_BYTE_TXBUFF							(128U)  // maximum bytes to TX in the buffers

/* Pin configuration */
#define UART2_TX_PORT			((GPIO_TypeDef *) (GPIOD))
#define UART2_TX_PIN			(0x5U)
#define UART2_TX_MODE			((Gpio_Drv_Moder_t) GPIO_MODER_ALTFUNC)
#define UART2_TX_OUTTYPE	((Gpio_Drv_Otyper_t) GPIO_OTYPER_PUSHPULL)
#define UART2_TX_SPEEED		((Gpio_Drv_Ospeedr_t) GPIO_OSPEEDR_VERYHIGH)
#define UART2_TX_PULLTYPE	((Gpio_Drv_Popdr_t) GPIO_POPDR_UP)
#define UART2_TX_ALTFUNC	((Gpio_Drv_Afr_t) GPIO_AFR_7)

#define UART2_RX_PORT			((GPIO_TypeDef *) (GPIOD))
#define UART2_RX_PIN			(0x6U)
#define UART2_RX_MODE			((Gpio_Drv_Moder_t) GPIO_MODER_ALTFUNC)
#define UART2_RX_OUTTYPE	((Gpio_Drv_Otyper_t) GPIO_OTYPER_PUSHPULL)
#define UART2_RX_SPEEED		((Gpio_Drv_Ospeedr_t) GPIO_OSPEEDR_VERYHIGH)
#define UART2_RX_PULLTYPE	((Gpio_Drv_Popdr_t) GPIO_POPDR_NO)
#define UART2_RX_ALTFUNC	((Gpio_Drv_Afr_t) GPIO_AFR_7)


#define USART2_PARITYENABLE_CFG		(USART_DISABLED)  // 0: Parity control disabled		1: Parity control enabled
#if (USART2_PARITYENABLE_CFG == USART_DISABLED)
	#define USART2_RXMASK						(0xFFU)
#else
	#define USART2_RXMASK						(0x7FU)
#endif


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

typedef enum
{
	USART2_DBG_CMD = 0x0U,
	
	USART_NOF
}USART_LIST_t;

#define USART_LIST_TOTAL (USART_NOF)

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* USART2_IRQHandler */
/**************************************************************************************/
void USART2_IRQHandler (void);

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
extern const Usart_Drv_Regs_t Usart_Drv_Regs[USART_LIST_TOTAL];


#endif /* __USART_CFG_H */
