#include "board.h"
#include <rtthread.h>
#include "threads.h"
#include "usbd_cdc_acm_hid.h"
#include "radio.h"
#include "params.h"

/*********************************************************************
 * @fn      main
 *
 * @brief   Ö÷º¯Êý
 *
 * @note    main is one of threads in rt-thread.
 *
 * @return  none
 */
int main()
{
    static uint8_t HID_Buffer[19];
    uint16_t channels[CHANNEL_MAX]={0};
    params_init();
    adc_hw_init();
    //cdc_acm_hid_init();
    led_hw_init();
    lcd_hw_init();
    //buzzer_hw_init();
    encoder_hw_init();
    tone_play_thread_init();
    led_thread_init();
    ui_thread_init();

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
        for (int i = 0; i < CHANNEL_MAX; ++i) {
            radio.channels[i] = channels[i];
        }
        //buttons
        HID_Buffer[0] = 0;
        HID_Buffer[1] = 0;
        HID_Buffer[2] = 0;
//        for (int i = 0; i < 8; ++i) {
//          if ( channels[i+8] > 0 ) {
//            HID_Buffer[0] |= (1 << i);
//          }
//          if ( channels[i+16] > 0 ) {
//            HID_Buffer[1] |= (1 << i);
//          }
//          if ( channels[i+24] > 0 ) {
//            HID_Buffer[2] |= (1 << i);
//          }
//        }

        //analog values
        //uint8_t * p = HID_Buffer + 1;
        for (int i = 0; i < CHANNEL_MAX; ++i) {

          int16_t value = channels[i] + 1024;
          if ( value > 2047 ) value = 2047;
          else if ( value < 0 ) value = 0;
          HID_Buffer[i*2 +3] = (value & 0xFF);
          HID_Buffer[i*2 +4] = ((value >> 8) & 0x07);

        }
        rt_thread_mdelay(20);
    }
    return 0;
}

void msh_test_print(void)
{
    rt_kprintf("this is a test for msh.\n");
}
MSH_CMD_EXPORT(msh_test_print, this is a msh test);


