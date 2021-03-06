#include "stm32f7xx.h"
#include "os.h"

#include "ChipInfo.h"
#include "Clock_Drv.h"
#include "CRC_Drv.h"
#include "Timer_Drv.h"
#include "Rtc_Drv.h"

#include "Gpio_Drv.h"
#include "Adc_Drv.h"
#include "Dma_Drv.h"
#include "Usart_Drv.h"
#include "AcquireInput.h"

#include "Sveglia.h"
#include "Display.h"
#include "SerialMonitor.h"


int main(void)
{
	ErrorStatus result = SUCCESS;
	
	/* Get chip informations */
	ChipInfo_Get();
	
	/* Clock initialization */
	result &= Clock_Drv_Init();	
		
	/* CRC initialization */
	CRC_Drv_Init();
	
	/* Timer initialization */
	Timer_Drv_Init(TIMERBASIC_7_DELAY);
	Timer_Drv_Int(TIMERBASIC_7_DELAY);

	Timer_Drv_Init(TIMERBASIC_6_ADC1TRIG);

	Timer_Drv_Init(TIMERGENERAL_14_PWM);
	
	Timer_Drv_Init(TIMERGENERAL_11_SONICTRIG);
	
	Timer_Drv_Init(TIMERGENERAL_13_SONICECHO);
	Timer_Drv_Int(TIMERGENERAL_13_SONICECHO);
	
	/* Rtc initialization */
	RTC_Drv_Init();
	
	/* Tick init */
	result &= SysTick_Init();
	
	/* Gpio Init */
	Gpio_Drv_Init();

	/* Adc Init */
	Adc_Drv_Init(ADC_LIST_1);
	
	/* Dma Init */
	Dma_Drv_Init(DMA_LIST_ADC_DMA2CH0);
	
	/* Usart Init */
	result &= Usart_Drv_Init(USART2_DBG_CMD);
	
	/* LCD Init */
	LCDACM1602B_Drv_Init();

	/* Interrupt Init */
	Rtc_Drv_AlarmInt();
	Clock_Drv_SystickInt();
	Usart_Drv_Int(USART2_DBG_CMD);
	Dma_Drv_Int(DMA_LIST_ADC_DMA2CH0);
	Adc_Drv_Int(ADC_LIST_1);
	Timer_Drv_Int(TIMERBASIC_6_ADC1TRIG);
	
	/* Start peripherals */
	Dma_Drv_Start(DMA_LIST_ADC_DMA2CH0);
	Adc_Drv_Start(ADC_LIST_1);
	Timer_Drv_Start(TIMERBASIC_6_ADC1TRIG);
	
	
	
	/*****************  TEST  **************************************/
	Gpio_Drv_SetPin(LED_BLUE_PORT, LED_BLUE_PIN, HIGH);
	
	
//	static uint64_t starttime,
//									endtime,
//									elapsedtime, elapsedtime2;
//	starttime = GetTime_us();
//	Delay_us(25000U);
//	Delay_us(25000U);
//	Delay_us(25000U);
//	Delay_us(25000U);
//	endtime = GetTime_us();
//	elapsedtime = endtime - starttime;
	
	
//	RTC_Drv_AlarmConfigure(&RTC_AlarmTime);
//	RTC_Drv_AlarmEnable();

//	static uint8_t prova[10] = {3,4,5,6,7,8,9,1,2,3};
////	UsartData_InitTXint(USART2_DBG_CMD, prova, 10);
//	UsartData_TXpoll(USART2_DBG_CMD, prova, 10);
//	UsartString_TXpoll(USART2_DBG_CMD, "Greve zi!",10);
	
	
//	static uint32_t crc_test;
//	static uint32_t data32[4] = {0x00002222, 0x00002225, 0x00002224, 0x00002223};
//	static uint16_t data16[5] = {0x2222, 0x2225, 0x2224, 0x2223, 0x2228};
//	static uint8_t data8[7] = {0x22, 0x25, 0x24, 0x23, 0x56, 0x34, 0x3A};
//	crc_test = CRC_Drv_Compute(data32, sizeof(data32), CRC_COMPUTE_TYPE_32);
//	crc_test = CRC_Drv_Compute((uint32_t *)data16, sizeof(data16), CRC_COMPUTE_TYPE_16);
//	crc_test = CRC_Drv_Compute((uint32_t *)data8, sizeof(data8), CRC_COMPUTE_TYPE_8);



	while(1)
	{
		if (ERROR == result)
		{
			Gpio_Drv_SetPin(LED_RED_PORT, LED_RED_PIN, HIGH);
		}
				
		/* AcquireInput managed in the interrupt handling of DMA (AcquireInput() function) */
				
		Acquire_SonicSensor();
				
		Sveglia_Mng();
		
		Display_Mng();
		
		SerialMonitor_Mng();
	}
	
	return 0;
}



	/* NEXT STEPS:		
	- accelerometer (SPI, I2C with DMA?)
	
	- usb OTG ?
	- ethernet?
	- low power management
	- hdmi?
	- hash processor? */


