#ifndef __RADIO_H__
#define __RADIO_H__
#include "board.h"
#include <stdint.h>

typedef enum
{
    ROLL = 0,
    PITCH,
    YAW,
    THROTTLE,
    AUX1,
    AUX2,
    AUX3,
    AUX4,
    CHANNEL_MAX,
}radio_channel_t;

typedef struct
{
 int16_t channels[CHANNEL_MAX];
 uint8_t battery_level;
 uint8_t calibra_step;
 uint8_t failsafe;
 uint8_t mac_addr[6];
 uint32_t last_update;
 uint8_t nrf_channel;
} radio_data_t;

#define NORM_MIN_VALUE  1000
#define NORM_MID_VALUE  1500
#define NORM_MAX_VALUE  2000

extern radio_data_t radio;

#endif /* __RADIO_H__ */
