#include "CH58x_common.h"
#include <rtthread.h>
#include "drv_lcd.h"


ALIGN(RT_ALIGN_SIZE)
static char task1_stack[512];
static struct rt_thread task1_thread;

/*********************************************************************
 * @fn      task1_entry
 *
 * @brief   task1任务函数
 *
 * @return  none
 */
void task1_entry(void *parameter)
{
    while(1)
    {
        rt_kprintf("task1\r\n");
        rt_thread_delay(1000);
    }
}

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
        uint16_t c = rand()%0xffff;
        lcd_fill_rect(0,0,240,240,c);
        rt_kprintf("task2\r\n");
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
    usbd_hw_init();
    lcd_hw_init();
    rt_enter_critical();

    rt_thread_init(&task1_thread,
                   "task1",
                   task1_entry,
                   RT_NULL,
                   &task1_stack[0],
                   sizeof(task1_stack),
                   4, 20);
    rt_thread_startup(&task1_thread);

    rt_thread_init(&task2_thread,
                   "task2",
                   task2_entry,
                   RT_NULL,
                   &task2_stack[0],
                   sizeof(task2_stack),
                   4, 20);
    rt_thread_startup(&task2_thread);

    rt_exit_critical();

    return 0;
}

void msh_test_print(void)
{
    rt_kprintf("this is a test for msh.\n");
}
MSH_CMD_EXPORT(msh_test_print, this is a msh test);


