#include "board.h"
#include "threads.h"
#include "drv_led.h"


#define LED_THREAD_STACK_SIZE        (512)

#define LED_THREAD_PRIORITY          (5)

static struct rt_thread led_thread;

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t led_thread_stack[LED_THREAD_STACK_SIZE];

static struct rt_event led_event;


static void led_thread_entry(void *parameter)
{
    rt_uint32_t evt = 0;
    rt_err_t ret = RT_EOK;
    rt_uint32_t timeout = 100;
    rt_uint32_t update = 0;

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
    
    return 0;
}
