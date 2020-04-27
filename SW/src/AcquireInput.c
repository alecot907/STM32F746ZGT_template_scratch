/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "AcquireInput.h"

#include "Gpio_Drv.h"
#include "Gpio_Cfg.h"
#include "Adc_Cfg.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


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

