
#include "params.h"

#define PARAMS_MAGIC     0x4643

#define JOYSTICK_DEFAULT_VALUE  1000

#define EEPROM_USER_START_ADDR   0

params_t params;

static uint32_t checksum(uint8_t *data,uint16_t len)
{
    uint32_t sum = 0;
    while(len--)
    {
        sum+=*data;
        data++;
    }
    return sum;
}
//加载遥控器参数
rt_err_t params_load(params_t * params)
{
    if(EEPROM_READ(EEPROM_USER_START_ADDR,(uint8_t*)params,sizeof(params_t)) != 0)
    {
    return RT_ERROR;
    }
    return RT_EOK;
}

//保存遥控器参数
rt_err_t params_save(params_t * params)
{    
    params->checksum = checksum((uint8_t *)params,sizeof(params_t) - 4);
    EEPROM_ERASE(EEPROM_USER_START_ADDR,EEPROM_BLOCK_SIZE);
    return EEPROM_WRITE(EEPROM_USER_START_ADDR,(uint8_t *)params,sizeof(params_t));
}


//校验遥控器参数有效性
rt_err_t params_is_valid(params_t * params)
{    
    uint32_t cks;
    
    if (params->magic != PARAMS_MAGIC)
    {
        return RT_ERROR;
    }
    
    //量程有效性检查
    for (uint8_t i = 0;i < JOYSTICK_ADC_MAX_CHANNELS;i++)
    {
        if (params->min_value[i] < 100 || params->max_value[i] > 3300)
        {
            return RT_ERROR;
        }
        
        
        if (abs(params->max_value[i] - params->min_value[i]) < 500)
        {
            return RT_ERROR;
        }
    }
    
    
    cks = checksum((uint8_t *)params,sizeof(params_t) - 4);
    
    if (params->checksum != cks)
    {
        return RT_ERROR;
    }
    
    return RT_EOK;
}



//恢复遥控器参数出厂设置
void params_load_default(params_t * params)
{
    uint32_t i;
        
    params->magic = PARAMS_MAGIC;
    
    for (i = 0;i < MAX_RC_CHANNELS;i++)
    {
        if (i < JOYSTICK_ADC_MAX_CHANNELS)
        {//模拟量通道（摇杆）
            params->min_value[i] = JOYSTICK_DEFAULT_VALUE;
            params->mid_value[i] = JOYSTICK_DEFAULT_VALUE;
            params->max_value[i] = JOYSTICK_DEFAULT_VALUE;
        }
        else
        {//数字量通道（按键）
            params->min_value[i] = RADIO_RANGE_MIN_VALUE;
            params->max_value[i] = RADIO_RANGE_MAX_VALUE;
        }
    }
    
    params_save(params);
}


//初始化遥控器参数
rt_err_t params_init(void)
{
    rt_err_t ret = RT_EOK;
    
    //加载遥控器参数到rc_params
    params_load(&params);
        
    //校验遥控器参数
    if ((params_is_valid(&params)!=RT_EOK) || !params.calibraed)
    {
        //恢复到默认值
        params_load_default(&params);
        
        //置未校准状态
        params.calibraed = 0;
        
        ret = RT_ERROR;
    }
    
    return ret;
}



