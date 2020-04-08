#ifndef __RTCDRV_H
#define __RTCDRV_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "stm32f7xx.h"


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
typedef enum 
{
	FORBIDDEN = 0x0,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
	SUNDAY
} RTC_DayOfWeek_t;
typedef struct 
{
	uint8_t 						Year;
	uint8_t 						Month;
	uint8_t 						Day;
	RTC_DayOfWeek_t			DayOfWeek;
	uint8_t 						Hour;
	uint8_t 						Minute;
	uint8_t 						Second;
} RTC_Time_t;


typedef struct 
{
	uint8_t 	DayOrDate;
	uint8_t 	DayOrDateSelection; // 0: date units; 	1: the week day.
	uint8_t 	Hour;
	uint8_t 	Minute;
	uint8_t 	Second;
}RTC_AlarmTimeMask_t;
typedef struct 
{
	uint8_t 	DayOrDate;
	uint8_t 	Hour;
	uint8_t 	Minute;
	uint8_t 	Second;
}RTC_AlarmTimeValue_t;
typedef struct 
{
	RTC_AlarmTimeMask_t 	Mask;
	RTC_AlarmTimeValue_t 	Value;
} RTC_AlarmTime_t;

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* RTC_Init */
/**************************************************************************************/
void RTC_Init (void);
/**************************************************************************************/
/* RTC_SettTime */
/**************************************************************************************/
void RTC_SetTime (RTC_Time_t *time);
/**************************************************************************************/
/* RTC_GetTime */
/**************************************************************************************/
void RTC_GetTime (RTC_Time_t *time);

/**************************************************************************************/
/* RTC_AlarmConfigure */
/**************************************************************************************/
void RTC_AlarmConfigure (RTC_AlarmTime_t *alarmtime);
/**************************************************************************************/
/* RTC_AlarmEnable */
/**************************************************************************************/
void RTC_AlarmEnable (void);
/**************************************************************************************/
/* RTC_AlarmDisable */
/**************************************************************************************/
void RTC_AlarmDisable (void);

/**************************************************************************************/
/* RTC_Alarm_IRQHandler */
/**************************************************************************************/
void RTC_Alarm_IRQHandler (void);

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
extern RTC_Time_t 			RTC_Time;
extern RTC_AlarmTime_t 	RTC_AlarmTime;

#endif /* __RTCDRV_H */
