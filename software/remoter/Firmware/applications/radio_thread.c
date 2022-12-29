#include "threads.h"
#include "radio.h"
#include "params.h"
#include "button.h"
#include "usbd_cdc_acm_hid.h"

#define JOYSTICK_CALIBRA_NONE     (0x00)
#define JOYSTICK_CALIBRA_STEP1    (0x01)
#define JOYSTICK_CALIBRA_STEP2    (0x02)
#define JOYSTICK_CALIBRA_STEP3    (0x03)

static struct rt_thread radio_thread;

static rt_uint8_t radio_thread_stack[RADIO_THREAD_STACK_SIZE];

radio_data_t radio;
static uint16_t channels[CHANNEL_MAX]={0};
static uint8_t HID_Buffer[19];
static button_t buttons[KEY_SW_NUM];

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

static void btn_clicked_callback(void*param)
{
    button_t *btn = (button_t*)param;
    RT_ASSERT(btn != RT_NULL);
    tones_play_background(TONE_BTN_CLICK);

}

static void btn_double_clicked_callback(void*param)
{
    button_t *btn = (button_t*)param;
    RT_ASSERT(btn != RT_NULL);
}

static void btn_long_press_callback(void*param)
{
    button_t *btn = (button_t*)param;
    RT_ASSERT(btn != RT_NULL);
    tones_play_background(TONE_BTN_LONG_CLICK);
    switch(btn->key_id)
    {
    case KEY_SW_HOME:
        break;
    case KEY_SW_LEFT:
        break;
    case KEY_SW_RIGHT:
        radio.calibra_step = JOYSTICK_CALIBRA_STEP1;
        params.calibraed = 0;
        break;
    case KEY_SW_POWER:
        break;
    }
}

static void radio_thread_entry(void *parameter)
{
    uint32_t timeout = 0;
    if(!params.calibraed)
    {
        radio.calibra_step = JOYSTICK_CALIBRA_STEP1;
    }
    else
    {
        radio.calibra_step = JOYSTICK_CALIBRA_NONE;
    }

    for (int i = 0; i < KEY_SW_NUM; ++i) {
        button_init(&buttons[i], (0x01 << i));
        button_attach(&buttons[i], SINGLE_CLICK, btn_clicked_callback);
        button_attach(&buttons[i], DOUBLE_CLICK, btn_double_clicked_callback);
        button_attach(&buttons[i], LONG_RRESS_START, btn_long_press_callback);
        button_start(&buttons[i]);
    }


    rt_thread_delay(RT_TICK_PER_SECOND);

    while(1)
    {
        adc_multi_convert();
        channels[ROLL] = adc_read(ADC_CHANNEL_3);
        channels[PITCH] = adc_read(ADC_CHANNEL_4);
        channels[YAW] = adc_read(ADC_CHANNEL_2);
        channels[THROTTLE] = adc_read(ADC_CHANNEL_1);
        channels[AUX1] = adc_read(ADC_CHANNEL_5);
        channels[AUX2] = encoder_read_pos(ROTARY_ENCODER_A);// = adc_read(ADC_CHANNEL_TEMP);
        channels[AUX3] = encoder_read_pos(ROTARY_ENCODER_B);
        channels[AUX4]++;
        if(radio.calibra_step != JOYSTICK_CALIBRA_NONE)
        {
            switch(radio.calibra_step)
            {
              case JOYSTICK_CALIBRA_STEP1:
                  if(timeout==0)
                  {
                      timeout = rt_tick_get();
                      for (int i = 0; i < JOYSTICK_ADC_MAX_CHANNELS; ++i) {
                          params.min_value[i] = RADIO_RANGE_MIN_VALUE;
                          params.max_value[i] = RADIO_RANGE_MAX_VALUE;
                      }
                  }
                  for (int i = 0; i < JOYSTICK_ADC_MAX_CHANNELS; ++i) {
                      radio_update_range_min_max(i,channels[i]);
                  }
                  if(rt_tick_get() - timeout > 5*RT_TICK_PER_SECOND)
                  {
                      BOOL finish = TRUE;
                      for (int i = 0; i < JOYSTICK_ADC_MAX_CHANNELS; ++i) {
                          if(params.max_value[i] - params.min_value[i] <1200)
                            {
                              finish = FALSE;
                              break;
                            }
                            }
                      if(finish == TRUE)
                      {
                      radio.calibra_step++;
                      timeout = 0;
                      }
                  }
                  break;
              case JOYSTICK_CALIBRA_STEP2:
                  if(timeout==0)
                  {
                      timeout = rt_tick_get();
                  }
                  if(rt_tick_get() - timeout > 2*RT_TICK_PER_SECOND)
                   {
                  for (int i = 0; i < JOYSTICK_ADC_MAX_CHANNELS; ++i) {
                      radio_update_range_mid(i,channels[i]);
                  }
                   }
                  if(rt_tick_get() - timeout > 5*RT_TICK_PER_SECOND)
                   {
                      BOOL finish = TRUE;
                      for (int i = 0; i < JOYSTICK_ADC_MAX_CHANNELS; ++i) {
                          if(params.max_value[i] - params.min_value[i] <1200)
                            {
                              finish = FALSE;
                              break;
                            }
                            }
                      if(finish == TRUE)
                      {
                      radio.calibra_step++;
                      timeout = 0;
                      }
                   }
                  break;
              case JOYSTICK_CALIBRA_STEP3:
                  params.calibraed = 1;
                  params_save(&params);
                  radio.calibra_step = JOYSTICK_CALIBRA_NONE;
                  break;
            }

        }
        else
        {
            for (int i = 0; i < 4; ++i) {
                if((i==ROLL)||(i==THROTTLE))
                {
                    radio.channels[i] = 3000 - radio_data_normalize(channels[i],params.min_value[i],params.mid_value[i],params.max_value[i],1000,1500,2000);
                }else {
                    radio.channels[i] = radio_data_normalize(channels[i],params.min_value[i],params.mid_value[i],params.max_value[i],1000,1500,2000);
                }

            }
            for (int i = 4; i < CHANNEL_MAX; ++i) {
                radio.channels[i] = channels[i];
            }
                    for (int i = 0; i < CHANNEL_MAX; ++i) {
                      int16_t value = radio_data_normalize(radio.channels[i],1000,1500,2000,0,1023,2047);
                      HID_Buffer[i*2 +3] = (value & 0xFF);
                      HID_Buffer[i*2 +4] = ((value >> 8) & 0x07);
                    }
                    usbd_hid_send_report(HID_Buffer,19);
        }
     button_process(key_scan());
     rt_thread_delay(20);
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

    return result;
}

