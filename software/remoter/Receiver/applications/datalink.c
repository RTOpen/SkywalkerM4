#include "datalink.h"

#define STAT_SYNC     0x00
#define STAT_CHECKSUM 0x01
#define STAT_RSSI     0x02
#define STAT_LEN      0x03
#define STAT_TYPE     0x04
#define STAT_DATA     0x05

typedef struct
{
    uint8_t state;
    uint8_t checksum;
    uint8_t index;
    uint32_t error_count;
} parse_state_t;

static parse_state_t parser;

uint8_t datalink_checksum(uint8_t *data,uint16_t len)
{
    uint8_t checksum = 0;
    while(len--)
    {
        checksum += *data;
        data++;
    }
    return checksum;
}

uint8_t datalink_parse_char(DataLinkUARTPacket_t *pkg,uint8_t ch)
{
        switch (parser.state)
        {
        case STAT_SYNC:
            if (ch == DATALINK_MAGIC)
            {
                parser.state = STAT_CHECKSUM;
                parser.index = 0;
                parser.checksum = 0;
                pkg->magic = ch;
            }
            break;
        case STAT_CHECKSUM:
            pkg->checksum = ch;
            parser.state = STAT_RSSI;
            break;
        case STAT_RSSI:
            pkg->rssi = ch;
            parser.state = STAT_LEN;
            break;
        case STAT_LEN:
            pkg->len = ch;
            parser.state = STAT_TYPE;
            break;
        case STAT_TYPE:
            pkg->payload.type = ch;
            parser.checksum += ch;
            parser.state = STAT_DATA;
            break;
        case STAT_DATA:
            pkg->payload.data[parser.index++] = ch;
            parser.checksum += ch;
            if (parser.index + 1 >= pkg->len)
            {
                    if (parser.checksum != pkg->checksum)
                    {
                        parser.state = STAT_SYNC;  //帧校验错误，重新接收下一帧
                        parser.error_count++;
                        return 0;
                    }
                    parser.state = STAT_SYNC;
                    parser.checksum = 0;
                    return 1;
            }
            break;
        default:
            parser.state = STAT_SYNC;
            break;
        }
    return 0;
}
