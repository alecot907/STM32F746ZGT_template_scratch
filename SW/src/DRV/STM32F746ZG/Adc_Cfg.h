#ifndef __ADC_CFG_H
#define __ADC_CFG_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Adc_DrvDefs.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

typedef enum
{
	ADC_LIST_1 = 0x0U,
	
	ADC_LIST_NOF
}ADC_LIST_t;

#define ADC_LIST_TOTAL (ADC_LIST_NOF)

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/* Pin configuration */
#define ADC_POT_PORT			((GPIO_TypeDef *) (GPIOA))
#define ADC_POT_PIN				(0x3U)
#define ADC_POT_MODE			((Gpio_Drv_Moder_t) GPIO_MODER_ANALOG)
#define ADC_POT_OUTTYPE		((Gpio_Drv_Otyper_t) GPIO_OTYPER_PUSHPULL)
#define ADC_POT_SPEEED		((Gpio_Drv_Ospeedr_t) GPIO_OSPEEDR_VERYHIGH)
#define ADC_POT_PULLTYPE	((Gpio_Drv_Popdr_t) GPIO_POPDR_NO)
#define ADC_POT_ALTFUNC		((Gpio_Drv_Afr_t) GPIO_AFR_0)

#define ADC_LIGHT_PORT			((GPIO_TypeDef *) (GPIOC))
#define ADC_LIGHT_PIN				(0x0U)
#define ADC_LIGHT_MODE			((Gpio_Drv_Moder_t) GPIO_MODER_ANALOG)
#define ADC_LIGHT_OUTTYPE		((Gpio_Drv_Otyper_t) GPIO_OTYPER_PUSHPULL)
#define ADC_LIGHT_SPEEED		((Gpio_Drv_Ospeedr_t) GPIO_OSPEEDR_VERYHIGH)
#define ADC_LIGHT_PULLTYPE	((Gpio_Drv_Popdr_t) GPIO_POPDR_NO)
#define ADC_LIGHT_ALTFUNC		((Gpio_Drv_Afr_t) GPIO_AFR_0)


/* Number of channels to be converted in scan mode */
#define ADC_REG_CH_SEQ_LENGTH_CFG (ADC_REG_CH_SEQ_LENGTH_2)


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* ADC_IRQHandler */
/**************************************************************************************/
void ADC_IRQHandler (void);


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

extern const Adc_Drv_Regs_t Adc_Drv_Regs[ADC_LIST_TOTAL];
extern const Adc_Drv_RegsCommon_t Adc_Drv_RegsCommon;

/* If DMA is used, this variable must be created */
extern volatile uint16_t ADC1_DMA_Samples[ADC_REG_CH_SEQ_LENGTH_CFG + 0x1U];


#endif /* __ADC_CFG_H */
