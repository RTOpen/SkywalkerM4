#ifndef __RADIO_H__
#define __RADIO_H__
#include <rtthread.h>
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
 uint8_t poweron_flag : 1;
} radio_data_t;

extern radio_data_t radio;

#endif /* __RADIO_H__ */
