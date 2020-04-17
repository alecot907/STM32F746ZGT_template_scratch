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
#define MAX_BYTE_RXBUFF	(3U)  // ID, IDS, LENGTH

#define RX_SYNC0				(0x42U)
#define RX_SYNC1				(0x43U)

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* TYPE definition */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/* Important: the order of this enum must be fixed */
typedef enum
{
	RX_SYNC_STATE_ID = 0x00U,
	RX_SYNC_STATE_IDS,
	RX_SYNC_STATE_LENGTH,
	RX_SYNC_STATE_DATA,
	
	RX_SYNC_STATE_SYNC0,
	RX_SYNC_STATE_SYNC1,
	
} RX_SYNC_STATE_t;

typedef enum
{
	RX_PACKET_ID_RTC = 0x01U	
} RX_PACKET_ID_t;



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* VARIABLES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
static uint8_t rx_buff[MAX_BYTE_RXBUFF];
static RX_SYNC_STATE_t rx_sync_index = RX_SYNC_STATE_SYNC0;
static uint8_t rx_DataCounter = 0x00U;

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* PRIVATE FUNCTIONS PROTOTYPES */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/**************************************************************************************/
/* RxPacketRtc_Mng */
/**************************************************************************************/
static void RxPacketRtc_Mng (uint8_t ids, uint8_t counter, uint8_t *data);

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
				rx_sync_index = RX_SYNC_STATE_SYNC1;
			}
			break;
		case RX_SYNC_STATE_SYNC1:
			if (RX_SYNC1 == (*data))
			{
				rx_sync_index = RX_SYNC_STATE_ID;
			}
			else
			{
				rx_sync_index = RX_SYNC_STATE_SYNC0;
			}
			break;
		case RX_SYNC_STATE_ID:
			rx_buff[rx_sync_index] = (*data);
			rx_sync_index = RX_SYNC_STATE_IDS;
			break;
		case RX_SYNC_STATE_IDS:
			rx_buff[rx_sync_index] = (*data);
			rx_sync_index = RX_SYNC_STATE_LENGTH;
			break;
		case RX_SYNC_STATE_LENGTH:
			rx_DataCounter = 0x00U;
			rx_buff[rx_sync_index] = (*data);
			rx_sync_index = RX_SYNC_STATE_DATA;
			break;
		case RX_SYNC_STATE_DATA:
			/* Data to be received untile length is reached. last data is CRC */
				switch (rx_buff[RX_SYNC_STATE_ID])
				{
					case RX_PACKET_ID_RTC:
						RxPacketRtc_Mng(rx_buff[RX_SYNC_STATE_IDS], rx_DataCounter, data);
						break;
					default:
						break;
				}
				
				rx_DataCounter++;
				
				/* CRC received. Ready for another sync */
				if (rx_DataCounter == (rx_buff[RX_SYNC_STATE_LENGTH]))
				{
					rx_sync_index = RX_SYNC_STATE_SYNC0;
				}
			break;
		default:
			break;
	}
}


/**************************************************************************************/
/* RxPacketRtc_Mng */
/**************************************************************************************/
static void RxPacketRtc_Mng (uint8_t ids, uint8_t counter, uint8_t *data)
{
	switch (ids)
	{
		case RTC_IDS_USART_TIMEDATE:
			switch (counter)
			{
				case 0x00U:
					RTC_TimeUsart.SetCfg.Year = (*data);
					break;
				case 0x01U:
					RTC_TimeUsart.SetCfg.Month = ((*data) & 0xF0) >> 0x04;
					RTC_TimeUsart.SetCfg.DayOfWeek = (RTC_DayOfWeek_t) ((*data) & 0x0F);
					break;
				case 0x02U:
					RTC_TimeUsart.SetCfg.Day = (*data);
					break;
				case 0x03U:
					RTC_TimeUsart.SetCfg.Hour = (*data);
					break;
				case 0x04U:
					RTC_TimeUsart.SetCfg.Minute = (*data);
					break;
				case 0x05U:
					RTC_TimeUsart.SetCfg.Second = (*data);
					break;
				case 0x06U:
					RTC_TimeUsart.CRC8 = (*data);
					RTC_TimeUsart.IDS = ids;
					break;
				default:
					break;
			}
			break;
		case RTC_IDS_USART_ALARM:
			switch (counter)
			{
				case 0x00U:
					RTC_AlarmTimeValueUart.SetAlarmCfg.Value.Hour = (*data);
					RTC_AlarmTimeValueUart.SetAlarmCfg.Value.DayOrDate = 0x00U;
					break;
				case 0x01U:
					RTC_AlarmTimeValueUart.SetAlarmCfg.Value.Minute = (*data);
					RTC_AlarmTimeValueUart.SetAlarmCfg.Value.Second = 0x00U;
					break;
				case 0x02U:
					RTC_AlarmTimeValueUart.CRC8 = (*data);
					RTC_AlarmTimeValueUart.IDS = ids;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}





