#include "threads.h"
#include "drv_buzzer.h"
#include "radio.h"
#include "tones.h"

#define T 1000
#ifdef __RTTHREAD__
static struct rt_thread tone_play_thread;

static uint8_t tone_play_thread_stack[TONE_THREAD_STACK_SIZE];

static struct rt_event tone_event;
#endif
#ifdef __FREERTOS__
static TaskHandle_t tone_play_thread;
SemaphoreHandle_t tone_event;
#endif

static uint8_t tone_id = 0;

static const tone_t poweron_tones[] = 
    {
    {C5,T/16},
    {D5,T/16},
    {E5,T/16},
    {F5,T/16},
    };
static const tone_t poweroff_tones[] = 
    {
    {F5,T/16},
    {E5,T/16},
    {D5,T/16},
    {C5,T/16},
    };
static const tone_t btn_click_tones[] = 
{
    {F5,T/16},
};

static const tone_t warning_tones[] = 
{
    {F5,T/16},
};

static const tone_t calibra_step1_tones[] = 
{
    {C5,T/8},
};
  
static const tone_t calibra_step2_tones[] = 
{
    {D5,T/8},
    {D5,T/8},
};
      
static const tone_t calibra_step3_tones[] = 
{
    {E5,T/8},
    {E5,T/8},
    {E5,T/8},
    {E5,T/8},
    {E5,T/8},
};


static void play_tones(const tone_t *tones, uint16_t count, uint8_t volume)
{
    tone_t *tone_data = (tone_t *)tones;
    if(tone_data == NULL)
    {
     return;
    }
    buzzer_set_volume(volume);
    while(count--)
    {
    buzzer_set_freq(tone_data->note);
    buzzer_start();
    rt_thread_delay(tone_data->duration); 
    buzzer_stop();
    rt_thread_delay(10);
    tone_data++;
    }
}

static void tone_play_thread_entry(void *parameter)
{
    uint32_t evt = 0;
    rt_err_t ret = RT_EOK;
    
    play_tones(poweron_tones,sizeof(poweron_tones)/sizeof(tone_t),128);
    
    while(1)
    {
#ifdef __RTTHREAD__
       ret = rt_event_recv(&tone_event, 0xffffffff,
                          RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, &evt);
        if(ret == RT_EOK)
#endif
#ifdef __FREERTOS__
       if(xSemaphoreTake(tone_event, portMAX_DELAY) == pdTRUE)
#endif
        {
        switch(tone_id)
        {
        case TONE_POWER_ON:
            play_tones(poweron_tones,sizeof(poweron_tones)/sizeof(tone_t),128);
            break;
        case TONE_POWER_OFF:
            play_tones(poweroff_tones,sizeof(poweroff_tones)/sizeof(tone_t),128);
            break;
        case TONE_BTN_CLICK:
            play_tones(btn_click_tones,sizeof(btn_click_tones)/sizeof(tone_t),128);
            break;
        case TONE_WARNING:
            play_tones(warning_tones,sizeof(warning_tones)/sizeof(tone_t),128);
            break;
        
        case TONE_CALIBRA_STEP1:
            play_tones(calibra_step1_tones,sizeof(calibra_step1_tones)/sizeof(tone_t),128);
            break;
        case TONE_CALIBRA_STEP2:
            play_tones(calibra_step2_tones,sizeof(calibra_step2_tones)/sizeof(tone_t),128);
            break;
        case TONE_CALIBRA_STEP3:
            play_tones(calibra_step3_tones,sizeof(calibra_step3_tones)/sizeof(tone_t),128);
            break;
        default:
            break;
        }
        }
     

    }
}

void tones_play_background(uint8_t id)
{
    tone_id = id;
#ifdef __RTTHREAD__
    rt_event_send(&tone_event, 0x01);
#endif
#ifdef __FREERTOS__
    xSemaphoreGive(tone_event);
#endif
}
int tone_play_thread_init(void)
{
    rt_err_t result = RT_EOK;
#ifdef __RTTHREAD__
    rt_event_init(&tone_event, "tone_evt", RT_IPC_FLAG_FIFO);
    
    result = rt_thread_init(&tone_play_thread, "tone",
        tone_play_thread_entry, RT_NULL, 
        &tone_play_thread_stack[0], sizeof(tone_play_thread_stack), 
        TONE_PLAY_THREAD_PRIORITY, 5);
    
    if (result == RT_EOK)
    {
        rt_thread_startup(&tone_play_thread);
    }
#endif
#ifdef __FREERTOS__
    tone_event = xSemaphoreCreateBinary();
    if(tone_event != NULL)
    {

    /* create three task */
    xTaskCreate((TaskFunction_t)tone_play_thread_entry,
                (const char *)"tone",
                (uint16_t)TONE_THREAD_STACK_SIZE/4,
                (void *)NULL,
                (UBaseType_t)(configMAX_PRIORITIES - TONE_PLAY_THREAD_PRIORITY - 1),
                (TaskHandle_t *)&tone_play_thread);
    }
#endif
    
    return 0;
}
