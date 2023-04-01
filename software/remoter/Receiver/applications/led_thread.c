#include "board.h"
#include "threads.h"
#include "drv_led.h"
#include "radio.h"

static struct rt_thread led_thread;

static uint8_t led_thread_stack[LED_THREAD_STACK_SIZE];

static struct rt_event led_event;


static void led_thread_entry(void *parameter)
{
    uint32_t update = 0;
    led_on(LED_1);

    while(1)
    {
        rt_thread_mdelay(30);
        if(radio.failsafe)
        {
        if(rt_tick_get() - update >500)
        {
            update = rt_tick_get();
           led_toggle(LED_1);
        }
        }
        else
        {
           led_toggle(LED_1);
        }
    }
}


int led_thread_init(void)
{
    rt_err_t result = RT_EOK;
    rt_event_init(&led_event, "led_evt", RT_IPC_FLAG_FIFO);
    result = rt_thread_init(&led_thread, "led",
        led_thread_entry, RT_NULL, 
        &led_thread_stack[0], sizeof(led_thread_stack), 
        LED_THREAD_PRIORITY, 5);
    
    if (result == RT_EOK)
    {
        rt_thread_startup(&led_thread);
    }
    
    return result;
}
