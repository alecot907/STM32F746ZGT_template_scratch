/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include <string.h>
#include "CRC_Drv.h"
#include "CRC_Cfg.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* CRC_Drv_Init */
/**************************************************************************************/
void CRC_Drv_Init (void)
{
	/* Enable CRC clock peripheral */
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_CRCEN);
		
	/* Set initial CRC value */
	WRITE_REG(CRC->INIT, CRC_INIT_POLY_CFG);

	/* Set coefficient polynomial */
	WRITE_REG(CRC->POL, CRC_POL_POLY_CFG);
	
	/* Set bit order, polynomial size and reset peripheral */
	WRITE_REG(CRC->CR, (CRC_CR_REVOUT_CFG << CRC_CR_REV_OUT_Pos) |
											(CRC_CR_REVIN_CFG << CRC_CR_REV_IN_Pos) |
											(CRC_CR_POLYSIZE_CFG << CRC_CR_POLYSIZE_Pos) |
											CRC_CR_RESET);
	
	/* Disable CRC clock peripheral */
	CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_CRCEN);
}


/**************************************************************************************/
/* CRC_Drv_Compute */
/**************************************************************************************/
uint32_t CRC_Drv_Compute (uint32_t *data, uint32_t bytes_number, CRC_COMPUTE_TYPE_t type)
{
	uint32_t counter = 0U;
	uint32_t temp_reg = 0U;
	uint32_t CRC_computed = 0U;
	
	/* Enable CRC clock peripheral */
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_CRCEN);
	
	/* Reset peripheral */
	SET_BIT(CRC->CR, CRC_CR_RESET);
	
	switch (type)
	{
		case CRC_COMPUTE_TYPE_32:
			for (counter = 0U; counter < (bytes_number/sizeof(uint32_t)); counter++)
			{
				WRITE_REG(CRC->DR, *(data+counter));
			}
			break;
		case 	CRC_COMPUTE_TYPE_16:
			for (counter = 0U; counter < (bytes_number/sizeof(uint32_t)); counter++)
			{
				temp_reg = ((*(data+counter) & 0x0000FFFF) << 16U) |
										((*(data+counter) & 0xFFFF0000) >> 16U);
				WRITE_REG(CRC->DR, temp_reg);
			}
			/* Manage last half-word, if number of half-word is not a multiple of 4 */
			if ((bytes_number % 4U) != 0U)
			{
				uint16_t *reg_ptr = NULL;
				reg_ptr = (uint16_t *) &CRC->DR;
				*reg_ptr = (uint16_t) *(data+counter);
			}
			break;
		case CRC_COMPUTE_TYPE_8:
			for (counter = 0U; counter < (bytes_number/sizeof(uint32_t)); counter++)
			{
				temp_reg = ((*(data+counter) & 0x000000FF) << 24U) |
										((*(data+counter) & 0x0000FF00) << 8U) |
										((*(data+counter) & 0x00FF0000) >> 8U) |
										((*(data+counter) & 0xFF000000) >> 24U);
				WRITE_REG(CRC->DR, temp_reg);
			}
			/* Manage last bytes, if number of bytes is not a multiple of 4 */
			if ((bytes_number % 4U) != 0U)
			{
				uint8_t last_bytes_counter = bytes_number % 4U;
				uint32_t last_bytes = *(data+counter);
				uint8_t *reg_ptr = NULL;
				reg_ptr = (uint8_t *) &CRC->DR;
				
				if (last_bytes_counter == 3U)
				{
					*reg_ptr = (uint8_t) (last_bytes & 0xFF);
					last_bytes = last_bytes >> 8U;
					last_bytes_counter--;
				}
				if (last_bytes_counter == 2U)
				{
					*reg_ptr = (uint8_t) (last_bytes & 0xFF);
					last_bytes = last_bytes >> 8U;
					last_bytes_counter--;
				}
				if (last_bytes_counter == 1U)
				{
					*reg_ptr = (uint8_t) (last_bytes & 0xFF);
				}
			}
			break;
		default:
			break;
	}
	
	CRC_computed = READ_REG(CRC->DR);
	
	/* Disable CRC clock peripheral */
	CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_CRCEN);
		
	return CRC_computed;
}



