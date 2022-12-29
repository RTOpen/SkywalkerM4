#include "board.h"
#include "threads.h"
#include "gui.h"


static struct rt_thread ui_thread;

static rt_uint8_t ui_thread_stack[UI_THREAD_STACK_SIZE];


static void ui_thread_entry(void *parameter)
{
    rt_err_t ret = RT_EOK;
    gui_init();
    while(1)
    {
     gui_update();
     rt_thread_mdelay(30);
    }
}

int ui_thread_init(void)
{
    rt_err_t result = RT_EOK;
    result = rt_thread_init(&ui_thread, "ui",
        ui_thread_entry, RT_NULL,
        &ui_thread_stack[0], sizeof(ui_thread_stack),
        UI_THREAD_PRIORITY, 5);
    
    if (result == RT_EOK)
    {
        rt_thread_startup(&ui_thread);
    }

    return result;
}
