#include "threads.h"
#include "radio.h"
#include "params.h"

#define RADIO_THREAD_STACK_SIZE        (1024)

#define RADIO_THREAD_PRIORITY          (7)



static struct rt_thread radio_thread;

static rt_uint8_t radio_thread_stack[RADIO_THREAD_STACK_SIZE];

radio_data_t radio;
static uint16_t channels[CHANNEL_MAX]={0};

void radio_update_range_min_max(uint8_t chan,uint16_t value)
{
    params.min_value[chan] = params.min_value[chan] > value ? value : params.min_value[chan];
    params.max_value[chan] = params.max_value[chan] < value ? value : params.max_value[chan];
}

void radio_update_range_mid(uint8_t chan,uint16_t value)
{
    params.mid_value[chan] = value;
}


static uint16_t radio_data_normalize(uint16_t value,uint16_t in_min_val,uint16_t in_mid_val,uint16_t in_max_val,uint16_t out_min_val,uint16_t out_mid_val,uint16_t out_max_val)
{
    uint16_t value_out;

    if (value > in_max_val)
        value = in_max_val;

    if (value < in_min_val)
        value = in_min_val;

    if (value >= in_mid_val)
    {
        value_out = out_mid_val + (value - in_mid_val) * (out_max_val - out_mid_val) / (in_max_val - in_mid_val);
    }
    else
    {
        value_out = out_min_val + (value - in_min_val) * (out_mid_val - out_min_val) / (in_mid_val - in_min_val);
    }

    return value_out;
}

static void radio_thread_entry(void *parameter)
{

    while(1)
    {
        adc_multi_convert();
        channels[ROLL] = adc_read(ADC_CHANNEL_4);
        channels[PITCH] = adc_read(ADC_CHANNEL_3);
        channels[YAW] = adc_read(ADC_CHANNEL_2);
        channels[THROTTLE] = adc_read(ADC_CHANNEL_1);
        channels[AUX1] = adc_read(ADC_CHANNEL_5);
        channels[AUX2] = encoder_read_pos(ROTARY_ENCODER_A);// = adc_read(ADC_CHANNEL_TEMP);
        channels[AUX3] = encoder_read_pos(ROTARY_ENCODER_B);
        channels[AUX4]++;
        if(radio.calibra_step > 0)
        {
            switch(radio.calibra_step)
            {
              case 1:
                  for (int i = 0; i < JOYSTICK_ADC_MAX_CHANNELS; ++i) {
                      radio_update_range_min_max(i,channels[i]);
                  }
                  break;
              case 2:
                  for (int i = 0; i < JOYSTICK_ADC_MAX_CHANNELS; ++i) {
                      radio_update_range_mid(i,channels[i]);
                  }
                  break;
              case 3:
                  params_save(&params);
                  break;
            }

        }
        else
        {
            for (int i = 0; i < 4; ++i) {
                radio.channels[i] = radio_data_normalize(channels[i],params.min_value[i],params.mid_value[i],params.max_value[i],1000,1500,2000);
            }
        }

     rt_thread_delay(100);
    }
}


int radio_thread_init(void)
{
    rt_err_t result = RT_EOK;
    
    
    result = rt_thread_init(&radio_thread, "radio",
        radio_thread_entry, RT_NULL, 
        &radio_thread_stack[0], sizeof(radio_thread_stack), 
        RADIO_THREAD_PRIORITY, 5);
    
    if (result == RT_EOK)
    {
        rt_thread_startup(&radio_thread);
    }
    
    return 0;
}
INIT_APP_EXPORT(radio_thread_init);
