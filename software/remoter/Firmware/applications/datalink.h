#ifndef __DATALINK_H__
#define __DATALINK_H__
#include <stdint.h>

#define DLINK_MAX_CHAN            (2)
#define DLINK_MAX_FRAME_LEN       (64)

#define FRAME_START  (0xAA)
#define FRAME_END    (0x55)

typedef enum
{
    CMD_MODULE_HEARTBEAT = 0x00,

    CMD_GET_MODULE_INFO = 0x01,
	
    CMD_GET_STATUS = 0x02,
    
	CMD_GET_NETWORK_CONFIG = 0x03,

    CMD_SET_NETWORK_CONFIG = 0x04,
    
    CMD_RW_I8080 = 0x05,
    
    CMD_IRQ_I8080_DATA = 0x06,
    
    CMD_RESTORE_FACTRY = 0xf0,
    
    CMD_DEVICE_RESET = 0xf1,
    
    CMD_UPGRADE_REQ = 0xfb,

    CMD_UPGRADE_INFO = 0xfc,

    CMD_UPGRADE_PACK = 0xfd,

    CMD_UPGRADE_FINISH = 0xfe,

    CMD_DISCOVERY = 0xff,

} command_t;

#pragma pack (1)

typedef struct
{
    uint8_t start;
    uint8_t cmd;
    uint16_t length ;
} frame_header_t;

typedef struct
{
    frame_header_t header;
    uint8_t data[DLINK_MAX_FRAME_LEN - 4];
} frame_t;

typedef union
{
    uint8_t raw_data[DLINK_MAX_FRAME_LEN];
    frame_t frame;
} lcs_data_frame_t;

typedef struct
{
    uint8_t id[12];
    uint32_t hw_version;
    uint32_t sw_version;
} module_info_t;

typedef struct
{
    uint32_t runtime;
    uint32_t cpu_freq;
    uint32_t cpu_load;
    uint32_t ram_used;
    uint32_t temperature;
    uint32_t eth_tx;
    uint32_t eth_rx;
    uint8_t  eth_mode;
} device_status_t;

typedef struct
{
    uint32_t ipaddr;
    uint32_t netmask;
    uint32_t gateway;
    uint32_t dns;
    uint8_t mac_addr[6];
    uint8_t  dhcp_enble;
} network_config_t;

typedef struct
{
   uint8_t action; //操作类型，0x00为写操作，0x01为读操作
   uint8_t addr;  //读写地址，0x00-0xff
   uint16_t data; //读写的数据，如果是读操作，上位机发送为0
} bus_msg_t;

#pragma pack ()

#define BUS_ACTION_WRITE 0x00
#define BUS_ACTION_READ  0x01

typedef struct
{
    uint8_t type;
    uint16_t count;
    bus_msg_t *msgs;
    uint32_t fd;
} recv_msg_data_t;

#define RECV_FROM_UDP    0x00
#define RECV_FROM_TCP    0x01

uint8_t LCS_ParseChar(uint8_t chan, lcs_data_frame_t *frame, uint8_t ch);

uint16_t LCS_ModuleInfoFramePack(lcs_data_frame_t *frame, uint8_t *id, uint32_t hw_version, uint32_t sw_version);

uint16_t LCS_ModuleInfoToFrame(lcs_data_frame_t *frame, module_info_t *module_info);

uint16_t LCS_DeviceStatusToFrame(lcs_data_frame_t *frame, device_status_t *status);

uint16_t LCS_NetworkConfigToFrame(lcs_data_frame_t *frame, network_config_t *config);

uint16_t LCS_BusDataToFrame(lcs_data_frame_t *frame, bus_msg_t *msg,uint16_t count);

uint16_t LCS_BusIrqDataToFrame(lcs_data_frame_t *frame, bus_msg_t *msg,uint16_t count);

uint16_t LCS_NetworkConfigResultPack(lcs_data_frame_t *frame, uint8_t result);

uint16_t LCS_UpgradeRequestPack(lcs_data_frame_t *frame, uint32_t sw_version, uint32_t hw_version);

uint16_t LCS_UpgradeInfoAckPack(lcs_data_frame_t *frame, uint8_t ack);

uint16_t LCS_UpgradeDataPack(lcs_data_frame_t *frame, uint16_t index);

uint16_t LCS_UpgradeFinish(lcs_data_frame_t *frame, uint8_t result);

uint16_t LCS_DiscoveryPack(lcs_data_frame_t *frame, uint8_t *mac, uint32_t sw_ver, uint32_t build_date);

#endif  /* endif __PROTOCOL_H__ */
