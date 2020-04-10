#include "main.h"
#include "os.h"

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

	/* Tick init */
	result &= SysTick_Init();
	
	
	while(1)
	{
	}
	
	return 0;
}

	
	/* NEXT STEPS: 
	- GPIO init
	
	- LCD showing data
	- buzzer+LED with pwm (timer) sound/light when alarm clock or when ultrrasonic sensor
	- potenziometer with ADC (use DMA?)
	
	- read Ultrasonic sensors (input capture?)
	
	
	- uart COM (use HW CRC)
	- accelerometer (SPI, I2C with DMA?) (use HW CRC)
	
	
	- usb OTG ?
	- ethernet?
	- low power management
	- hdmi?
	- hash processor? */


