#include <stdio.h>
#include "resource.h"
#include "board.h"
#include "gui.h"
#include "radio.h"

uint16_t gui_color = 0x0000;
uint16_t gui_bgcolor = 0x2AD3;
static radio_data_t data;
static const char *channel_name[CHANNEL_MAX] = {"ROLL","PITC","YAW ","THRO","AUX1","AUX2","AUX3","AUX4"};
static const char *mode_name[3] = {"STAB","ALT_HOLD","LOITER"};
static char disp_buffer[32];

uint8_t page_index = 0;
uint8_t mode_flash = 0;
uint32_t mode_last_tick = 0;
uint8_t mode_reflush = 0;

static void statusbar_init(void)
{
    lcd_draw_image9patch(gImage_bg,40,40,10,2,0,236,40);
    lcd_set_font(&ascii_font20);
    lcd_draw_text(70,10,"Remoter",gui_color,gui_bgcolor);
    lcd_draw_image(gImage_battery_0,15,5,40,28);
    lcd_draw_image(gImage_rssi_3,185,5,33,28);
}
static void statusbar_update(void)
{
   static uint32_t tick = 0;
    if(rt_tick_get() - tick > 100)
    {
        tick = rt_tick_get();
    radio.battery_level++;
    radio.battery_level = radio.battery_level%100;
    }
   if(radio.battery_level < 10)
   {
       lcd_draw_image(gImage_battery_0,15,5,40,28);
   }else if(radio.battery_level < 25)
   {
       lcd_draw_image(gImage_battery_1,15,5,40,28);
   }
   else if(radio.battery_level < 50)
      {
          lcd_draw_image(gImage_battery_2,15,5,40,28);
      }
   else if(radio.battery_level < 75)
      {
          lcd_draw_image(gImage_battery_3,15,5,40,28);
      }
   else
      {
          lcd_draw_image(gImage_battery_4,15,5,40,28);
      }
}

static void page0_init(void)
{
    uint8_t i = 0;

    lcd_draw_image9patch(gImage_bg,40,40,10,2,42,236,196);
    for (i = 0; i < 6; ++i) {
            data.channels[i] = radio.channels[i];
            sprintf(disp_buffer,"%s:% 4d",channel_name[i],data.channels[i]);
            lcd_draw_text(10,60+i*20,disp_buffer,gui_color,gui_bgcolor);
    }
         data.flight_mode = radio.flight_mode;
         sprintf(disp_buffer,"%s:%s","Mode",mode_name[radio.flight_mode]);
         lcd_draw_text(10,190,disp_buffer,gui_color,gui_bgcolor);
}
static void page0_update(void)
{
    uint8_t i = 0;
    for (i = 0; i < 6; ++i) {
        if(data.channels[i] != radio.channels[i])
        {
            data.channels[i] = radio.channels[i];
            sprintf(disp_buffer,"%s:% 4d",channel_name[i],data.channels[i]);
            lcd_draw_text(10,60+i*20,disp_buffer,gui_color,gui_bgcolor);
        }
    }
    if(radio.new_mode != radio.flight_mode)
     {
        if((rt_tick_get() - mode_last_tick) > 500)
        {
            mode_last_tick =  rt_tick_get();
            mode_flash = !mode_flash;
            mode_reflush = 1;
            lcd_fill_rect(80, 190, 200, 210, gui_bgcolor);
            if(mode_flash)
            {
              sprintf(disp_buffer,"%s:%s","Mode",mode_name[radio.new_mode]);
              lcd_draw_text(80,190,mode_name[radio.new_mode],gui_color,gui_bgcolor);
            }else {
              sprintf(disp_buffer,"%s:%s","Mode",mode_name[radio.new_mode]);
              lcd_draw_text(80,190,mode_name[radio.new_mode],gui_color,LIGHTGREEN);
            }
        }
     }else if((data.flight_mode != radio.flight_mode)|| mode_reflush ) {
         mode_reflush = 0;
         data.flight_mode = radio.flight_mode;
         lcd_fill_rect(80, 190, 200, 210, gui_bgcolor);
         sprintf(disp_buffer,"%s:%s","Mode",mode_name[radio.flight_mode]);
         lcd_draw_text(80,190,mode_name[radio.flight_mode],gui_color,gui_bgcolor);
    }
}
static void page1_init(void)
{
    uint8_t i = 0;
    lcd_draw_image9patch(gImage_bg,40,40,10,2,42,236,196);
    for (i = 0; i < 6; ++i) {
            data.channels[i] = radio.channels[i];
            sprintf(disp_buffer,"%s:% 4d",channel_name[i],data.channels[i]);
            lcd_draw_text(10,60+i*20,disp_buffer,gui_color,gui_bgcolor);
    }
}
static void page1_update(void)
{
    uint8_t i = 0;
    for (i = 0; i < 6; ++i) {
        if(data.channels[i] != radio.channels[i])
        {
            data.channels[i] = radio.channels[i];
            sprintf(disp_buffer,"%s:% 4d",channel_name[i],data.channels[i]);
            lcd_draw_text(10,60+i*20,disp_buffer,gui_color,gui_bgcolor);
        }
    }
}
static void page2_init(void)
{
    uint8_t i = 0;
    lcd_draw_image9patch(gImage_bg,40,40,10,2,42,236,196);
    for (i = 0; i < 6; ++i) {
            data.channels[i] = radio.channels[i];
            sprintf(disp_buffer,"%s:% 4d",channel_name[i],data.channels[i]);
            lcd_draw_text(10,60+i*20,disp_buffer,gui_color,gui_bgcolor);
    }
}
static void page2_update(void)
{
    uint8_t i = 0;
    for (i = 0; i < 6; ++i) {
        if(data.channels[i] != radio.channels[i])
        {
            data.channels[i] = radio.channels[i];
            sprintf(disp_buffer,"%s:% 4d",channel_name[i],data.channels[i]);
            lcd_draw_text(10,60+i*20,disp_buffer,gui_color,gui_bgcolor);
        }
    }
}

void gui_init(void)
{
    statusbar_init();
    switch(page_index)
    {
    case 0:
        page0_init();
    break;
    case 1:
        page1_init();
    break;
    case 2:
        page2_init();
    break;
    default:
        page0_init();
        page_index = 0;
        break;
    }
}

void gui_update (void)
{
    static uint8_t last_page = 0;
    statusbar_update();
    switch(page_index)
    {
    case 0:
        if(last_page != page_index)
        {
            last_page = page_index;
            page0_init();
        }else {
            page0_update();
        }
    break;
    case 1:
        if(last_page != page_index)
        {
            last_page = page_index;
            page1_init();
        }else {
            page1_update();
        }
    break;
    case 2:
        if(last_page != page_index)
        {
            last_page = page_index;
            page2_init();
        }else {
            page2_update();
        }
    break;
    default:
        page0_init();
        page_index = 0;
        last_page = page_index;
        break;
    }
}

