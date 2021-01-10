/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "AcquireInput.h"

#include "Gpio_Drv.h"
#include "Adc_Cfg.h"
#include "Clock_Drv.h"
#include "Timer_Drv.h"



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

struct SONICSENSOR_OBJ_t SonicSensor_Obj = 
{
	SONICSENSOR_STATE_INIT,
	0U,
	0U,
	{0U,0U},
	0U,
	0.0f
};

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* QualifyInput */
/**************************************************************************************/
static void QualifyInput (SYSDATA_IN_t signal, uint32_t qualify_ticks);


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* AcquireInput */
/**************************************************************************************/
void AcquireInput (void)
{
	/* Input acquisition */
	SysData_In[SYS_IN_BUTTONUSR] = (SysData_In[SYS_IN_BUTTONUSR] & INPUT_MASK_INV) | 
																	(Gpio_Drv_GetPin(BUTTONUSR_PORT, BUTTONUSR_PIN) << INPUT_RAWPOS);
	
	SysData_In[SYS_IN_POTENTIOMETER] = ADC1_DMA_Samples[0];
	SysData_In[SYS_IN_LIGHT_SENSOR] = ADC1_DMA_Samples[1];
	
	/* Input qualification */
	QualifyInput(SYS_IN_BUTTONUSR, 5U);
}



/**************************************************************************************/
/* QualifyInput */
/**************************************************************************************/
static void QualifyInput (SYSDATA_IN_t signal, uint32_t qualify_ticks)
{	
	if (HIGH == (SysData_In[signal] >> INPUT_RAWPOS))
	{
		if ((SysData_In[signal] & INPUT_MASK_INV) >= (qualify_ticks * 0x02U))
		{
			SysData_In[signal] = ((HIGH & 0x1U) << INPUT_RAWPOS) | qualify_ticks * 0x02U | HIGH;
		}
		SysData_In[signal] += 0x02U;
	}
	if (LOW == (SysData_In[signal] >> INPUT_RAWPOS))
	{
		if ((SysData_In[signal] & INPUT_MASK_INV) <= 0x02U)
		{
			SysData_In[signal] = (LOW << INPUT_RAWPOS) | 0x02U | LOW;
		}
		SysData_In[signal] -= 0x02U;
	}
}


/**************************************************************************************/
/* Acquire_SonicSensor */
/**************************************************************************************/
void Acquire_SonicSensor (void)
{
	switch (SonicSensor_Obj.state)
	{
		case SONICSENSOR_STATE_INIT:
			SonicSensor_Obj.start_time = sys_ticks;
			SonicSensor_Obj.state = SONICSENSOR_STATE_LAUNCHTRIGGER;
			SonicSensor_Obj.rising_falling_state = 0U;
			break;
		case SONICSENSOR_STATE_LAUNCHTRIGGER:
			if ((sys_ticks - SonicSensor_Obj.start_time) > 500U)
			{
				Timer_Drv_Start(TIMERGENERAL_13_SONICECHO);
				Timer_Drv_Start(TIMERGENERAL_11_SONICTRIG);
				
				SonicSensor_Obj.start_time = sys_ticks;
				SonicSensor_Obj.state = SONICSENSOR_STATE_MEASURE;
			}
			break;
		case SONICSENSOR_STATE_MEASURE:
			if ((sys_ticks - SonicSensor_Obj.start_time) > 200U)
			{
				// NO MEASURE!!
				SonicSensor_Obj.rising_falling_measure[0U] = 0U;
				SonicSensor_Obj.rising_falling_measure[1U] = 0U;
				SonicSensor_Obj.pulse_width = 0;
				SonicSensor_Obj.distance = 0U;
				SonicSensor_Obj.state = SONICSENSOR_STATE_INIT;
			}
			else if (SonicSensor_Obj.rising_falling_state == 2U)
			{
				SonicSensor_Obj.pulse_width = SonicSensor_Obj.rising_falling_measure[1U] - SonicSensor_Obj.rising_falling_measure[0U];
				if ((SonicSensor_Obj.pulse_width > 150) && (SonicSensor_Obj.pulse_width < 50000))
				{
					SonicSensor_Obj.distance = (uint16_t) SonicSensor_Obj.pulse_width / 58;
				}
				else
				{
					SonicSensor_Obj.distance = 0U;
				}
				
				SonicSensor_Obj.state = SONICSENSOR_STATE_INIT;
			}
		default:
			break;
	}
}

