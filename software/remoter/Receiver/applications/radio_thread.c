#include "threads.h"
#include "radio.h"
#include "params.h"


static struct rt_thread radio_thread;

static rt_uint8_t radio_thread_stack[RADIO_THREAD_STACK_SIZE];

radio_data_t radio;


static void radio_thread_entry(void *parameter)
{
    uint32_t timeout = 0;

    rt_thread_delay(RT_TICK_PER_SECOND);

    while(1)
    {
        timeout = rt_tick_get() - radio.last_update;

        if(timeout > RT_TICK_PER_SECOND*3)
        {
            radio.failsafe = 1;
        }

     rt_thread_mdelay(100);
    }
}

int radio_thread_init(void)
{
    rt_err_t result = RT_EOK;
    
    result = rt_thread_init(&radio_thread, "radio",
        radio_thread_entry, RT_NULL, 
        &radio_thread_stack[0], sizeof(radio_thread_stack), 
        RADIO_THREAD_PRIORITY, 5);
    
    if (result == RT_EOK)
    {
        rt_thread_startup(&radio_thread);
    }

    return result;
}

