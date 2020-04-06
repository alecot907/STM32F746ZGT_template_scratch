#include "main.h"

#include <stdint.h>
#include "ClockDrv.h"
#include "stm32f746xx.h"


int main(void)
{
	ErrorStatus result = SUCCESS;
	
	/* Clock initialization */
	result &= ClockDrv_Init();
	
	
	
	// controllare RTC, get info dal chip
	
	
	
	
	
	uint32_t x = 0;
	while(1)
	{
		x++;
	}
	
	return 0;
}
