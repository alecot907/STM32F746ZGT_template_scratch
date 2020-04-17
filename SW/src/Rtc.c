/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Rtc.h"
#include "SysData.h"
#include "CommonDefs.h"
#include <string.h>

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/* Initial values of RTC_Time and RTC_AlarmTime set to generate a Alarm interrupt
10 seconds after startup if RTC_AlarmConfigure and RTC_AlarmEnable are called
in the initialization procedure */
RTC_Time_t RTC_Time = {00, 01, 01, SATURDAY,
											 21, 00, 00};
RTC_AlarmTime_t RTC_AlarmTime = {{1, 1, 0, 		 0, 	0},
																 {01, 	21, 	00,  10}}; // day, hour, min, sec

RTC_TimeUsart_t 					RTC_TimeUsart;
RTC_AlarmTimeValueUart_t	RTC_AlarmTimeValueUart;

																 
static uint8_t button_old = 0x0U;
static uint8_t alarm_state = 0x0U;

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
																 
/**************************************************************************************/
/* Rtc_TimeDateCfg */
/**************************************************************************************/
static void Rtc_TimeDateCfg (void);

/**************************************************************************************/
/* Rtc_AlarmCfg */
/**************************************************************************************/
static void Rtc_AlarmCfg (void);

/**************************************************************************************/
/* Rtc_AlarmEnDis */
/**************************************************************************************/
static void Rtc_AlarmEnDis (void);

/**************************************************************************************/
/* Rtc_AlarmTriggered */
/**************************************************************************************/
static void Rtc_AlarmTriggered (void);


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* Rtc_Mng */
/**************************************************************************************/
void Rtc_Mng (void)
{
	Rtc_AlarmCfg();
	
	Rtc_TimeDateCfg();
	
	Rtc_AlarmEnDis();
	
	Rtc_AlarmTriggered();
}

/**************************************************************************************/
/* Rtc_AlarmEnDis */
/**************************************************************************************/
static void Rtc_AlarmEnDis (void)
{
	if ((HIGH == SysData_Der[SYS_IN_BUTTONUSR]) && (LOW == button_old))
	{
		alarm_state = !alarm_state;
	}
	button_old = SysData_Der[SYS_IN_BUTTONUSR];
	
	if (alarm_state)
	{
		RTC_Drv_AlarmEnable();
	}
	else
	{
		RTC_Drv_AlarmDisable();
	}
}

/**************************************************************************************/
/* Rtc_AlarmCfg */
/**************************************************************************************/
static void Rtc_AlarmCfg (void)
{
	if (RTC_IDS_USART_ALARM == RTC_AlarmTimeValueUart.IDS)
	{
		RTC_AlarmTimeValueUart.IDS = 0x00U;
		
		// TODO: Check CRC!!!!!
			  
		memcpy ( &RTC_AlarmTime.Value, &RTC_AlarmTimeValueUart.SetAlarmCfg.Value, sizeof(RTC_AlarmTimeValueUart.SetAlarmCfg.Value) );
				
		RTC_Drv_AlarmConfigure(&(RTC_AlarmTime));
	}
}


/**************************************************************************************/
/* Rtc_TimeDateCfg */
/**************************************************************************************/
static void Rtc_TimeDateCfg (void)
{
	if (RTC_IDS_USART_TIMEDATE == RTC_TimeUsart.IDS)
	{
		RTC_TimeUsart.IDS = 0x00U;
		
		// TODO: Check CRC!!!!!
				
		RTC_Drv_SetTime(&(RTC_TimeUsart.SetCfg));
	}
}

/**************************************************************************************/
/* Rtc_AlarmTriggered */
/**************************************************************************************/
static void Rtc_AlarmTriggered (void)
{
	/* Il RTC alarm A was triggered */
	if (READ_BIT(RTC->ISR, RTC_ISR_ALRAF) & RTC_ISR_ALRAF)
	{
		/* Clear alarm flag */
//		CLEAR_BIT(RTC->ISR, RTC_ISR_ALRAF);
//		Activate buzzer!!!
	}
}

