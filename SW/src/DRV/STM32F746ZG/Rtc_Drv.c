/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Rtc_Drv.h"
#include "Rtc_Cfg.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

const char RTC_DaysString[8U][5U] = {"NAD\0", "MON\0", "TUE\0", "WED\0", "THU\0", "FRI\0", "SAT\0", "SUN\0"};
					
RTC_Time_t RTC_TimeDefault = {00U, 01U, 01U, MONDAY,
															00U, 00U, 00U};


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* RTC_Drv_ByteToBcd2 */
/**************************************************************************************/
static uint8_t RTC_Drv_ByteToBcd2(uint8_t Value);

/**************************************************************************************/
/* RTC_Drv_Bcd2ToByte */
/**************************************************************************************/
static uint8_t RTC_Drv_Bcd2ToByte(uint8_t Value);

/**************************************************************************************/
/* RTC_Drv_ProtectionEnable */
/**************************************************************************************/
static void RTC_Drv_ProtectionEnable(void);

/**************************************************************************************/
/* RTC_Drv_ProtectionDisable */
/**************************************************************************************/
static void RTC_Drv_ProtectionDisable(void);

												
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* RTC_Drv_Init */
/**************************************************************************************/
void RTC_Drv_Init (void)
{	
	/* Note: before calling this function, the source clock of RTC must be enabled and stable */
	
	/* RTC clock source */
	MODIFY_REG(RCC->BDCR, RCC_BDCR_RTCSEL, RCC_BDCR_RTCSEL_CFG << RCC_BDCR_RTCSEL_Pos);

	/* RTC peripheral */
	/* RTC clock enable */
	SET_BIT(RCC->BDCR, RCC_BDCR_RTCEN);
	
	/* Just for test */
  /* Check if the RTC was never initialized (if year == 00) */
	if(!READ_BIT(RTC->ISR, RTC_ISR_INITS))
	{		
		RTC_Drv_SetTime(&RTC_TimeDefault);
	}
}


/**************************************************************************************/
/* RTC_SetTime */
/**************************************************************************************/
void RTC_Drv_SetTime (RTC_Time_t *time)
{
	uint32_t reg_temp;

	/* Disable the RTC registers write protection */
	RTC_Drv_ProtectionDisable();
	
	/* Enter Initialization mode */
	SET_BIT(RTC->ISR, RTC_ISR_INIT);
	
	/* Wait for the confirmation of Initialization mode */
	while(!READ_BIT(RTC->ISR, RTC_ISR_INITF));
	
	/* Configure the time format (24h) */
	MODIFY_REG(RTC->CR, RTC_CR_FMT, RTC_CR_FMT_CFG);

	/* Program the prescaler values if needed */
	reg_temp = (RTC_PRER_PREDIV_S_CFG << RTC_PRER_PREDIV_S_Pos) | (RTC_PRER_PREDIV_A_CFG << RTC_PRER_PREDIV_A_Pos);
	SET_BIT(RTC->PRER, reg_temp);
	
	/* Load time and date values in the shadow registers */		
	reg_temp = 	(RTC_TR_PM_CFG << RTC_TR_PM_Pos) 	| 
							(((((uint32_t) RTC_Drv_ByteToBcd2(time->Hour)) >> 4U)) << RTC_TR_HT_Pos) |
							(((((uint32_t)RTC_Drv_ByteToBcd2(time->Hour)) & 0xFU)) << RTC_TR_HU_Pos) |
							(((((uint32_t) RTC_Drv_ByteToBcd2(time->Minute)) >> 4U)) << RTC_TR_MNT_Pos) |
							(((((uint32_t)RTC_Drv_ByteToBcd2(time->Minute)) & 0xFU)) << RTC_TR_MNU_Pos) |
							(((((uint32_t) RTC_Drv_ByteToBcd2(time->Second)) >> 4U)) << RTC_TR_ST_Pos) |
							(((((uint32_t)RTC_Drv_ByteToBcd2(time->Second)) & 0xFU)) << RTC_TR_SU_Pos);
	WRITE_REG(RTC->TR, reg_temp);
	
	
	reg_temp =	(((((uint32_t) RTC_Drv_ByteToBcd2(time->Year)) >> 4U)) << RTC_DR_YT_Pos) |
							(((((uint32_t)RTC_Drv_ByteToBcd2(time->Year)) & 0xFU)) << RTC_DR_YU_Pos) |
							(time->DayOfWeek << RTC_DR_WDU_Pos) 	| 
							(((((uint32_t) RTC_Drv_ByteToBcd2(time->Month)) >> 4U)) << RTC_DR_MT_Pos) |
							(((((uint32_t)RTC_Drv_ByteToBcd2(time->Month)) & 0xFU)) << RTC_DR_MU_Pos) |
							(((((uint32_t) RTC_Drv_ByteToBcd2(time->Day)) >> 4U)) << RTC_DR_DT_Pos) |
							(((((uint32_t)RTC_Drv_ByteToBcd2(time->Day)) & 0xFU)) << RTC_DR_DU_Pos);
	WRITE_REG(RTC->DR, reg_temp);		
	
	/* Exit Initialization mode */
	CLEAR_BIT(RTC->ISR, RTC_ISR_INIT);
		
	/* Wait the registers to be synchronised */
	CLEAR_BIT(RTC->ISR, RTC_ISR_RSF);
	while(!READ_BIT(RTC->ISR, RTC_ISR_RSF));	

	/* Enable the RTC Registers Write Protection */
	RTC_Drv_ProtectionEnable();
}

/**************************************************************************************/
/* RTC_Drv_GetTime */
/**************************************************************************************/
void RTC_Drv_GetTime (RTC_Time_t *time)
{
	/*The RSF bit is set in RTC_ISR register each time the calendar registers are copied into the
	RTC_SSR, RTC_TR and RTC_DR shadow registers. The copy is performed every
	RTCCLK cycles. To ensure consistency between the 3 values, reading either RTC_SSR or
	RTC_TR locks the values in the higher-order calendar shadow registers until RTC_DR is
	read. */
	uint32_t reg_temp;
	
	reg_temp = (uint32_t) (READ_REG(RTC->TR));
	time->Hour = 		((reg_temp & (RTC_TR_HT| RTC_TR_HU))) >> RTC_TR_HU_Pos;
	time->Minute = 	((reg_temp & (RTC_TR_MNT | RTC_TR_MNU))) >> RTC_TR_MNU_Pos;
	time->Second = 	((reg_temp & (RTC_TR_ST | RTC_TR_SU))) >> RTC_TR_SU_Pos;
	
	reg_temp = READ_REG(RTC->DR);
	time->Year = 			((reg_temp & (RTC_DR_YT | RTC_DR_YU))) >> RTC_DR_YU_Pos;
	time->DayOfWeek = (RTC_DayOfWeek_t) ((reg_temp & RTC_DR_WDU) >> RTC_DR_WDU_Pos);
	time->Month = 		((reg_temp & (RTC_DR_MT | RTC_DR_MU))) >> RTC_DR_MU_Pos;
	time->Day = 			((reg_temp & (RTC_DR_DT | RTC_DR_DU))) >> RTC_DR_DU_Pos;
	
	/* Conversion from BDC to byte */
	time->Hour = (uint8_t)RTC_Drv_Bcd2ToByte(time->Hour);
	time->Minute = (uint8_t)RTC_Drv_Bcd2ToByte(time->Minute);
	time->Second = (uint8_t)RTC_Drv_Bcd2ToByte(time->Second);
	
	time->Year = (uint8_t)RTC_Drv_Bcd2ToByte(time->Year);
	time->DayOfWeek = (RTC_DayOfWeek_t) RTC_Drv_Bcd2ToByte(time->DayOfWeek);
	time->Month = (uint8_t)RTC_Drv_Bcd2ToByte(time->Month);
	time->Day = (uint8_t)RTC_Drv_Bcd2ToByte(time->Day);
}

/**************************************************************************************/
/* Rtc_Drv_AlarmInt */
/**************************************************************************************/
void Rtc_Drv_AlarmInt (void)
{
	/* Enable RTC alarm interrupt */
//	/*Configure and enable the EXTI line corresponding to the RTC Alarm event in interrupt
//		mode and select the rising edge sensitivity. EXTI line 17 is connected to the RTC Alarm event */
	SET_BIT(EXTI->IMR, EXTI_IMR_IM17);
	SET_BIT(EXTI->RTSR, EXTI_RTSR_TR17);
	
	/* Configure and enable the RTC_ALARM IRQ channel in the NVIC. */
	NVIC_SetPriority (RTC_Alarm_IRQn, (7UL << __NVIC_PRIO_BITS) - 1UL);
	NVIC_EnableIRQ(RTC_Alarm_IRQn);
}


/**************************************************************************************/
/* RTC_AlarmConfigure */
/**************************************************************************************/
void RTC_Drv_AlarmConfigure (RTC_AlarmTime_t *alarmtime)
{
	uint32_t reg_temp;
	uint32_t  alarm_state;
	
	/* Save Alarm A state */
	alarm_state = READ_BIT(RTC->CR, RTC_CR_ALRAE);
	
	/* Disable the RTC registers write protection */
	RTC_Drv_ProtectionDisable();
	
	/* Disable alarm A */
	CLEAR_BIT(RTC->CR, RTC_CR_ALRAE);
	
	/* Check that the RTC_ALRMAR register can be accessed */
	while(!READ_BIT(RTC->ISR, RTC_ISR_ALRAWF));
	
	/* Configure the alarm */
	reg_temp = 	(((((uint32_t) RTC_Drv_ByteToBcd2(alarmtime->Value.DayOrDate)) >> 4U)) << RTC_ALRMAR_DT_Pos) |
							(((((uint32_t)RTC_Drv_ByteToBcd2(alarmtime->Value.DayOrDate)) & 0xFU)) << RTC_ALRMAR_DU_Pos) |
							(((((uint32_t) RTC_Drv_ByteToBcd2(alarmtime->Value.Hour)) >> 4U)) << RTC_ALRMAR_HT_Pos) |
							(((((uint32_t)RTC_Drv_ByteToBcd2(alarmtime->Value.Hour)) & 0xFU)) << RTC_ALRMAR_HU_Pos) |
							(((((uint32_t) RTC_Drv_ByteToBcd2(alarmtime->Value.Minute)) >> 4U)) << RTC_ALRMAR_MNT_Pos) |
							(((((uint32_t)RTC_Drv_ByteToBcd2(alarmtime->Value.Minute)) & 0xFU)) << RTC_ALRMAR_MNU_Pos) |
							(((((uint32_t) RTC_Drv_ByteToBcd2(alarmtime->Value.Second)) >> 4U)) << RTC_ALRMAR_ST_Pos) |
							(((((uint32_t)RTC_Drv_ByteToBcd2(alarmtime->Value.Second)) & 0xFU)) << RTC_ALRMAR_SU_Pos);
	reg_temp |= (RTC_TR_PM_CFG << RTC_TR_PM_Pos) 	| 
							(((uint32_t) alarmtime->Mask.DayOrDate) << RTC_ALRMAR_MSK4_Pos) |
							(((uint32_t) alarmtime->Mask.DayOrDateSelection) << RTC_ALRMAR_WDSEL_Pos) |
							(((uint32_t) alarmtime->Mask.Hour) << RTC_ALRMAR_MSK3_Pos) |
							(((uint32_t) alarmtime->Mask.Minute) << RTC_ALRMAR_MSK2_Pos) |
							(((uint32_t) alarmtime->Mask.Second) << RTC_ALRMAR_MSK1_Pos);
	WRITE_REG(RTC->ALRMAR, reg_temp);
		
	/* Reset alarm to previous state */
	MODIFY_REG(RTC->CR, RTC_CR_ALRAE, alarm_state);
		
	/* Enable the RTC registers write protection */
	RTC_Drv_ProtectionEnable();
}

/**************************************************************************************/
/* RTC_Drv_AlarmEnable */
/**************************************************************************************/
void RTC_Drv_AlarmEnable (void)
{
	/* Disable the RTC registers write protection */
	RTC_Drv_ProtectionDisable();
	
	/* Enable RTC alarm A interrupt */
	SET_BIT(RTC->CR, RTC_CR_ALRAIE);
	
	/* Enable alarm A */
	SET_BIT(RTC->CR, RTC_CR_ALRAE);

	/* Enable the RTC registers write protection */
	RTC_Drv_ProtectionEnable();
}

/**************************************************************************************/
/* RTC_Drv_AlarmDisable */
/**************************************************************************************/
void RTC_Drv_AlarmDisable (void)
{
	/* Disable the RTC registers write protection */
	RTC_Drv_ProtectionDisable();
	
	/* Disable RTC alarm A interrupt */
	CLEAR_BIT(RTC->CR, RTC_CR_ALRAIE);

	/* Disable alarm A */
	CLEAR_BIT(RTC->CR, RTC_CR_ALRAE);

	/* Enable the RTC registers write protection */
	RTC_Drv_ProtectionEnable();
}

/**************************************************************************************/
/* RTC_Drv_AlarmIsEnabled */
/**************************************************************************************/
RTC_AlarmIsEnabled_t RTC_Drv_AlarmIsEnabled (void)
{
	return ((RTC_AlarmIsEnabled_t) (READ_BIT(RTC->CR, RTC_CR_ALRAE) >> RTC_CR_ALRAE_Pos));
}

/**************************************************************************************/
/* RTC_Drv_ByteToBcd2 */
/**************************************************************************************/
static uint8_t RTC_Drv_ByteToBcd2(uint8_t Value)
{
  uint32_t bcdhigh = 0U;

  while(Value >= 10U)
  {
    bcdhigh++;
    Value -= 10U;
  }

  return  ((uint8_t)(bcdhigh << 4) | Value);
}

/**************************************************************************************/
/* RTC_Drv_Bcd2ToByte */
/**************************************************************************************/
static uint8_t RTC_Drv_Bcd2ToByte(uint8_t Value)
{
  uint32_t tmp = 0U;
  tmp = ((uint8_t)(Value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
  return (tmp + (Value & (uint8_t)0x0F));
}

/**************************************************************************************/
/* RTC_Drv_ProtectionDisable */
/**************************************************************************************/
static void RTC_Drv_ProtectionDisable(void)
{
	/* Disable the RTC registers write protection */
		WRITE_REG(RTC->WPR, 0xCAU);
		WRITE_REG(RTC->WPR, 0x53U);
}

/**************************************************************************************/
/* RTC_Drv_ProtectionEnable */
/**************************************************************************************/
static void RTC_Drv_ProtectionEnable(void)
{
		/* Enable the RTC Registers Write Protection */
		WRITE_REG(RTC->WPR, 0xFFU);
}

/**************************************************************************************/
/* RTC_Alarm_IRQHandler */
/**************************************************************************************/
void RTC_Alarm_IRQHandler (void)
{
	/* Clear EXTI interrupt flag */
	SET_BIT(EXTI->PR, EXTI_PR_PR17);

	/* Il RTC alarm A was triggered */
	if (READ_BIT(RTC->ISR, RTC_ISR_ALRAF) & RTC_ISR_ALRAF)
	{
	}
}


