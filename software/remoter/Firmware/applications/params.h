#ifndef __PARAMS_H__
#define __PARAMS_H__

#include "board.h"

#define MAX_RC_CHANNELS     (8)  //遥控器最大通道数
#define JOYSTICK_ADC_MAX_CHANNELS  4
#define RADIO_RANGE_MAX_VALUE  3500 //遥控器量程最大值
#define RADIO_RANGE_MID_VALUE  2500 //遥控器量程中间值
#define RADIO_RANGE_MIN_VALUE  1500 //遥控器量程最小值

#define RC_MODE_AMERICAN  0  //美国手
#define RC_MODE_JAPAN     1  //日本手


//跳频序列长度
#define HOPPING_LENGTH    8


typedef struct{
        uint16_t magic;
        uint8_t  mode;      //摇杆模式
        uint8_t  calibraed; //是否已校准
        uint16_t  product_id; //产品ID
        uint16_t min_value[MAX_RC_CHANNELS];//最小值
        uint16_t mid_value[MAX_RC_CHANNELS];//中间位置
        uint16_t max_value[MAX_RC_CHANNELS];//最大值
        uint32_t checksum;
} params_t;


extern params_t params;

extern rt_err_t params_load(params_t * params);
extern rt_err_t params_save(params_t * params);
extern void params_load_default(params_t * params);
extern rt_err_t params_init(void);

#endif /* __PARAMS_H__ */
