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
  TONE_NONE=0x00,         //��
  TONE_POWER_ON,         //��������
  TONE_POWER_OFF,        //�ػ�����
  TONE_BTN_CLICK,        //��������
  TONE_WARNING,          //��������
  TONE_CALIBRA_STEP1,    //У׼����
  TONE_CALIBRA_STEP2,    //У׼����
  TONE_CALIBRA_STEP3,    //У׼����
}tone_id_t;

extern void tones_play_background(uint8_t id);

extern radio_data_t radio;

#endif /* __RADIO_H__ */
