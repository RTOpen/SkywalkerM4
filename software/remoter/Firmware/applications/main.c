#include "board.h"
#include "threads.h"
#include "usbd_cdc_acm_hid.h"
#include "radio.h"
#include "params.h"
#include "hal.h"

__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];

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
    params_init();
    led_hw_init();
    adc_hw_init();
    key_hw_init();
    cdc_acm_hid_init();
    lcd_hw_init();
    buzzer_hw_init();
    encoder_hw_init();
    tone_play_thread_init();
    led_thread_init();
    radio_thread_init();
    ui_thread_init();
    CH58X_BLEInit();
    HAL_Init();
    RF_RoleInit();
    radio_hw_init();

    while(1)
    {
        TMOS_SystemProcess();
        usbd_cdc_acm_proccess();
        rt_thread_mdelay(1);
    }
    return 0;
}

