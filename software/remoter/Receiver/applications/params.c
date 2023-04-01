
#include "params.h"

#define PARAMS_MAGIC     0x4643


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
//����ң��������
rt_err_t params_load(params_t * params)
{
    if(EEPROM_READ(EEPROM_USER_START_ADDR,(uint8_t*)params,sizeof(params_t)) != 0)
    {
    return RT_ERROR;
    }
    return RT_EOK;
}

//����ң��������
rt_err_t params_save(params_t * params)
{    
    params->checksum = checksum((uint8_t *)params,sizeof(params_t) - 4);
    EEPROM_ERASE(EEPROM_USER_START_ADDR,EEPROM_BLOCK_SIZE);
    return EEPROM_WRITE(EEPROM_USER_START_ADDR,(uint8_t *)params,sizeof(params_t));
}


//У��ң����������Ч��
rt_err_t params_is_valid(params_t * params)
{    
    uint32_t cks;
    
    if (params->magic != PARAMS_MAGIC)
    {
        return RT_ERROR;
    }
    
    
    
    cks = checksum((uint8_t *)params,sizeof(params_t) - 4);
    
    if (params->checksum != cks)
    {
        return RT_ERROR;
    }
    
    return RT_EOK;
}



//�ָ�ң����������������
void params_load_default(params_t * params)
{
    uint32_t i;
        
    params->magic = PARAMS_MAGIC;
    
    
    params_save(params);
}


//��ʼ��ң��������
rt_err_t params_init(void)
{
    rt_err_t ret = RT_EOK;
    
    //����ң����������rc_params
    params_load(&params);
        
    //У��ң��������
    if (params_is_valid(&params)!=RT_EOK)
    {
        //�ָ���Ĭ��ֵ
        params_load_default(&params);
        
        
        ret = RT_ERROR;
    }
    
    return ret;
}



