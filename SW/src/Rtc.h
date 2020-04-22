#ifndef __RTC_H
#define __RTC_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "stm32f7xx.h"
#include "Rtc_Drv.h"
#include "Usart.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

typedef struct 
{
	RX_PACKET_ID_t 		ID;
	uint8_t 					Length;
	RTC_Time_t 				SetCfg;
	uint32_t 					CRC32;
}__attribute__ ((__packed__)) RTC_TimeUsart_t;
typedef union
{
	RTC_TimeUsart_t 	structure;
	uint8_t 					bytes[9U];
}__attribute__ ((__packed__)) RTC_TimeUsartUnion_t;

typedef struct 
{
	RX_PACKET_ID_t				ID;
	uint8_t 							Length;
	RTC_AlarmTimeValue_t 	SetAlarmCfg;
	uint32_t 							CRC32;
}__attribute__ ((__packed__)) RTC_AlarmTimeValueUsart_t;
typedef union
{
	RTC_AlarmTimeValueUsart_t 	structure;
	uint8_t 										bytes[5U];
}__attribute__ ((__packed__)) RTC_AlarmTimeValueUsartUnion_t;


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

extern RTC_Time_t 								RTC_Time;
extern RTC_AlarmTime_t 						RTC_AlarmTime; // day, hour, min, sec
//extern RTC_TimeUsart_t 						RTC_TimeUsart;
//extern RTC_AlarmTimeValueUsart_t	RTC_AlarmTimeValueUart;
extern RTC_TimeUsartUnion_t 					RTC_TimeUsart;
extern RTC_AlarmTimeValueUsartUnion_t	RTC_AlarmTimeValueUart;



#endif /* __RTC_H */
