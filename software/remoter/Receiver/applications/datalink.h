#ifndef __DATALINK_H__
#define __DATALINK_H__
#include <stdint.h>

#define DATALINK_MAGIC             (0xaa)
#define DATALINK_MAX_PAYLOAD_SIZE  (32)

typedef enum
{
    DATA_TYPE_HEARTBEAT = 0x00,

    DATA_TYPE_BIND = 0x01,

    DATA_TYPE_RC = 0x02,
    
    DATA_TYPE_ANOLINK = 0x03,
    
    DATA_TYPE_MAVLINK = 0x04,

    DATA_TYPE_DAPLINK = 0x05,

    DATA_TYPE_UNDEFINE = 0xff,

} data_type_t;

#pragma pack (1)

typedef struct
{
uint8_t type;
uint8_t data[DATALINK_MAX_PAYLOAD_SIZE -1]; //< Data
}payload_t;

typedef struct
{
  int8_t rssi;
  uint8_t len;                         //< len of payload
  payload_t payload;
} DataLinkRxPacket_t;

typedef struct
{
  uint8_t magic;
  uint8_t checksum;
  uint8_t rssi;
  uint8_t len;                         //< len of payload
  payload_t payload;
} DataLinkUARTPacket_t;

#pragma pack ()

uint8_t datalink_checksum(uint8_t *data,uint16_t len);

uint8_t datalink_parse_char(DataLinkUARTPacket_t *pkg,uint8_t ch);

#endif  /* endif __DATALINK_H__ */
