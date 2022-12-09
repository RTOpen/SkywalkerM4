#ifndef __PARAMS_H__
#define __PARAMS_H__

#include "board.h"

#define MAX_RC_CHANNELS     (8)  //ң�������ͨ����
#define JOYSTICK_ADC_MAX_CHANNELS (4)
#define RC_RANGE_MAX_VALUE  2000 //ң�����������ֵ
#define RC_RANGE_MID_VALUE  1000 //ң���������м�ֵ
#define RC_RANGE_MIN_VALUE  0    //ң����������Сֵ

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
}__attribute__ ((packed))params_t;


extern params_t params;


extern void params_update_range_min_max(uint8_t chan,uint16_t value);
extern void params_update_range_mid(uint8_t chan,uint16_t value);
extern rt_err_t params_load(params_t * params);
extern rt_err_t params_save(params_t * params);
extern void params_load_default(params_t * params);
extern rt_err_t params_init(void);

#endif /* __PARAMS_H__ */
