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
 uint32_t last_update;
 uint8_t battery_level;
 uint8_t rssi;
 uint8_t poweron_flag : 1;
 uint8_t linkup_flag:1;
 uint8_t bind_flag:1;
 uint8_t calibra_step;
 uint8_t flight_mode;
 uint8_t new_mode;
} radio_data_t;

typedef enum
{
  TONE_NONE=0x00,         //无
  TONE_POWER_ON,         //开机声音
  TONE_POWER_OFF,        //关机声音
  TONE_BTN_CLICK,        //按键短按声音
  TONE_BTN_LONG_CLICK,   //按键长按声音
  TONE_WARNING,          //警告声音
  TONE_CALIBRA_STEP1,    //校准声音
  TONE_CALIBRA_STEP2,    //校准声音
  TONE_CALIBRA_STEP3,    //校准声音
}tone_id_t;

typedef enum
{
  FLIGHT_MODE_STAB=0x00,
  FLIGHT_MODE_ALT_HOLD,
  FLIGHT_MODE_LOITER,
  FLIGHT_MODE_USER,
}flight_mode_t;

#define NORM_MIN_VALUE  1000
#define NORM_MID_VALUE  1500
#define NORM_MAX_VALUE  2000


extern void tones_play_background(uint8_t id);

extern radio_data_t radio;

#endif /* __RADIO_H__ */
