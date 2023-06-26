#ifndef __PARAMS_H__
#define __PARAMS_H__

#include "board.h"


//跳频序列长度
#define HOPPING_LENGTH    8


typedef struct{
        uint32_t magic;
        uint8_t  mode;      //摇杆模式
        uint8_t  bind;      //是否已校准
        uint16_t  product_id; //产品ID

        uint32_t checksum;
} params_t;


extern params_t params;

extern rt_err_t params_load(params_t * params);
extern rt_err_t params_save(params_t * params);
extern void params_load_default(params_t * params);
extern rt_err_t params_init(void);

#endif /* __PARAMS_H__ */
