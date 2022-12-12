#include "threads.h"
#include "drv_buzzer.h"
#include "radio.h"
#include "tones.h"

#define T 1000

#define TONE_THREAD_STACK_SIZE        (512)

#define TONE_PLAY_THREAD_PRIORITY      (6)

static struct rt_thread tone_play_thread;

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t tone_play_thread_stack[TONE_THREAD_STACK_SIZE];

static struct rt_event tone_event;

static rt_uint8_t tone_id = 0;

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
    if(tone_data == RT_NULL)
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
    rt_uint32_t evt = 0;
    rt_err_t ret = RT_EOK;
    
    play_tones(poweron_tones,sizeof(poweron_tones)/sizeof(tone_t),128);
    
    while(1)
    {
       ret = rt_event_recv(&tone_event, 0xffffffff,
                          RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, &evt);
        if(ret == RT_EOK)
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
    rt_event_send(&tone_event, 0x01);
}
int tone_play_thread_init(void)
{
    rt_err_t result = RT_EOK;
    
    rt_event_init(&tone_event, "tone_evt", RT_IPC_FLAG_FIFO);
    
    result = rt_thread_init(&tone_play_thread, "tone",
        tone_play_thread_entry, RT_NULL, 
        &tone_play_thread_stack[0], sizeof(tone_play_thread_stack), 
        TONE_PLAY_THREAD_PRIORITY, 5);
    
    if (result == RT_EOK)
    {
        rt_thread_startup(&tone_play_thread);
    }
    
    return 0;
}
