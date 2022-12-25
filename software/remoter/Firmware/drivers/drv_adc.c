

/* 头文件包含 */
#include "board.h"
#include "CH58x_common.h"
#include "drv_adc.h"

/**************************************************************************************************
 *                                        GLOBAL VARIABLES
 **************************************************************************************************/


/**************************************************************************************************
 *                                        FUNCTIONS - Local
 **************************************************************************************************/


static uint16_t adc_buffer[ADC_CHANNEL_MAX];
static const uint8_t adc_channel_map[ADC_CHANNEL_MAX] = { CH_EXTIN_9,CH_EXTIN_8,CH_EXTIN_7,CH_EXTIN_6,CH_EXTIN_11, CH_INTE_VTEMP};
static int16_t RoughCalib_Value;
#ifdef __RTTHREAD__
static struct rt_semaphore wait_sem;
#endif
#ifdef __FREERTOS__
static SemaphoreHandle_t wait_sem;
#endif
static uint8_t conver_index = 0;

static uint16_t ADC_SingleConver(void)
{
    R8_ADC_CONVERT = RB_ADC_START;
    while(R8_ADC_CONVERT & RB_ADC_START)
    {
        rt_thread_delay(1);
    }

    return (R16_ADC_DATA & RB_ADC_DATA);
}

/**************************************************************************************************
 * @fn      adc_hw_init
 *
 * @brief   Initilize ADC Service
 *
 * @param   none
 *
 * @return  None
 **************************************************************************************************/
void adc_hw_init(void)
{
    GPIOA_ModeCfg(GPIO_Pin_0, GPIO_ModeIN_Floating);
    GPIOA_ModeCfg(GPIO_Pin_1, GPIO_ModeIN_Floating);
    GPIOA_ModeCfg(GPIO_Pin_2, GPIO_ModeIN_Floating);
    GPIOA_ModeCfg(GPIO_Pin_3, GPIO_ModeIN_Floating);
    GPIOA_ModeCfg(GPIO_Pin_7, GPIO_ModeIN_Floating);
    ADC_ExtSingleChSampInit(SampleFreq_8, ADC_PGA_1_2);
    RoughCalib_Value = ADC_DataCalib_Rough(); // 用于计算ADC内部偏差，记录到全局变量 RoughCalib_Value中
#ifdef __RTTHREAD__
    rt_sem_init(&wait_sem, "adc_wait", 0x00, RT_IPC_FLAG_FIFO);
#endif
#ifdef __FREERTOS__
    wait_sem = xSemaphoreCreateBinary();
#endif
    PFIC_EnableIRQ(ADC_IRQn);
}



/**************************************************************************************************
 * @fn      HalKeyRead
 *
 * @brief   Read the current value of a key
 *
 * @param   None
 *
 * @return  keys - current keys status
 **************************************************************************************************/
uint16_t adc_read(uint8_t ch)
{
    uint16_t adc_value = 0;

    if(ch < ADC_CHANNEL_MAX)
    {
    adc_value = adc_buffer[ch] + RoughCalib_Value;
    }
    return adc_value;
}

void adc_multi_convert(void)
{
    conver_index = 0;
    ADC_ChannelCfg(adc_channel_map[conver_index]);
    ADC_ClearITFlag();
    ADC_StartUp();
//    for (conver_index = 0; conver_index < ADC_CHANNEL_MAX; ++conver_index) {
//        ADC_ChannelCfg(adc_channel_map[conver_index]);
//        adc_buffer[conver_index] = ADC_SingleConver();
//    }
#ifdef __RTTHREAD__
    rt_sem_take(&wait_sem, 50);
#endif
#ifdef __FREERTOS__
    xSemaphoreTake(wait_sem, 50);
#endif
}

/*********************************************************************
 * @fn      ADC_IRQHandler
 *
 * @brief   ADC中断函数
 *
 * @return  none
 */
__HIGH_CODE
void ADC_IRQHandler(void) //adc中断服务程序
{
#ifdef __FREERTOS__
    portBASE_TYPE xHigherPriorityTaskWoken = 0;
#endif
    if(ADC_GetITStatus())
    {
        ADC_ClearITFlag();
        if(conver_index < ADC_CHANNEL_MAX)
        {
            adc_buffer[conver_index] = ADC_ReadConverValue();
            conver_index++;
            if(conver_index < ADC_CHANNEL_MAX)
            {
            ADC_ChannelCfg(adc_channel_map[conver_index]);
            ADC_StartUp(); // 作用清除中断标志并开启新一轮采样
            }
            else
            {
#ifdef __RTTHREAD__
             rt_sem_release(&wait_sem);
#endif
#ifdef __FREERTOS__
             xSemaphoreGiveFromISR( wait_sem, &xHigherPriorityTaskWoken );
             portYIELD_FROM_ISR(xHigherPriorityTaskWoken);   /* 根据需要发起切换请求 */
#endif
            }
        }
    }
}
