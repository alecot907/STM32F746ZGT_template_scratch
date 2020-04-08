#include "main.h"

#include <stdint.h>
#include "ChipInfo.h"
#include "ClockDrv.h"
#include "RtcDrv.h"

int main(void)
{
	ErrorStatus result = SUCCESS;
	
	/* Get chip informations */
	ChipInfo_Get();

	/* Clock initialization */
	result &= ClockDrv_Init();	
	
	/* Rtc initialization */
	RTC_Init();


	
	
	uint32_t x = 0;
	while(1)
	{
		x++;
	}
	
	return 0;
}


	
	
	
	/* NEXT STEPS: 
	- set, reset data
	- set alarm (gpio??)
	- delete alarm
	
	- GPIO init
	
	- buzzer+LED with pwm (timer) sound/light when alarm clock or when ultrrasonic sensor
	- LCD showing data
	- potenziometer with ADC (use DMA?)
	
	- read Ultrasonic sensors (input capture?)
	
	
	- uart COM (use HW CRC)
	- accelerometer (SPI, I2C with DMA?) (use HW CRC)
	
	
	- usb OTG ?
	- ethernet?
	- low power management
	- hdmi?
	- hash processor? */


