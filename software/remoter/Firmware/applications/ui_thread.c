#include "board.h"
#include "threads.h"
#include "drv_lcd.h"
#include "resource.h"
#include "radio.h"

#ifdef __RTTHREAD__
static struct rt_thread ui_thread;

static rt_uint8_t ui_thread_stack[UI_THREAD_STACK_SIZE];
#endif
#ifdef __FREERTOS__
static TaskHandle_t ui_thread;
#endif
uint16_t gui_color = 0x0000;
uint16_t gui_bgcolor = 0x2AD3;
static radio_data_t data;
static const char *channel_name[CHANNEL_MAX] = {"ROLL","PITC","YAW ","THRO","AUX1","AUX2","AUX3","AUX4"};
static char disp_buffer[64];
static void ui_init(void)
{
    uint8_t i = 0;
    lcd_draw_image9patch(gImage_bg,40,40,10,2,0,236,40);
    lcd_draw_image9patch(gImage_bg,40,40,10,2,42,236,194);
    lcd_set_font(&ascii_font20);
    lcd_draw_text(50,10,"RTRemoter",gui_color,gui_bgcolor);

    for (i = 0; i < CHANNEL_MAX; ++i) {
            data.channels[i] = radio.channels[i];
            sprintf(disp_buffer,"%s:% 4d",channel_name[i],data.channels[i]);
            lcd_draw_text(10,60+i*20,disp_buffer,gui_color,gui_bgcolor);
    }
}

static void ui_update(void)
{

    uint8_t i = 0;
    for (i = 0; i < CHANNEL_MAX; ++i) {
        if(data.channels[i] != radio.channels[i])
        {
            data.channels[i] = radio.channels[i];
            sprintf(disp_buffer,"%s:% 4d",channel_name[i],data.channels[i]);
            lcd_draw_text(10,60+i*20,disp_buffer,gui_color,gui_bgcolor);
        }
    }

}

static void ui_thread_entry(void *parameter)
{
    rt_err_t ret = RT_EOK;
    ui_init();
    while(1)
    {
     ui_update();
     rt_thread_mdelay(30);
    }
}

int ui_thread_init(void)
{

    rt_err_t result = RT_EOK;
#ifdef __RTTHREAD__
    result = rt_thread_init(&ui_thread, "ui",
        ui_thread_entry, RT_NULL,
        &ui_thread_stack[0], sizeof(ui_thread_stack),
        UI_THREAD_PRIORITY, 5);
    
    if (result == RT_EOK)
    {
        rt_thread_startup(&ui_thread);
    }
#endif
#ifdef __FREERTOS__
    /* create radio task */
    xTaskCreate((TaskFunction_t)ui_thread_entry,
                (const char *)"radio",
                (uint16_t)UI_THREAD_STACK_SIZE/4,
                (void *)NULL,
                (UBaseType_t)(configMAX_PRIORITIES - UI_THREAD_PRIORITY - 1),
                (TaskHandle_t *)&ui_thread);
#endif
    return result;
}
