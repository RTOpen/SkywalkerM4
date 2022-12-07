#include "board.h"
#include <rtthread.h>
#include "threads.h"
#include "usbd_cdc_acm_hid.h"

ALIGN(RT_ALIGN_SIZE)
static char task2_stack[512];
static struct rt_thread task2_thread;

/*********************************************************************
 * @fn      task2_entry
 *
 * @brief   task2任务函数
 *
 * @return  none
 */
void task2_entry(void *parameter)
{
    while(1)
    {
//        uint16_t c = rand()%0xffff;
//        lcd_fill_rect(0,0,240,240,c);
//        rt_kprintf("task2\r\n");
        rt_thread_delay(800);
    }
}


/*********************************************************************
 * @fn      main
 *
 * @brief   主函数
 *
 * @note    main is one of threads in rt-thread.
 *
 * @return  none
 */
int main()
{
    cdc_acm_init();
    led_hw_init();
    lcd_hw_init();
    buzzer_hw_init();
    //usbd_hw_init();
    tone_play_thread_init();
    led_thread_init();

//    rt_enter_critical();
//
//
//    rt_thread_init(&task2_thread,
//                   "task2",
//                   task2_entry,
//                   RT_NULL,
//                   &task2_stack[0],
//                   sizeof(task2_stack),
//                   4, 20);
//    rt_thread_startup(&task2_thread);
//
//    rt_exit_critical();

    return 0;
}

void msh_test_print(void)
{
    rt_kprintf("this is a test for msh.\n");
}
MSH_CMD_EXPORT(msh_test_print, this is a msh test);


