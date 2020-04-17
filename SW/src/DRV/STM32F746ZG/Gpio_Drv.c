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
#include "Gpio_Drv.h"
#include "Gpio_Cfg.h"

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
/* Gpio_Drv_Init */
/**************************************************************************************/
void Gpio_Drv_Init (void)
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
	
	
	for (GPIO_LIST_t gpio_list = GPIO_LIST_START; gpio_list < GPIO_LIST_TOTAL; gpio_list++)
	{
		MODIFY_REG(Gpio_Drv_Regs[gpio_list].port->MODER, 0x03U << (Gpio_Drv_Regs[gpio_list].pin * 2U),
							Gpio_Drv_Regs[gpio_list].mode << (Gpio_Drv_Regs[gpio_list].pin * 2U));
		MODIFY_REG(Gpio_Drv_Regs[gpio_list].port->OTYPER, 0x01 << Gpio_Drv_Regs[gpio_list].pin,
							Gpio_Drv_Regs[gpio_list].out_type << Gpio_Drv_Regs[gpio_list].pin);
		MODIFY_REG(Gpio_Drv_Regs[gpio_list].port->OSPEEDR, 0x03U << (Gpio_Drv_Regs[gpio_list].pin * 2U),
							Gpio_Drv_Regs[gpio_list].speed << (Gpio_Drv_Regs[gpio_list].pin * 2U));
		MODIFY_REG(Gpio_Drv_Regs[gpio_list].port->PUPDR, 0x03U << (Gpio_Drv_Regs[gpio_list].pin * 2U),
							Gpio_Drv_Regs[gpio_list].pull_type << (Gpio_Drv_Regs[gpio_list].pin * 2U));
		
		if (Gpio_Drv_Regs[gpio_list].pin <= 0x07U)
		{
			MODIFY_REG(Gpio_Drv_Regs[gpio_list].port->AFR[0], 0x0FU << (Gpio_Drv_Regs[gpio_list].pin * 4U),
								Gpio_Drv_Regs[gpio_list].alt_func << (Gpio_Drv_Regs[gpio_list].pin * 4U));
		}
		else
		{
			MODIFY_REG(Gpio_Drv_Regs[gpio_list].port->AFR[1], 0x0FU << (Gpio_Drv_Regs[gpio_list].pin * 4U),
							Gpio_Drv_Regs[gpio_list].alt_func << (Gpio_Drv_Regs[gpio_list].pin * 4U));
		}
	}
}

/**************************************************************************************/
/* Gpio_Drv_SetPin */
/**************************************************************************************/
void Gpio_Drv_SetPin (GPIO_TypeDef *port, uint8_t pin, LOGIC_STATE_t gpio_state)
{
	if (LOW == gpio_state)
	{
		WRITE_REG(port->BSRR, 0x1U << (pin + 16U));
	}
	else
	{
		WRITE_REG(port->BSRR, (0x1U << pin));
	}
}

/**************************************************************************************/
/* Gpio_Drv_GetPin */
/**************************************************************************************/
uint8_t Gpio_Drv_GetPin (GPIO_TypeDef *port, uint8_t pin)
{
	return ((READ_BIT(port->IDR, 0x1U << pin)) >> pin);
}


