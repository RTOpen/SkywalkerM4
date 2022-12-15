#ifndef __PARAMS_H__
#define __PARAMS_H__

#include "board.h"

#define MAX_RC_CHANNELS     (8)  //ң�������ͨ����
#define JOYSTICK_ADC_MAX_CHANNELS  4
#define RADIO_RANGE_MAX_VALUE  3500 //ң�����������ֵ
#define RADIO_RANGE_MID_VALUE  2500 //ң���������м�ֵ
#define RADIO_RANGE_MIN_VALUE  1500 //ң����������Сֵ

#define RC_MODE_AMERICAN  0  //������
#define RC_MODE_JAPAN     1  //�ձ���


//��Ƶ���г���
#define HOPPING_LENGTH    8


typedef struct{
        uint16_t magic;
        uint8_t  mode;      //ҡ��ģʽ
        uint8_t  calibraed; //�Ƿ���У׼
        uint16_t  product_id; //��ƷID
        uint16_t min_value[MAX_RC_CHANNELS];//��Сֵ
        uint16_t mid_value[MAX_RC_CHANNELS];//�м�λ��
        uint16_t max_value[MAX_RC_CHANNELS];//���ֵ
        uint32_t checksum;
} params_t;


extern params_t params;

extern rt_err_t params_load(params_t * params);
extern rt_err_t params_save(params_t * params);
extern void params_load_default(params_t * params);
extern rt_err_t params_init(void);

#endif /* __PARAMS_H__ */
