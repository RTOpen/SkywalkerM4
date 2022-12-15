#include "board.h"
#include <rtthread.h>
#include "threads.h"
#include "usbd_cdc_acm_hid.h"
#include "radio.h"
#include "params.h"

/*********************************************************************
 * @fn      main
 *
 * @brief   ������
 *
 * @note    main is one of threads in rt-thread.
 *
 * @return  none
 */
int main()
{
    params_init();
    adc_hw_init();
    cdc_acm_hid_init();
    led_hw_init();
    lcd_hw_init();
    buzzer_hw_init();
    encoder_hw_init();
    //tone_play_thread_init();
    led_thread_init();
    radio_thread_init();
    ui_thread_init();
    while(1)
    {

        rt_thread_mdelay(20);
    }
    return 0;
}

void msh_test_print(void)
{
    rt_kprintf("this is a test for msh.\n");
}
MSH_CMD_EXPORT(msh_test_print, this is a msh test);


