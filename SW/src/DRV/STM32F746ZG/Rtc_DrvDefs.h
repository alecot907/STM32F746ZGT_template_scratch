#ifndef __RTC_DRVDEFS_H
#define __RTC_DRVDEFS_H

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


typedef enum 
{
	ALARM_DISABLED = 0x0U,
	ALARM_ENABLED
} RTC_AlarmIsEnabled_t;
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
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


#endif /* __RTC_DRVDEFS_H */
