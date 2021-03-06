/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Adc_Cfg.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

const Adc_Drv_Regs_t Adc_Drv_Regs[ADC_LIST_TOTAL] = 
{
	{
		ADC1,
		
		ADC_DISABLED,										// Overrun interrupt enable
		ADC_RESOLUTION12,
		ADC_DISCONT_CHANNELS1,
		ADC_DISABLED,
		ADC_ENABLED,										// CR1_ScanMode
		ADC_DISABLED,										// CR1_EndOfConv_Interrupt

		ADC_EXT_TRIGGER_DISABLED,				// ADC_EXT_TRIGGER_RIS_EDGE
		ADC_EXT_TRIGGER_SEL_TIM6TRGO,
		ADC_DATA_ALIGNMENT_RIGHT,
		ADC_EOC_SEL_EOSEQ,   						// ADC_EOC_SEL_EOSINGLE    ADC_EOC_SEL_EOSEQ
		ADC_DMA_DISABLE_SEL_YES,
		ADC_ENABLED,										// CR2_Dma
		ADC_CONV_MODE_SINGLE,  						// ADC_CONV_MODE_CONT    ADC_CONV_MODE_SINGLE
		
		ADC_SAMPL_TIME_CYCLES_480,
		
		ADC_REG_CH_SEQ_LENGTH_CFG,
		ADC_CHAN_NUMB_SEQ_0,
		ADC_CHAN_NUMB_SEQ_0,
		ADC_CHAN_NUMB_SEQ_0,
		ADC_CHAN_NUMB_SEQ_0,
		ADC_CHAN_NUMB_SEQ_0,
		ADC_CHAN_NUMB_SEQ_0,
		ADC_CHAN_NUMB_SEQ_0,
		ADC_CHAN_NUMB_SEQ_0,
		ADC_CHAN_NUMB_SEQ_0,
		ADC_CHAN_NUMB_SEQ_0,
		ADC_CHAN_NUMB_SEQ_0,
		ADC_CHAN_NUMB_SEQ_0,
		ADC_CHAN_NUMB_SEQ_0,
		ADC_CHAN_NUMB_SEQ_0,
		ADC_CHAN_NUMB_SEQ_10,
		ADC_CHAN_NUMB_SEQ_3
	}
};

const Adc_Drv_RegsCommon_t Adc_Drv_RegsCommon = 
{
	ADC_DISABLED,
	ADC_DISABLED,
	ADC_PRESCALER_DIV2
};


/* If DMA is used, this variable must be created */
volatile uint16_t ADC1_DMA_Samples[ADC_REG_CH_SEQ_LENGTH_CFG + 0x1U] = {0};


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/**************************************************************************************/
/* ADC_IRQHandler */
/**************************************************************************************/
void ADC_IRQHandler (void)
{
	/* ADC1 End of conversion management */
	if (READ_REG(ADC1->SR) & ADC_SR_EOC)
	{
		READ_REG(ADC1->DR);
	}
	
//	static uint8_t led = 0;
//	led = !led;
//	Gpio_Drv_SetPin(LED_GREEN_PORT, LED_GREEN_PIN, led);	
}
