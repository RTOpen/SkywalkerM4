#include "board.h"
#include "drv_buzzer.h"

#define SET_FREQ(BUZZER_FREQ) (80000000 / (256*BUZZER_FREQ))

static uint8_t buzzer_volume = 128;

void buzzer_hw_init(void)
{
    /* 配置GPIO */
    GPIOB_ModeCfg(GPIO_Pin_0, GPIO_ModeOut_PP_5mA);  // PB0 - PWM6
    PWMX_CLKCfg(40000/256);                                   // cycle = 4/Fsys
    PWMX_CycleCfg(PWMX_Cycle_256);                     // 周期 = 256*cycle
    PWMX_ACTOUT(CH_PWM6, 128, Low_Level, DISABLE);  // 50% 占空比
}

void buzzer_set_volume(uint8_t volume)
{
    buzzer_volume = volume;
}

void buzzer_set_freq(uint16_t freq)
{
    PWMX_ACTOUT(CH_PWM6, buzzer_volume, Low_Level, DISABLE);
    PWMX_CLKCfg(SET_FREQ(freq));
    PWMX_CycleCfg(PWMX_Cycle_256);           // 周期 = 256*cycle
}
void buzzer_start(void)
{
    PWMX_ACTOUT(CH_PWM6, buzzer_volume, Low_Level, ENABLE);
}

void buzzer_stop(void)
{
    PWMX_ACTOUT(CH_PWM6, buzzer_volume, Low_Level, DISABLE);
}
