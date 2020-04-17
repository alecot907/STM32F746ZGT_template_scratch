#include "stm32f7xx.h"
#include "os.h"

#include "ChipInfo.h"
#include "Clock_Drv.h"
#include "Rtc_Drv.h"

#include "Gpio_Drv.h"
#include "Gpio_Cfg.h"
#include "Usart_Drv.h"
#include "Interrupt_Drv.h"
#include "AcquireInput.h"

#include "LCDACM1602B.h"
#include "Rtc.h"


int main(void)
{
	ErrorStatus result = SUCCESS;
	
	/* Get chip informations */
	ChipInfo_Get();
	
	/* Clock initialization */
	result &= Clock_Drv_Init();	
	
	/* Rtc initialization */
	RTC_Drv_Init();
	
	/* Tick init */
	result &= SysTick_Init();
	
	/* Gpio Init */
	Gpio_Drv_Init();
	
	/* Usart Init */
	result &= Usart_Drv_Init();
	
	/* LCD Init */
	LCDACM1602B_Drv_Init();

	/* Interrupt Init */
	Interrupt_Drv_Init();
	
	
	
	
	/*****************  TEST  **************************************/
	MODIFY_REG(GPIOB->MODER, 0x03U << (8 * 2U), 0x01U << (8 * 2U));
	CLEAR_BIT(GPIOB->OTYPER, 0x01 << 8);
	CLEAR_BIT(GPIOB->PUPDR, 0x03U << (8 * 2U));
	
	Gpio_Drv_SetPin(GPIOB, 8, HIGH);
	Gpio_Drv_SetPin(GPIOB, 8, LOW);

	RTC_Drv_AlarmConfigure(&RTC_AlarmTime);
	RTC_Drv_AlarmEnable();

	Gpio_Drv_SetPin(LED_BLUE_PORT, LED_BLUE_PIN, HIGH);
	Gpio_Drv_SetPin(LED_RED_PORT, LED_RED_PIN, HIGH);
	Gpio_Drv_SetPin(LED_GREEN_PORT, LED_GREEN_PIN, HIGH);
	
	
	static uint8_t prova[10] = {3,4,5,6,7,8,9,1,2,3};
//	UsartData_InitTXint(USART2_DBG_CMD, prova, 10);
	UsartData_TXpoll(USART2_DBG_CMD, prova, 10);
	UsartString_TXpoll(USART2_DBG_CMD, "Greve zi!",10);
	
	while(1)
	{
		if (READ_BIT(RTC->ISR, RTC_ISR_ALRAF) & RTC_ISR_ALRAF)
		{
			Gpio_Drv_SetPin(GPIOB, 8, HIGH);
			Gpio_Drv_SetPin(LED_RED_PORT, LED_RED_PIN, HIGH);
			Delay_ms(1);
			Gpio_Drv_SetPin(GPIOB, 8, LOW);
			Gpio_Drv_SetPin(LED_RED_PORT, LED_RED_PIN, LOW);
			Delay_ms(1);
		}

		AcquireInput();
		
		Rtc_Mng();
		
		LCDACM1602B_Mng();
	}
	
	return 0;
}



	/* NEXT STEPS:

	- Basic timer for delay in us
	- pwm timer per buzzer

	- configure a interrupt timer
	- uart2 using HW CRC and DMA

	- buzzer+LED with pwm (timer) sound/light when alarm clock or when ultrrasonic sensor
	- potenziometer with ADC (use DMA?)
	
	- read Ultrasonic sensors (input capture?)
	
	- accelerometer (SPI, I2C with DMA?) (use HW CRC)
	
	- usb OTG ?
	- ethernet?
	- low power management
	- hdmi?
	- hash processor? */
