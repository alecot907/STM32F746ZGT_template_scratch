#ifndef __ACQUIREINPUT_H
#define __ACQUIREINPUT_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "stm32f7xx.h"
#include "SysData.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
typedef enum
{
	SONICSENSOR_STATE_INIT = 0U,
	SONICSENSOR_STATE_LAUNCHTRIGGER,
	SONICSENSOR_STATE_MEASURE
} SONICSENSOR_STATE_t;

struct SONICSENSOR_OBJ_t
{
	SONICSENSOR_STATE_t 	state;
	uint32_t 							start_time;
	uint8_t								rising_falling_state;
	uint16_t							rising_falling_measure[2];
	int32_t								pulse_width;
	uint16_t							distance;		/*[cm]*/
};

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#define INPUT_RAWPOS 			(31U)
#define INPUT_MASK_INV 		(0x7FFFFFFF)

#define INPUT_QUAL(X)			(X & 0x1U)
#define INPUT_NOTQUAL(X)	(X >> INPUT_RAWPOS)


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* AcquireInput */
/**************************************************************************************/
void AcquireInput (void);

/**************************************************************************************/
/* Acquire_SonicSensor */
/**************************************************************************************/
void Acquire_SonicSensor (void);


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
extern struct SONICSENSOR_OBJ_t SonicSensor_Obj;

#endif /* __ACQUIREINPUT_H */
