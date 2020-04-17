#ifndef __GPIO_DRVDEFS_H
#define __GPIO_DRVDEFS_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "stm32f7xx.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

typedef enum
{
	GPIO_MODER_INPUT = 0x0U,
	GPIO_MODER_OUTPUT,
	GPIO_MODER_ALTFUNC,
	GPIO_MODER_ANALOG
}Gpio_Drv_Moder_t;
typedef enum
{
	GPIO_OTYPER_PUSHPULL = 0x0U,
	GPIO_OTYPER_OPENDRAIN
}Gpio_Drv_Otyper_t;
typedef enum
{
	GPIO_OSPEEDR_LOW = 0x0U,
	GPIO_OSPEEDR_MEDIUM,
	GPIO_OSPEEDR_HIGH,
	GPIO_OSPEEDR_VERYHIGH
}Gpio_Drv_Ospeedr_t;
typedef enum
{
	GPIO_POPDR_NO = 0x0U,
	GPIO_POPDR_UP,
	GPIO_POPDR_DOWN,
	GPIO_POPDR_RESERVED
}Gpio_Drv_Popdr_t;
typedef enum
{
	GPIO_AFR_0 = 0x0U,
	GPIO_AFR_1,
	GPIO_AFR_2,
	GPIO_AFR_3,
	GPIO_AFR_4,
	GPIO_AFR_5,
	GPIO_AFR_6,
	GPIO_AFR_7,
	GPIO_AFR_8,
	GPIO_AFR_9,
	GPIO_AFR_10,
	GPIO_AFR_11,
	GPIO_AFR_12,
	GPIO_AFR_13,
	GPIO_AFR_14,
	GPIO_AFR_15
} Gpio_Drv_Afr_t;

typedef struct
{
	GPIO_TypeDef 				*port;
	uint8_t							pin;
	Gpio_Drv_Moder_t 		mode;
	Gpio_Drv_Otyper_t 	out_type;
	Gpio_Drv_Ospeedr_t 	speed;
	Gpio_Drv_Popdr_t 		pull_type;
	Gpio_Drv_Afr_t 			alt_func;
} Gpio_Drv_Regs_t;

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


#endif /* __GPIO_DRVDEFS_H */
