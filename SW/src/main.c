#include "main.h"
#include "os.h"
#include <string.h>
#include <stdint.h>

#include "ChipInfo.h"
#include "ClockDrv.h"
#include "RtcDrv.h"
#include "GpioDrv.h"
#include "UsartDrv.h"
#include "InterruptDrv.h"


int main(void)
{
	ErrorStatus result = SUCCESS;
	
	/* Get chip informations */
	ChipInfo_Get();
	
	/* Clock initialization */
	result &= ClockDrv_Init();	
	
	/* Rtc initialization */
	RTC_Init();
	
	/* Tick init */
	result &= SysTick_Init();
	
	/* Gpio Init */
	GpioDrv_Init();
	
	/* Usart Init */
	result &= UsartDrv_Init(USART_BAUD_9600);
	
	/* Interrupt Init */
	InterruptDrv_Init();
	
	
	/*****************  TEST  **************************************/
	RTC_AlarmConfigure(&RTC_AlarmTime);
	RTC_AlarmEnable();

	GpioDrv_SetPin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_HIGH);
	GpioDrv_SetPin(LED_RED_PORT, LED_RED_PIN, GPIO_HIGH);
	GpioDrv_SetPin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_HIGH);
	
	static uint8_t button;
	while(1)
	{		
		button = GpioDrv_GetPin(BUTTON_USER_PORT, BUTTON_USER_PIN);
	}
	
	return 0;
}

	
	/* NEXT STEPS: 

	- usart2 using also TX interrupt


	- LCD showing data
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


