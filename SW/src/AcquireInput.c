/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "AcquireInput.h"

#include "main.h"
#include "SysData.h"

#include "Gpio_Drv.h"
#include "Gpio_Cfg.h"

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
	SysData_In[SYS_IN_BUTTONUSR] = (SysData_In[SYS_IN_BUTTONUSR] & (~0x01U)) | Gpio_Drv_GetPin(BUTTONUSR_PORT, BUTTONUSR_PIN);
		
	/* Input qualification */
	QualifyInput(SYS_IN_BUTTONUSR, 5U);
}



/**************************************************************************************/
/* QualifyInput */
/**************************************************************************************/
static void QualifyInput (SYSDATA_IN_t signal, uint32_t qualify_ticks)
{	
	if (HIGH == (SysData_In[signal] & 0x01U))
	{
		if ((SysData_In[signal] & 0xFFFFFFFE) >= (qualify_ticks * 0x02U))
		{
			SysData_In[signal] = qualify_ticks * 0x02U;
			SysData_Der[signal] = HIGH;
		}
		SysData_In[signal] += 0x02U;
	}
	if (LOW == (SysData_In[signal] & 0x01U))
	{
		if ((SysData_In[signal] & 0xFFFFFFFE) <= 0x02U)
		{
			SysData_In[signal] = 0x02U;
			SysData_Der[signal] = LOW;
		}
		SysData_In[signal] -= 0x02U;
	}
}

