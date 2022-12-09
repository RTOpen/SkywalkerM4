

/* 头文件包含 */
#include "rtthread.h"
#include "CH58x_common.h"
#include "drv_adc.h"

/**************************************************************************************************
 *                                        GLOBAL VARIABLES
 **************************************************************************************************/


/**************************************************************************************************
 *                                        FUNCTIONS - Local
 **************************************************************************************************/
#define ADC_OVERSAMPLE_TIMES (4)
static uint16_t adc_buffer[ADC_OVERSAMPLE_TIMES];
static int16_t RoughCalib_Value;
static struct rt_semaphore wait_sem;
volatile uint8_t DMA_end = 0;
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

    rt_sem_init(&wait_sem, "adc_wait", 0x00, RT_IPC_FLAG_FIFO);
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
    int i = 0;
    switch(ch)
    {
    case ADC_CHANNEL_1:
        ADC_ChannelCfg(9);
        break;
    case ADC_CHANNEL_2:
        ADC_ChannelCfg(8);
        break;
    case ADC_CHANNEL_3:
        ADC_ChannelCfg(7);
        break;
    case ADC_CHANNEL_4:
        ADC_ChannelCfg(6);
        break;
    case ADC_CHANNEL_5:
        ADC_ChannelCfg(11);
        break;
    case ADC_CHANNEL_TEMP:
        ADC_InterTSSampInit();
        break;
    default:
        return adc_value;
    }

      ADC_ExtSingleChSampInit(SampleFreq_8, ADC_PGA_1_2);

//    //ADC_ExtSingleChSampInit(SampleFreq_8, ADC_PGA_0);
//    //ADC_AutoConverCycle(192); // 采样周期为 (256-192)*16个系统时钟
//    ADC_DMACfg(ENABLE, (uint16_t)(uint32_t)&adc_buffer[0], (uint16_t)(uint32_t)&adc_buffer[ADC_OVERSAMPLE_TIMES], ADC_Mode_Single);
//    PFIC_EnableIRQ(ADC_IRQn);
//    ADC_StartDMA();

//    ADC_InterTSSampInit();
//        for(i = 0; i < 20; i++)
//        {
//            abcBuff[i] = ADC_ExcutSingleConver(); // 连续采样20次
//        }
//        for(i = 0; i < 20; i++)
//        {
//            PRINT("%d \n", adc_to_temperature_celsius(abcBuff[i]));
//        }
//
//        GPIOA_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_Floating);
//        ADC_ExtSingleChSampInit(SampleFreq_3_2, ADC_PGA_0);
//
//        RoughCalib_Value = ADC_DataCalib_Rough(); // 用于计算ADC内部偏差，记录到全局变量 RoughCalib_Value中
//
//
//    /* DMA单通道采样：选择adc通道0做采样，对应 PA4引脚 */
//    GPIOA_ModeCfg(GPIO_Pin_4, GPIO_ModeIN_Floating);
//    ADC_ExtSingleChSampInit(SampleFreq_3_2, ADC_PGA_0);
//    ADC_ChannelCfg(0);
//    ADC_AutoConverCycle(192); // 采样周期为 (256-192)*16个系统时钟
//    ADC_DMACfg(ENABLE, (uint16_t)(uint32_t)&abcBuff[0], (uint16_t)(uint32_t)&abcBuff[40], ADC_Mode_Single);
//    PFIC_EnableIRQ(ADC_IRQn);
//    ADC_StartDMA();
//
//    while(!DMA_end);
//    DMA_end = 0;
    adc_value = ADC_ExcutSingleConver() + RoughCalib_Value;
    //rt_sem_take(&wait_sem, RT_WAITING_FOREVER);
    //adc_value = (abcBuff[0] + abcBuff[1] + abcBuff[2] + abcBuff[3])/4;

    return adc_value;
}

/*********************************************************************
 * @fn      ADC_IRQHandler
 *
 * @brief   ADC中断函数
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void ADC_IRQHandler(void) //adc中断服务程序
{
    if(ADC_GetDMAStatus())
    {
        ADC_StopDMA();
        R16_ADC_DMA_BEG = ((uint32_t)adc_buffer) & 0xffff;
        ADC_ClearDMAFlag();
        DMA_end = 1;
        rt_sem_release(&wait_sem);
    }
}
