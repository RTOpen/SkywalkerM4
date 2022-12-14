#include "board.h"
#include "threads.h"
#include "drv_led.h"


static struct rt_thread led_thread;

static uint8_t led_thread_stack[LED_THREAD_STACK_SIZE];

static struct rt_event led_event;


static void led_thread_entry(void *parameter)
{
    uint32_t evt = 0;
    rt_err_t ret = RT_EOK;
    uint32_t timeout = 100;
    uint32_t update = 0;
    led_on(LED_1);

    while(1)
    {

       ret = rt_event_recv(&led_event, 0xffffffff,
                          RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                          timeout, &evt);
       if(ret == RT_EOK)
        {
        
            
            
        }
        if(rt_tick_get() - update >500)
        {
           led_toggle(LED_2);
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
