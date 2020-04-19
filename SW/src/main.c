#include "stm32f7xx.h"
#include "os.h"

#include "ChipInfo.h"
#include "Clock_Drv.h"
#include "Timer_Drv.h"
#include "Timer_Cfg.h"
#include "Rtc_Drv.h"

#include "Gpio_Drv.h"
#include "Gpio_Cfg.h"
#include "Usart_Drv.h"
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
	
	/* Timer initialization */
	Timer_Drv_Init(TIMERBASIC_7);
	Timer_Drv_Int(TIMERBASIC_7);
	
	/* Rtc initialization */
	RTC_Drv_Init();
	
	/* Tick init */
	result &= SysTick_Init();
	
	/* Gpio Init */
	Gpio_Drv_Init();
	
	/* Usart Init */
	result &= Usart_Drv_Init(USART2_DBG_CMD);
	
	/* LCD Init */
	LCDACM1602B_Drv_Init();

	/* PWM init */
	Timer_Drv_Init(TIMEGENERAL_PWM_14);

	/* Interrupt Init */
	Rtc_Drv_AlarmInt();
	Clock_Drv_SystickInt();
	Usart_Drv_Int(USART2_DBG_CMD);

	
	
	/*****************  TEST  **************************************/
	Gpio_Drv_SetPin(LED_BLUE_PORT, LED_BLUE_PIN, HIGH);
	Gpio_Drv_SetPin(LED_RED_PORT, LED_RED_PIN, HIGH);
	Gpio_Drv_SetPin(LED_GREEN_PORT, LED_GREEN_PIN, HIGH);
	
//	RTC_Drv_AlarmConfigure(&RTC_AlarmTime);
//	RTC_Drv_AlarmEnable();

//	static uint8_t prova[10] = {3,4,5,6,7,8,9,1,2,3};
////	UsartData_InitTXint(USART2_DBG_CMD, prova, 10);
//	UsartData_TXpoll(USART2_DBG_CMD, prova, 10);
//	UsartString_TXpoll(USART2_DBG_CMD, "Greve zi!",10);
	
	while(1)
	{	
//		static uint16_t duty = 500U;
//		Timer_Drv_PwmSetDuty(TIMEGENERAL_PWM_14, duty);

		AcquireInput();
		
		Rtc_Mng();
		
		LCDACM1602B_Mng();
	}
	
	return 0;
}



	/* NEXT STEPS:
	- uart2 using HW CRC and DMA

	- potenziometer with ADC to tune the freqiuency of the alarm (use DMA?)
	
	- read Ultrasonic sensors (input capture?)
	
	- accelerometer (SPI, I2C with DMA?) (use HW CRC)
	
	- usb OTG ?
	- ethernet?
	- low power management
	- hdmi?
	- hash processor? */
