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

/* default date: (00:00:00) 20000101 */
RTC_Time_t RTC_Time = {	00, 01, 01, SATURDAY,
												21, 00, 00 };


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
												
												
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* RTC_Init */
/**************************************************************************************/
void RTC_Init (void)
{	
	/* Note: before calling this function, the source clock of RTC must be enabled and stable */
	uint32_t reg_temp;
	
	/* RTC clock source */
	MODIFY_REG(RCC->BDCR, RCC_BDCR_RTCSEL, RCC_BDCR_RTCSEL_CFG);

	/* RTC peripheral */
	/* RTC clock enable */
	SET_BIT(RCC->BDCR, RCC_BDCR_RTCEN);
	
	/* Disable the RTC registers write protection */
	WRITE_REG(RTC->WPR, 0xCAU);
	WRITE_REG(RTC->WPR, 0x53U);
	
  /* Check if the Initialization mode is set (if year == 00) */
	if(!READ_BIT(RTC->ISR, RTC_ISR_INITS))
	{		
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
								(((((uint32_t) RTC_ByteToBcd2(RTC_Time.Hour)) >> 4)) << RTC_TR_HT_Pos) |
								(((((uint32_t)RTC_ByteToBcd2(RTC_Time.Hour)) & 0xF)) << RTC_TR_HU_Pos) |
								(((((uint32_t) RTC_ByteToBcd2(RTC_Time.Minute)) >> 4)) << RTC_TR_MNT_Pos) |
								(((((uint32_t)RTC_ByteToBcd2(RTC_Time.Minute)) & 0xF)) << RTC_TR_MNU_Pos) |
								(((((uint32_t) RTC_ByteToBcd2(RTC_Time.Second)) >> 4)) << RTC_TR_ST_Pos) |
								(((((uint32_t)RTC_ByteToBcd2(RTC_Time.Second)) & 0xF)) << RTC_TR_SU_Pos);
		WRITE_REG(RTC->TR, reg_temp);
		
		
		reg_temp =	(((((uint32_t) RTC_ByteToBcd2(RTC_Time.Year)) >> 4)) << RTC_DR_YT_Pos) |
								(((((uint32_t)RTC_ByteToBcd2(RTC_Time.Year)) & 0xF)) << RTC_DR_YU_Pos) |
								(RTC_Time.DayOfWeek << RTC_DR_WDU_Pos) 	| 
								(((((uint32_t) RTC_ByteToBcd2(RTC_Time.Month)) >> 4)) << RTC_DR_MT_Pos) |
								(((((uint32_t)RTC_ByteToBcd2(RTC_Time.Month)) & 0xF)) << RTC_DR_MU_Pos) |
								(((((uint32_t) RTC_ByteToBcd2(RTC_Time.Day)) >> 4)) << RTC_DR_DT_Pos) |
								(((((uint32_t)RTC_ByteToBcd2(RTC_Time.Day)) & 0xF)) << RTC_DR_DU_Pos);
		WRITE_REG(RTC->DR, reg_temp);		
		
		/* Exit Initialization mode */
		CLEAR_BIT(RTC->ISR, RTC_ISR_INIT);
			
		/* Wait the registers to be synchronised */
		CLEAR_BIT(RTC->ISR, RTC_ISR_RSF);
		while(!READ_BIT(RTC->ISR, RTC_ISR_RSF));		
	}
	
	/* Enable the RTC Registers Write Protection */
	WRITE_REG(RTC->WPR, 0xFFU);
	
	RTC_GetTime(&RTC_Time);
}


/**************************************************************************************/
/* RTC_ResetTime */
/**************************************************************************************/
void RTC_ResetTime (RTC_Time_t *time)
{
	// TODO
}


/**************************************************************************************/
/* RTC_SetTime */
/**************************************************************************************/
void RTC_SetTime (RTC_Time_t *time){
	// TODO
}

/**************************************************************************************/
/* RTC_GetTime */
/**************************************************************************************/
void RTC_GetTime (RTC_Time_t *time){
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

