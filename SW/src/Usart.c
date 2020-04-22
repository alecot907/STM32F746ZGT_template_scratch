/* Protocol description 
- Packet structure:
| RX_SYNC0 | RX_SYNC1 | ID | IDs | LENGTH | D0 ... D_LENGTH | CRC | RX_STOP |
	
- Packet description:
ID					IDs
0x01 (RTC)	0x01 (Set time, date)
						0x02 (Set alarm)
0x01 ()			-

*/

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEPENDENCIES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "Usart.h"
#include "Rtc.h"

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* DEFINES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#define RX_SYNC0				(0x42U)
#define RX_SYNC1				(0x43U)

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/* Important: the order of this enum must be fixed */
typedef enum
{
	RX_SYNC_STATE_SYNC0 = -0x02,
	RX_SYNC_STATE_SYNC1,
	RX_SYNC_STATE_ID_MSB ,
	RX_SYNC_STATE_ID_LSB,
	RX_SYNC_STATE_LENGTH,
	RX_SYNC_STATE_DATA
	
} RX_SYNC_STATE_t;




/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
static uint8_t rx_buff[RX_SYNC_STATE_LENGTH + 1U];
static RX_SYNC_STATE_t rx_sync_index = RX_SYNC_STATE_SYNC0;

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* RxPacketRtc_Mng */
/**************************************************************************************/
static void RxPacketRtc_Mng (uint8_t *rxbuff, uint8_t counter, uint8_t *data);

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* FUNCTIONS DECLARATIONS */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* Usart2RX_mng */
/**************************************************************************************/
void Usart2RX_mng (uint8_t *data)
{
	/* manage RX */
	switch (rx_sync_index)
	{
		case RX_SYNC_STATE_SYNC0:
			if (RX_SYNC0 == (*data))
			{
				rx_sync_index++;
			}
			break;
		case RX_SYNC_STATE_SYNC1:
			if (RX_SYNC1 == (*data))
			{
				rx_sync_index++;
			}
			else
			{
				rx_sync_index = RX_SYNC_STATE_SYNC0;
			}
			break;
		case RX_SYNC_STATE_ID_MSB:
			rx_buff[rx_sync_index++] = (*data);
			break;
		case RX_SYNC_STATE_ID_LSB:
			rx_buff[rx_sync_index++] = (*data);
			break;
		case RX_SYNC_STATE_LENGTH:
			rx_buff[rx_sync_index++] = (*data);
			break;
		case RX_SYNC_STATE_DATA:
		default:
				switch (rx_buff[RX_SYNC_STATE_ID_MSB])
				{
					case RX_PACKET_ID_RTC:
						RxPacketRtc_Mng(rx_buff, rx_sync_index - RX_SYNC_STATE_DATA, data);
						break;
					default:
						break;
				}
				
				rx_sync_index++;
				
				/* CRC received. Ready for another sync */
				if ((rx_sync_index - RX_SYNC_STATE_DATA) == rx_buff[RX_SYNC_STATE_LENGTH])
				{
					rx_sync_index = RX_SYNC_STATE_SYNC0;
				}
			break;
	}
}


/**************************************************************************************/
/* RxPacketRtc_Mng */
/**************************************************************************************/
static void RxPacketRtc_Mng (uint8_t *rxbuff, uint8_t counter, uint8_t *data)
{
	switch (*(rxbuff + RX_SYNC_STATE_ID_LSB))
	{
		case (RX_PACKET_ID_RTC_TIMEDATE & 0xFF):
			switch (counter)
			{
				case 0x00U:
					RTC_TimeUsart.structure.SetCfg.Year = (*data);
					RTC_TimeUsart.structure.Length = *(rxbuff + RX_SYNC_STATE_LENGTH);
					break;
				case 0x01U:
					RTC_TimeUsart.structure.SetCfg.Month = ((*data) & 0xF0) >> 0x04;
					RTC_TimeUsart.structure.SetCfg.DayOfWeek = (RTC_DayOfWeek_t) ((*data) & 0x0F);
					break;
				case 0x02U:
					RTC_TimeUsart.structure.SetCfg.Day = (*data);
					break;
				case 0x03U:
					RTC_TimeUsart.structure.SetCfg.Hour = (*data);
					break;
				case 0x04U:
					RTC_TimeUsart.structure.SetCfg.Minute = (*data);
					break;
				case 0x05U:
					RTC_TimeUsart.structure.SetCfg.Second = (*data);
					break;
				case 0x06U:
					RTC_TimeUsart.structure.CRC32 = (uint32_t) (*data) << 24U;
					break;
				case 0x07U:
					RTC_TimeUsart.structure.CRC32 |= (uint32_t) (*data) << 16U;
					break;
				case 0x08U:
					RTC_TimeUsart.structure.CRC32 |= (uint32_t) (*data) << 8U;
					break;
				case 0x09U:
					RTC_TimeUsart.structure.CRC32 |= (*data);
					RTC_TimeUsart.structure.ID = (RX_PACKET_ID_t) ((*(rxbuff + RX_SYNC_STATE_ID_MSB) << 8U) | *(rxbuff + RX_SYNC_STATE_ID_LSB));
					break;
				default:
					break;
			}
			break;
		case (RX_PACKET_ID_RTC_ALARM & 0xFF):
			switch (counter)
			{
				case 0x00U:
					RTC_AlarmTimeValueUart.structure.SetAlarmCfg.Hour = (*data);
					RTC_AlarmTimeValueUart.structure.Length = *(rxbuff + RX_SYNC_STATE_LENGTH);
					break;
				case 0x01U:
					RTC_AlarmTimeValueUart.structure.SetAlarmCfg.Minute = (*data);
					break;
				case 0x02U:
					RTC_AlarmTimeValueUart.structure.CRC32 = (uint32_t) (*data) << 24U;
					break;
				case 0x03U:
					RTC_AlarmTimeValueUart.structure.CRC32 |= (uint32_t) (*data) << 16U;
					break;
				case 0x04U:
					RTC_AlarmTimeValueUart.structure.CRC32 |= (uint32_t) (*data) << 8U;
					break;
				case 0x05U:
					RTC_AlarmTimeValueUart.structure.CRC32 |= (*data);
					RTC_AlarmTimeValueUart.structure.ID = (RX_PACKET_ID_t) ((*(rxbuff + RX_SYNC_STATE_ID_MSB) << 8U) | *(rxbuff + RX_SYNC_STATE_ID_LSB));
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}





