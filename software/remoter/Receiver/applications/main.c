#include "board.h"
#include "threads.h"
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
    radio.failsafe =1;

    led_hw_init();
    led_thread_init();
    radio_thread_init();
    CH58X_BLEInit();
    HAL_Init();
    RF_RoleInit();
    radio_hw_init();
    while(1)
    {
        TMOS_SystemProcess();
        rt_thread_mdelay(1);
    }
    return 0;
}

