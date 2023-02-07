#include "datalink.h"
#include "string.h"

#define STAT_SYNC     0x00
#define STAT_HEAD     0x01
#define STAT_DATA     0x02

typedef struct
{
    uint8_t state;
    uint8_t checksum;
    uint16_t index;
	  uint32_t error_count;
} parse_state_t;

parse_state_t parse_state[LCS_MAX_CHAN];

static uint8_t getCheckSum(uint8_t *data, uint16_t length)
{
    uint8_t checksum = 0;
    while (length--)
    {
        checksum += *data++;
    }
    return checksum;
}

uint16_t LCS_ModuleInfoFramePack(lcs_data_frame_t *frame, uint8_t *id, uint32_t hw_version, uint32_t sw_version)
{
    module_info_t *module_info = (module_info_t *)frame->frame.data;
    frame->frame.header.start = FRAME_START;
    frame->frame.header.cmd = CMD_GET_MODULE_INFO;
    frame->frame.header.length = sizeof(module_info_t);
    memcpy(module_info->id, id, 12);
    module_info->hw_version = hw_version;
    module_info->sw_version = sw_version;
    frame->frame.data[frame->frame.header.length] = getCheckSum(frame->raw_data, frame->frame.header.length + 4);
    frame->frame.data[frame->frame.header.length + 1] = FRAME_END;
    return (frame->frame.header.length + 6);
}

uint16_t LCS_ModuleInfoToFrame(lcs_data_frame_t *frame, module_info_t *module_info)
{
    frame->frame.header.start = FRAME_START;
    frame->frame.header.cmd = CMD_GET_MODULE_INFO;
    frame->frame.header.length = sizeof(module_info_t);
    memcpy(frame->frame.data, module_info, sizeof(module_info_t));
    frame->frame.data[frame->frame.header.length] = getCheckSum(frame->raw_data, frame->frame.header.length + 4);
    frame->frame.data[frame->frame.header.length + 1] = FRAME_END;
    return (frame->frame.header.length + 6);
}

uint16_t LCS_UpgradeRequestPack(lcs_data_frame_t *frame, uint32_t sw_version, uint32_t hw_version)
{
    frame->frame.header.start = FRAME_START;
    frame->frame.header.cmd = CMD_UPGRADE_REQ;
    frame->frame.header.length = 8;
    memcpy(&frame->frame.data[0], &sw_version, 4);
    memcpy(&frame->frame.data[4], &hw_version, 4);
    frame->frame.data[frame->frame.header.length] = getCheckSum(frame->raw_data, frame->frame.header.length + 4);
    frame->frame.data[frame->frame.header.length + 1] = FRAME_END;

    return (frame->frame.header.length + 6);
}
uint16_t LCS_UpgradeInfoAckPack(lcs_data_frame_t *frame, uint8_t ack)
{
    frame->frame.header.start = FRAME_START;
    frame->frame.header.cmd = CMD_UPGRADE_INFO;
    frame->frame.header.length = 1;
    frame->frame.data[0] = ack;
    frame->frame.data[frame->frame.header.length] = getCheckSum(frame->raw_data, frame->frame.header.length + 4);
    frame->frame.data[frame->frame.header.length + 1] = FRAME_END;

    return (frame->frame.header.length + 6);
}
uint16_t LCS_UpgradeDataPack(lcs_data_frame_t *frame, uint16_t index)
{
    frame->frame.header.start = FRAME_START;
    frame->frame.header.cmd = CMD_UPGRADE_PACK;
    frame->frame.header.length = 2;
    memcpy(&frame->frame.data[0], &index, 2);
    frame->frame.data[frame->frame.header.length] = getCheckSum(frame->raw_data, frame->frame.header.length + 4);
    frame->frame.data[frame->frame.header.length + 1] = FRAME_END;

    return (frame->frame.header.length + 6);
}

uint16_t LCS_UpgradeFinish(lcs_data_frame_t *frame, uint8_t result)
{
    frame->frame.header.start = FRAME_START;
    frame->frame.header.cmd = CMD_UPGRADE_FINISH;
    frame->frame.header.length = 1;
    frame->frame.data[0] = result;
    frame->frame.data[frame->frame.header.length] = getCheckSum(frame->raw_data, frame->frame.header.length + 4);
    frame->frame.data[frame->frame.header.length + 1] = FRAME_END;

    return (frame->frame.header.length + 6);
}
uint16_t LCS_DiscoveryPack(lcs_data_frame_t *frame, uint8_t *mac, uint32_t sw_ver, uint32_t build_date)
{
    frame->frame.header.start = FRAME_START;
    frame->frame.header.cmd = CMD_DISCOVERY;
    frame->frame.header.length = 54;
    memset(&frame->frame.data[0], 0, 40);
    memcpy(&frame->frame.data[0], "DIGITAL ETH BUS", 19);
    memcpy(&frame->frame.data[20], "0000000000001", 15);
    memcpy(&frame->frame.data[40], mac, 6);
    memcpy(&frame->frame.data[46], &sw_ver, 4);
    memcpy(&frame->frame.data[50], &build_date, 4);
    frame->frame.data[frame->frame.header.length] = getCheckSum(frame->raw_data, frame->frame.header.length + 4);
    frame->frame.data[frame->frame.header.length + 1] = FRAME_END;

    return (frame->frame.header.length + 6);
}

uint16_t LCS_DeviceStatusToFrame(lcs_data_frame_t *frame, device_status_t *status)
{
    frame->frame.header.start = FRAME_START;
    frame->frame.header.cmd = CMD_GET_STATUS;
    frame->frame.header.length = sizeof(device_status_t);
    memcpy(frame->frame.data, status, sizeof(device_status_t));
    frame->frame.data[frame->frame.header.length] = getCheckSum(frame->raw_data, frame->frame.header.length + 4);
    frame->frame.data[frame->frame.header.length + 1] = FRAME_END;
    return (frame->frame.header.length + 6);
}

uint16_t LCS_NetworkConfigToFrame(lcs_data_frame_t *frame, network_config_t *config)
{
    frame->frame.header.start = FRAME_START;
    frame->frame.header.cmd = CMD_GET_NETWORK_CONFIG;
    frame->frame.header.length = sizeof(network_config_t);
    memcpy(frame->frame.data, config, sizeof(network_config_t));
    frame->frame.data[frame->frame.header.length] = getCheckSum(frame->raw_data, frame->frame.header.length + 4);
    frame->frame.data[frame->frame.header.length + 1] = FRAME_END;
    return (frame->frame.header.length + 6);
}

uint16_t LCS_NetworkConfigResultPack(lcs_data_frame_t *frame, uint8_t result)
{
    frame->frame.header.start = FRAME_START;
    frame->frame.header.cmd = CMD_SET_NETWORK_CONFIG;
    frame->frame.header.length = 1;
    frame->frame.data[0] = result;
    frame->frame.data[frame->frame.header.length] = getCheckSum(frame->raw_data, frame->frame.header.length + 4);
    frame->frame.data[frame->frame.header.length + 1] = FRAME_END;
    return (frame->frame.header.length + 6);
}
uint16_t LCS_BusDataToFrame(lcs_data_frame_t *frame, bus_msg_t *msg,uint16_t count)
{
    frame->frame.header.start = FRAME_START;
    frame->frame.header.cmd = CMD_RW_I8080;
    frame->frame.header.length = sizeof(bus_msg_t)*count;
    memcpy(frame->frame.data, msg, sizeof(bus_msg_t)*count);
    frame->frame.data[frame->frame.header.length] = getCheckSum(frame->raw_data, frame->frame.header.length + 4);
    frame->frame.data[frame->frame.header.length + 1] = FRAME_END;
    return (frame->frame.header.length + 6);
}

uint16_t LCS_BusIrqDataToFrame(lcs_data_frame_t *frame, bus_msg_t *msg,uint16_t count)
{
    frame->frame.header.start = FRAME_START;
    frame->frame.header.cmd = CMD_IRQ_I8080_DATA;
    frame->frame.header.length = sizeof(bus_msg_t)*count;
    memcpy(frame->frame.data, msg, sizeof(bus_msg_t)*count);
    frame->frame.data[frame->frame.header.length] = getCheckSum(frame->raw_data, frame->frame.header.length + 4);
    frame->frame.data[frame->frame.header.length + 1] = FRAME_END;
    return (frame->frame.header.length + 6);
}
uint8_t LCS_ParseChar(uint8_t chan, lcs_data_frame_t *frame, uint8_t ch)
{
    if (chan < DLINK_MAX_CHAN)
    {
        parse_state_t *parse = &parse_state[chan];
        switch (parse->state)
        {
        case STAT_SYNC:
            if (ch == FRAME_START)
            {
                parse->state = STAT_HEAD;
                parse->index = 0;
                parse->checksum = ch;
                frame->raw_data[parse->index++] = ch;
            }
            break;
        case STAT_HEAD:
            parse->checksum += ch;
            frame->raw_data[parse->index++] = ch;
            if (parse->index >= 4)
            {
                if (frame->frame.header.length > (DLINK_MAX_FRAME_LEN - 4))
                {
                    parse->state = STAT_SYNC;
                }
                else
                {
                    parse->state = STAT_DATA;
                }
            }
            break;
        case STAT_DATA:
            frame->raw_data[parse->index++] = ch;
            if (parse->index < (frame->frame.header.length + 5))
            {
                parse->checksum += ch;
            }
            else
            {
                if (parse->index == (frame->frame.header.length + 5))
                {
                    if (parse->checksum != ch)
                    {
                        parse->state = STAT_SYNC;  //帧校验错误，重新接收下一帧
                        parse->error_count++;
                        return 0;
                    }
                }
                else if (parse->index == (frame->frame.header.length + 6))
                {
                    parse->state = STAT_SYNC;
                    if (ch == FRAME_END)
                    {
                        return 1;
                    }
                    else
                    {
                        parse->error_count++;
                        return 0;
                    }
                }
            }
            break;
        default:
            parse->state = STAT_SYNC;
            break;
        }
    }
    return 0;
}
