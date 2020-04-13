/* Registers description summary
MODE(i)		OTYPER(i) 	OSPEED(i)  	PUPD(i)		I/O configuration
[1:0] 		[1:0]					[1:0]
------------------------------------------------------------------
					0												0 	0 		GP output 	PP
					0 											0 	1 		GP output 	PP + PU
					0 											1 	0 		GP output 	PP + PD
01				0 					SPEED				1		1 		Reserved
					1						[1:0]				0		0			GP output OD
					1 											0 	1 		GP output OD + PU
					1 											1 	0 		GP output OD + PD
					1 											1 	1 		Reserved (GP output OD)
------------------------------------------------------------------
					0												0		0			AF PP
					0 											0 	1 		AF PP + PU
					0 											1 	0 		AF PP + PD
10				0						SPEED				1 	1 		Reserved
					1						[1:0]				0		0			AF OD
					1 											0 	1 		AF OD + PU
					1 											1 	0 		AF OD + PD
					1 											1 	1 		Reserved
------------------------------------------------------------------
					x 					x 	x 			0 	0 		Input Floating
00				x 					x 	x 			0 	1 		Input PU
					x 					x 	x 			1 	0 		Input PD
					x 					x 	x 			1 	1 		Reserved (input floating)
------------------------------------------------------------------
					x 					x 	x 			0 	0 		Input/output Analog
					x 					x 	x 			0 	1
11				x 					x 	x 			1 	0 		Reserved
					x 					x 	x 			1 	1
------------------------------------------------------------------
GP = general-purpose, PP = push-pull, PU = pull-up, PD = pull-down, OD = open-drain, AF = alternate
function. */


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "GpioDrv.h"
#include "UsartDrv.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* GpioDrv_Init */
/**************************************************************************************/
void GpioDrv_Init (void)
{
	/* Enable gpio clock */
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN);
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN);
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN);
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOHEN);
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOIEN);
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOJEN);
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOKEN);
	
	/* LED on the board (LD1(green):PB0  LD2(blue):PB7  LD3(red):PB14 ) */
	// General purpose output mode
	MODIFY_REG(GPIOB->MODER, (GPIO_MODER_MODER0 | GPIO_MODER_MODER7 | GPIO_MODER_MODER14),
													 (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER14_0));
	// Output push-pull
	CLEAR_BIT(GPIOB->OTYPER, (GPIO_OTYPER_OT_0 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_14));
	// no Pullup-pulldown
	CLEAR_BIT(GPIOB->PUPDR, (GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR14));


	/* Button on the board (PC13) */
	// Input mode
	CLEAR_BIT(GPIOC->MODER, GPIO_MODER_MODER13);
	// no Pullup-pulldown
	CLEAR_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPDR13);

	/* USART */
	UsartDrv_GpioInit();
}


/**************************************************************************************/
/* GpioDrv_SetPin */
/**************************************************************************************/
void GpioDrv_SetPin (GPIO_TypeDef *port, uint8_t pin, GPIO_STATE gpio_state)
{
	if (GPIO_LOW == gpio_state)
	{
		WRITE_REG(port->BSRR, 1 << (pin + 16));
	}
	else
	{
		WRITE_REG(port->BSRR, (1 << pin));
	}
}

/**************************************************************************************/
/* GpioDrv_GetPin */
/**************************************************************************************/
uint8_t GpioDrv_GetPin (GPIO_TypeDef *port, uint8_t pin)
{
	return ((READ_BIT(port->IDR, 1 << pin)) >> pin);
}


