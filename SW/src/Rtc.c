/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Rtc.h"
#include "AcquireInput.h"
#include "CommonDefs.h"
#include "Timer_Drv.h"
#include "CRC_Drv.h"

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
RTC_Time_t 			RTC_Time = {00, 01, SATURDAY, 01, 
														21, 00, 00};
RTC_AlarmTime_t RTC_AlarmTime = {{1, 1, 0, 		 0, 	0},
																 {21, 	00}}; // day, hour, min, sec
																 
RTC_TimeUsartUnion_t 						RTC_TimeUsart;
RTC_AlarmTimeValueUsartUnion_t	RTC_AlarmTimeValueUart;

										

static uint8_t button_old = 0x0U;
static LOGIC_STATE_t alarm_state = LOW, alarm_state_old = LOW;

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
static void Rtc_AlarmMng (void);



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
	
	Rtc_AlarmMng();
}

/**************************************************************************************/
/* Rtc_AlarmEnDis */
/**************************************************************************************/
static void Rtc_AlarmMng (void)
{
	/* Button check */
	if ((HIGH == INPUT_QUAL(SysData_In[SYS_IN_BUTTONUSR])) && (LOW == button_old))
	{
		alarm_state = (LOGIC_STATE_t) (!alarm_state);
	}
	button_old = INPUT_QUAL(SysData_In[SYS_IN_BUTTONUSR]);
	
	
	/* Alarm state check */
	if ((HIGH == alarm_state) && (LOW == alarm_state_old))
	{
		RTC_Drv_AlarmEnable();
	}
	else if ((LOW == alarm_state) && (HIGH == alarm_state_old))
	{
		if (READ_BIT(RTC->ISR, RTC_ISR_ALRAF) & RTC_ISR_ALRAF)
		{
			/* Clear alarm flag */
			CLEAR_BIT(RTC->ISR, RTC_ISR_ALRAF);
			/* Activate alarm speaker */
			Timer_Drv_Stop(TIMEGENERAL_14_PWM);
			Timer_Drv_PwmSetDuty(TIMEGENERAL_14_PWM, 0U);
		}

		RTC_Drv_AlarmDisable();
	}
	alarm_state_old = alarm_state;
	
	
	/* Il RTC alarm A was triggered */
	if (READ_BIT(RTC->ISR, RTC_ISR_ALRAF) & RTC_ISR_ALRAF)
	{		
		/* Activate alarm speaker */
		Timer_Drv_Start(TIMEGENERAL_14_PWM);
		Timer_Drv_PwmSetDuty(TIMEGENERAL_14_PWM, 500U);
	}
}

/**************************************************************************************/
/* Rtc_AlarmCfg */
/**************************************************************************************/
static void Rtc_AlarmCfg (void)
{
	if (RX_PACKET_ID_RTC_ALARM == RTC_AlarmTimeValueUart.structure.ID)
	{
		uint32_t crc_computed;
		
		crc_computed = CRC_Drv_Compute((uint32_t *)RTC_AlarmTimeValueUart.bytes, sizeof(RTC_AlarmTimeValueUart.bytes), CRC_COMPUTE_TYPE_8);
		
		if (crc_computed == RTC_AlarmTimeValueUart.structure.CRC32)
		{
			memcpy ( &RTC_AlarmTime.Value, &RTC_AlarmTimeValueUart.structure.SetAlarmCfg, sizeof(RTC_AlarmTimeValueUart.structure.SetAlarmCfg) );
			
			RTC_Drv_AlarmConfigure(&(RTC_AlarmTime));
		}
		
		RTC_AlarmTimeValueUart.structure.ID = RX_PACKET_ID_DEF_DEFAULT;
	}
}


/**************************************************************************************/
/* Rtc_TimeDateCfg */
/**************************************************************************************/
static void Rtc_TimeDateCfg (void)
{
	if (RX_PACKET_ID_RTC_TIMEDATE == RTC_TimeUsart.structure.ID)
	{
		uint32_t crc_computed;
		
		crc_computed = CRC_Drv_Compute((uint32_t *)RTC_TimeUsart.bytes, sizeof(RTC_TimeUsart.bytes), CRC_COMPUTE_TYPE_8);

		if (crc_computed == RTC_TimeUsart.structure.CRC32)
		{
			RTC_Drv_SetTime(&(RTC_TimeUsart.structure.SetCfg));
		}
		
		RTC_TimeUsart.structure.ID = RX_PACKET_ID_DEF_DEFAULT;
	}
}

