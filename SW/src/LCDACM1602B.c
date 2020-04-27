/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "LCDACM1602B_Cfg.h"
#include "LCDACM1602B_Drv.h"

#include "Rtc.h"
#include "Clock_Drv.h"
#include "Usart_Drv.h"
#include "SysData.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

typedef enum
{
	LCDACM1602B_STATE_WAIT = 0U,
	LCDACM1602B_STATE_DO
} LCDACM1602B_STATE_MNG_t;

struct LCDACM1602B_OBJ_t
{
	LCDACM1602B_STATE_MNG_t state;
	uint32_t 								start_time;
};

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

static struct LCDACM1602B_OBJ_t LCDACM1602B_Obj = 
{
	LCDACM1602B_STATE_WAIT,
	0U
};

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* LCDACM1602B_Mng */
/**************************************************************************************/
void LCDACM1602B_Mng (void)
{
	switch (LCDACM1602B_Obj.state)
	{
		case LCDACM1602B_STATE_WAIT:
			LCDACM1602B_Obj.start_time = sys_ticks;
			LCDACM1602B_Obj.state = LCDACM1602B_STATE_DO;
			break;
		case LCDACM1602B_STATE_DO:
			if ((sys_ticks - LCDACM1602B_Obj.start_time) > 300U)
			{
				char Alarm_char;
				char *DayOfWeek;
				
				RTC_Drv_GetTime(&RTC_Time);
				
				Alarm_char = RTC_Drv_AlarmIsEnabled() ? '@' : ' ';
				DayOfWeek = (char *) &(RTC_DaysString[RTC_Time.DayOfWeek][0]);
				
				LCDACM1602B_Drv_SendString(ROW_1, 0, "%s", DayOfWeek);
				LCDACM1602B_Drv_SendString(ROW_1, 4, "%02d:%02d:%02d", RTC_Time.Hour, RTC_Time.Minute, RTC_Time.Second);
				LCDACM1602B_Drv_SendString(ROW_2, 0, "20%02d%02d%02d  ", RTC_Time.Year, RTC_Time.Month, RTC_Time.Day);
				LCDACM1602B_Drv_SendString(ROW_2, 10, "%c%02d:%02d", Alarm_char, RTC_AlarmTime.Value.Hour, RTC_AlarmTime.Value.Minute);
				
				LCDACM1602B_Obj.state = LCDACM1602B_STATE_WAIT;
				
				
				UsartString_TXpoll(USART2_DBG_CMD, "Alarm freq: %d\n", (uint16_t) SysData_Eng[SYS_ENG_POTENTIOMETER_FREQ]);
			}
			break;
		default:
			break;
	}
}
