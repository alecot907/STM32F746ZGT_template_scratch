/* This module gets the electronic signature information of the chip */

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "ChipInfo.h"


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/* This struct is possible since UID0, UID1 and UID2 have consecutive addresses in memory */
typedef struct {
	__I uint32_t UID0;
	__I uint32_t UID1;
	__I uint32_t UID2;
}UID_TypeDef;

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#define UID		((UID_TypeDef *) UID_BASE)

#define UID_UID0_XYCOORD_Pos	(0U)
#define UID_UID0_XYCOORD_Mask	(0xFFFFFFFFU << UID_UID0_XYCOORD_Pos)
#define UID_UID0_XYCOORD			(UID_UID0_XYCOORD_Mask)

#define UID_UID1_WAFNUM_Pos		(0U)
#define UID_UID1_WAFNUM_Mask	(0xFFU << UID_UID1_WAFNUM_Pos)
#define UID_UID1_WAFNUM				(UID_UID1_WAFNUM_Mask)
#define UID_UID1_LOTNUM0_Pos	(8U)
#define UID_UID1_LOTNUM0_Mask	(0xFFFFFFU << UID_UID1_LOTNUM0_Pos)
#define UID_UID1_LOTNUM0			(UID_UID1_LOTNUM0_Mask)

#define UID_UID2_LOTNUM1_Pos	(0U)
#define UID_UID2_LOTNUM1_Mask	(0xFFFFFFFFU << UID_UID2_LOTNUM1_Pos)
#define UID_UID2_LOTNUM1			(UID_UID2_LOTNUM1_Mask)

#define FLASHSIZE_FSIZE_Pos		(0U)
#define FLASHSIZE_FSIZE_Mask	(0xFFFFU << FLASHSIZE_FSIZE_Pos)
#define FLASHSIZE_FSIZE				(FLASHSIZE_FSIZE_Mask)

#define PACKAGE_PKG_Pos				(8U)
#define PACKAGE_PKG_Mask			(0x7U << PACKAGE_PKG_Pos)
#define PACKAGE_PKG						(PACKAGE_PKG_Mask)


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
ChipInfo_t ChipInfo = {0};

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

void ChipInfo_Get (void)
{
	/* Get MCU ID */
	ChipInfo.McuID.DeviceID = READ_BIT(DBGMCU->IDCODE, DBGMCU_IDCODE_DEV_ID) >> DBGMCU_IDCODE_DEV_ID_Pos;
	ChipInfo.McuID.RevisionID = READ_BIT(DBGMCU->IDCODE, DBGMCU_IDCODE_REV_ID) >> DBGMCU_IDCODE_REV_ID_Pos;	
	
	/* Get Unique ID */
	ChipInfo.UniqueID.XY_WaferCoord = READ_BIT(UID->UID0, UID_UID0_XYCOORD) >> UID_UID0_XYCOORD_Pos;
	ChipInfo.UniqueID.WaferNumber = READ_BIT(UID->UID1, UID_UID1_WAFNUM) >> UID_UID1_WAFNUM_Pos;
	
	ChipInfo.UniqueID.LotNumber = ((( ((uint64_t) (READ_BIT(UID->UID2, UID_UID2_LOTNUM1))) >> UID_UID2_LOTNUM1_Pos)) << 24) |
																(( ((uint64_t)READ_BIT(UID->UID1, UID_UID1_LOTNUM0)) >> UID_UID1_LOTNUM0_Pos));
	
	/* Get Flash size */
	ChipInfo.FlashSizeKbytes = ((*(uint16_t *)FLASHSIZE_BASE) & FLASHSIZE_FSIZE) >> FLASHSIZE_FSIZE_Pos;
	
	/* Get Package type */
	/* ?TBU? I should read 0b010 with my board, but I read 0b111 */
	ChipInfo.PackageType = ((*(uint16_t *)PACKAGE_BASE) & PACKAGE_PKG) >> PACKAGE_PKG_Pos;
}





