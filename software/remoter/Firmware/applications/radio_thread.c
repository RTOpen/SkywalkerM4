#include "threads.h"
#include "radio.h"

#define RADIO_THREAD_STACK_SIZE        (1024)

#define RADIO_THREAD_PRIORITY          (9)

static struct rt_thread radio_thread;

static rt_uint8_t radio_thread_stack[RADIO_THREAD_STACK_SIZE];

radio_data_t radio;

static void radio_thread_entry(void *parameter)
{

    while(1)
    {
     rt_thread_delay(100);
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
    
    return 0;
}
INIT_APP_EXPORT(radio_thread_init);
