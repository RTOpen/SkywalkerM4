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

typedef enum
{
  TONE_NONE=0x00,         //无
  TONE_POWER_ON,         //开机声音
  TONE_POWER_OFF,        //关机声音
  TONE_BTN_CLICK,        //按键声音
  TONE_WARNING,          //警告声音
  TONE_CALIBRA_STEP1,    //校准声音
  TONE_CALIBRA_STEP2,    //校准声音
  TONE_CALIBRA_STEP3,    //校准声音
}tone_id_t;

extern void tones_play_background(uint8_t id);

extern radio_data_t radio;

#endif /* __RADIO_H__ */
