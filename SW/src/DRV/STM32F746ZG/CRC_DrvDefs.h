#ifndef __CRC_DRVDEFS_H
#define __CRC_DRVDEFS_H

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

typedef enum
{
	CRC_CR_REVOUT_STD = 0x0U,
	CRC_CR_REVOUT_REV
	
} CRC_CR_REVOUT_t;

typedef enum
{
	CRC_CR_REVIN_STD = 0x0U,
	CRC_CR_REVIN_REV_BYTE,
	CRC_CR_REVIN_REV_HALFWORD,
	CRC_CR_REVIN_REV_WORD
	
} CRC_CR_REVIN_t;

typedef enum
{
	CRC_CR_POLYSIZE_32 = 0x0U,
	CRC_CR_POLYSIZE_16,
	CRC_CR_POLYSIZE_8,
	CRC_CR_POLYSIZE_7
	
} CRC_CR_POLYSIZE_t;

typedef enum
{
	CRC_COMPUTE_TYPE_32 = 0x0U,
	CRC_COMPUTE_TYPE_16,
	CRC_COMPUTE_TYPE_8
	
} CRC_COMPUTE_TYPE_t;


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PUBLIC FUNCTIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* EXTERN VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


#endif /* __CRC_DRVDEFS_H */
