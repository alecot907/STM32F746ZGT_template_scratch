/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "RtcDrv.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#define RTC_TR_PM_CFG		(1 << RTC_TR_PM_Pos)  //0: AM or 24-hour format    1: PM

#define RTC_CR_FMT_CFG					(1)  /* 0:12h format   1:24h format */
#define RCC_BDCR_RTCSEL_CFG			(RCC_BDCR_RTCSEL_0) /* LSE oscillator clock used as the RTC clock */
#define RTC_PRER_PREDIV_S_CFG		(255)
#define RTC_PRER_PREDIV_A_CFG		(127)

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/* Initial values of RTC_Time and RTC_AlarmTime set to generate a Alarm interrupt
10 seconds after startup if RTC_AlarmConfigure and RTC_AlarmEnable are called
in the initialization procedure */
RTC_Time_t RTC_Time = {00, 01, 01, SATURDAY,
											 21, 00, 00};
RTC_AlarmTime_t RTC_AlarmTime = {{0, 0, 0, 		 0, 	0},
																 {01, 	21, 	00,  10}}; // day, hour, min, sec
	

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* RTC_ByteToBcd2 */
/**************************************************************************************/
static uint8_t RTC_ByteToBcd2(uint8_t Value);

/**************************************************************************************/
/* RTC_Bcd2ToByte */
/**************************************************************************************/
static uint8_t RTC_Bcd2ToByte(uint8_t Value);

/**************************************************************************************/
/* RTC_ProtectionEnable */
/**************************************************************************************/
static void RTC_ProtectionEnable(void);

/**************************************************************************************/
/* RTC_ProtectionDisable */
/**************************************************************************************/
static void RTC_ProtectionDisable(void);

/**************************************************************************************/
/* RTC_AlarmConfigure */
/**************************************************************************************/
static void RTC_AlarmInit (void);

												
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* RTC_Init */
/**************************************************************************************/
void RTC_Init (void)
{	
	/* Note: before calling this function, the source clock of RTC must be enabled and stable */
	
	/* RTC clock source */
	MODIFY_REG(RCC->BDCR, RCC_BDCR_RTCSEL, RCC_BDCR_RTCSEL_CFG);

	/* RTC peripheral */
	/* RTC clock enable */
	SET_BIT(RCC->BDCR, RCC_BDCR_RTCEN);
	
  /* Check if the RTC was never initialized (if year == 00) */
	if(!READ_BIT(RTC->ISR, RTC_ISR_INITS))
	{		
		RTC_SetTime(&RTC_Time);
	}
	
	RTC_AlarmInit();

	RTC_GetTime(&RTC_Time);	
}


/**************************************************************************************/
/* RTC_SetTime */
/**************************************************************************************/
void RTC_SetTime (RTC_Time_t *time)
{
	uint32_t reg_temp;

	/* Disable the RTC registers write protection */
	RTC_ProtectionDisable();
	
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
	reg_temp = 	RTC_TR_PM_CFG 	| 
							(((((uint32_t) RTC_ByteToBcd2(time->Hour)) >> 4)) << RTC_TR_HT_Pos) |
							(((((uint32_t)RTC_ByteToBcd2(time->Hour)) & 0xF)) << RTC_TR_HU_Pos) |
							(((((uint32_t) RTC_ByteToBcd2(time->Minute)) >> 4)) << RTC_TR_MNT_Pos) |
							(((((uint32_t)RTC_ByteToBcd2(time->Minute)) & 0xF)) << RTC_TR_MNU_Pos) |
							(((((uint32_t) RTC_ByteToBcd2(time->Second)) >> 4)) << RTC_TR_ST_Pos) |
							(((((uint32_t)RTC_ByteToBcd2(time->Second)) & 0xF)) << RTC_TR_SU_Pos);
	WRITE_REG(RTC->TR, reg_temp);
	
	
	reg_temp =	(((((uint32_t) RTC_ByteToBcd2(time->Year)) >> 4)) << RTC_DR_YT_Pos) |
							(((((uint32_t)RTC_ByteToBcd2(time->Year)) & 0xF)) << RTC_DR_YU_Pos) |
							(time->DayOfWeek << RTC_DR_WDU_Pos) 	| 
							(((((uint32_t) RTC_ByteToBcd2(time->Month)) >> 4)) << RTC_DR_MT_Pos) |
							(((((uint32_t)RTC_ByteToBcd2(time->Month)) & 0xF)) << RTC_DR_MU_Pos) |
							(((((uint32_t) RTC_ByteToBcd2(time->Day)) >> 4)) << RTC_DR_DT_Pos) |
							(((((uint32_t)RTC_ByteToBcd2(time->Day)) & 0xF)) << RTC_DR_DU_Pos);
	WRITE_REG(RTC->DR, reg_temp);		
	
	/* Exit Initialization mode */
	CLEAR_BIT(RTC->ISR, RTC_ISR_INIT);
		
	/* Wait the registers to be synchronised */
	CLEAR_BIT(RTC->ISR, RTC_ISR_RSF);
	while(!READ_BIT(RTC->ISR, RTC_ISR_RSF));	

	/* Enable the RTC Registers Write Protection */
	RTC_ProtectionEnable();
}

/**************************************************************************************/
/* RTC_GetTime */
/**************************************************************************************/
void RTC_GetTime (RTC_Time_t *time)
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
	time->Hour = (uint8_t)RTC_Bcd2ToByte(time->Hour);
	time->Minute = (uint8_t)RTC_Bcd2ToByte(time->Minute);
	time->Second = (uint8_t)RTC_Bcd2ToByte(time->Second);
	
	time->Year = (uint8_t)RTC_Bcd2ToByte(time->Year);
	time->DayOfWeek = (RTC_DayOfWeek_t) RTC_Bcd2ToByte(time->DayOfWeek);
	time->Month = (uint8_t)RTC_Bcd2ToByte(time->Month);
	time->Day = (uint8_t)RTC_Bcd2ToByte(time->Day);
}

/**************************************************************************************/
/* RTC_AlarmConfigure */
/**************************************************************************************/
static void RTC_AlarmInit (void)
{
	/* Enable RTC alarm interrupt */
	/*Configure and enable the EXTI line corresponding to the RTC Alarm event in interrupt
		mode and select the rising edge sensitivity. EXTI line 17 is connected to the RTC Alarm event */
	SET_BIT(EXTI->IMR, EXTI_IMR_IM17);
	SET_BIT(EXTI->RTSR, EXTI_RTSR_TR17);
	
	/* Configure and enable the RTC_ALARM IRQ channel in the NVIC. */
	NVIC_SetPriority (RTC_Alarm_IRQn, (7UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
	NVIC_EnableIRQ(RTC_Alarm_IRQn);
}


/**************************************************************************************/
/* RTC_AlarmConfigure */
/**************************************************************************************/
void RTC_AlarmConfigure (RTC_AlarmTime_t *alarmtime)
{
	uint32_t reg_temp;

	/* Disable the RTC registers write protection */
	RTC_ProtectionDisable();
	
	/* Disable alarm A */
	CLEAR_BIT(RTC->CR, RTC_CR_ALRAE);
	
	/* Check that the RTC_ALRMAR register can be accessed */
	while(!READ_BIT(RTC->ISR, RTC_ISR_ALRAWF));
	
	/* Configure the alarm */
	reg_temp = 	(((((uint32_t) RTC_ByteToBcd2(alarmtime->Value.DayOrDate)) >> 4)) << RTC_ALRMAR_DT_Pos) |
							(((((uint32_t)RTC_ByteToBcd2(alarmtime->Value.DayOrDate)) & 0xF)) << RTC_ALRMAR_DU_Pos) |
							(((((uint32_t) RTC_ByteToBcd2(alarmtime->Value.Hour)) >> 4)) << RTC_ALRMAR_HT_Pos) |
							(((((uint32_t)RTC_ByteToBcd2(alarmtime->Value.Hour)) & 0xF)) << RTC_ALRMAR_HU_Pos) |
							(((((uint32_t) RTC_ByteToBcd2(alarmtime->Value.Minute)) >> 4)) << RTC_ALRMAR_MNT_Pos) |
							(((((uint32_t)RTC_ByteToBcd2(alarmtime->Value.Minute)) & 0xF)) << RTC_ALRMAR_MNU_Pos) |
							(((((uint32_t) RTC_ByteToBcd2(alarmtime->Value.Second)) >> 4)) << RTC_ALRMAR_ST_Pos) |
							(((((uint32_t)RTC_ByteToBcd2(alarmtime->Value.Second)) & 0xF)) << RTC_ALRMAR_SU_Pos);
	reg_temp |= RTC_TR_PM_CFG 	| 
							(((uint32_t) alarmtime->Mask.DayOrDate) << RTC_ALRMAR_MSK4_Pos) |
							(((uint32_t) alarmtime->Mask.DayOrDateSelection) << RTC_ALRMAR_WDSEL_Pos) |
							(((uint32_t) alarmtime->Mask.Hour) << RTC_ALRMAR_MSK3_Pos) |
							(((uint32_t) alarmtime->Mask.Minute) << RTC_ALRMAR_MSK2_Pos) |
							(((uint32_t) alarmtime->Mask.Second) << RTC_ALRMAR_MSK1_Pos);
	WRITE_REG(RTC->ALRMAR, reg_temp);
		
	/* Enable the RTC registers write protection */
	RTC_ProtectionEnable();
}

/**************************************************************************************/
/* RTC_AlarmEnable */
/**************************************************************************************/
void RTC_AlarmEnable (void)
{
	/* Disable the RTC registers write protection */
	RTC_ProtectionDisable();
	
	/* Enable RTC alarm A interrupt */
	SET_BIT(RTC->CR, RTC_CR_ALRAIE);
	
	/* Enable alarm A */
	SET_BIT(RTC->CR, RTC_CR_ALRAE);

	/* Enable the RTC registers write protection */
	RTC_ProtectionEnable();
}

/**************************************************************************************/
/* RTC_AlarmDisable */
/**************************************************************************************/
void RTC_AlarmDisable (void)
{
	/* Disable the RTC registers write protection */
	RTC_ProtectionDisable();
	
	/* Disable RTC alarm A interrupt */
	CLEAR_BIT(RTC->CR, RTC_CR_ALRAIE);

	/* Disable alarm A */
	CLEAR_BIT(RTC->CR, RTC_CR_ALRAE);

	/* Enable the RTC registers write protection */
	RTC_ProtectionEnable();
}


/**************************************************************************************/
/* RTC_ByteToBcd2 */
/**************************************************************************************/
static uint8_t RTC_ByteToBcd2(uint8_t Value)
{
  uint32_t bcdhigh = 0;

  while(Value >= 10)
  {
    bcdhigh++;
    Value -= 10;
  }

  return  ((uint8_t)(bcdhigh << 4) | Value);
}

/**************************************************************************************/
/* RTC_Bcd2ToByte */
/**************************************************************************************/
static uint8_t RTC_Bcd2ToByte(uint8_t Value)
{
  uint32_t tmp = 0;
  tmp = ((uint8_t)(Value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
  return (tmp + (Value & (uint8_t)0x0F));
}

/**************************************************************************************/
/* RTC_ProtectionDisable */
/**************************************************************************************/
static void RTC_ProtectionDisable(void)
{
	/* Disable the RTC registers write protection */
		WRITE_REG(RTC->WPR, 0xCAU);
		WRITE_REG(RTC->WPR, 0x53U);
}

/**************************************************************************************/
/* RTC_ProtectionEnable */
/**************************************************************************************/
static void RTC_ProtectionEnable(void)
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
	/* Clear alarm flag */
		CLEAR_BIT(RTC->ISR, RTC_ISR_ALRAF);
	}
}


