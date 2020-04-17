#ifndef __RTC_H
#define __RTC_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "stm32f7xx.h"
#include "Rtc_Drv.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
typedef struct 
{
	uint8_t 		IDS;
	RTC_Time_t 	SetCfg;
	uint8_t 		CRC8;
} RTC_TimeUsart_t;

typedef struct 
{
	uint8_t 					IDS;
	RTC_AlarmTime_t 	SetAlarmCfg;
	uint8_t 					CRC8;
} RTC_AlarmTimeValueUart_t;

typedef enum
{
	RTC_IDS_USART_TIMEDATE 	= 0x01U,
	RTC_IDS_USART_ALARM
	
} RTC_IDS_USART_t;


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* Rtc_Mng */
/**************************************************************************************/
void Rtc_Mng (void);


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

extern RTC_Time_t 							RTC_Time;
extern RTC_AlarmTime_t 					RTC_AlarmTime; // day, hour, min, sec
extern RTC_TimeUsart_t 					RTC_TimeUsart;
extern RTC_AlarmTimeValueUart_t	RTC_AlarmTimeValueUart;

#endif /* __RTC_H */
