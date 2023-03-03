#include "board.h"
#include "threads.h"
#include "drv_led.h"
#include "radio.h"

static struct rt_thread led_thread;

static uint8_t led_thread_stack[LED_THREAD_STACK_SIZE];


static void led_thread_entry(void *parameter)
{
    uint32_t timeout = 100;
    uint32_t update = 0;
    led_on(LED_ALL);

    while(1)
    {
        if(radio.rssi==0)
        {
            if(rt_tick_get() - update >= 500)
            {
               led_toggle(LED_2);
            }
        }else {
            led_on(LED_2);
        }
        rt_thread_mdelay(100);
    }
}


int led_thread_init(void)
{
    rt_err_t result = RT_EOK;

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
