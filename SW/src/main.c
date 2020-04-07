#include "main.h"

#include <stdint.h>
#include "ClockDrv.h"
#include "ChipInfo.h"

int main(void)
{
	ErrorStatus result = SUCCESS;
	
	/* Get chip informations */
	ChipInfo_Get();

	/* Clock initialization */
	result &= ClockDrv_Init();	
	
	
	uint32_t x = 0;
	while(1)
	{
		x++;
	}
	
	return 0;
}
