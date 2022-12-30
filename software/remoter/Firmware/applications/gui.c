#include "resource.h"
#include "board.h"
#include "gui.h"
#include "radio.h"

uint16_t gui_color = 0x0000;
uint16_t gui_bgcolor = 0x2AD3;
static radio_data_t data;
static const char *channel_name[CHANNEL_MAX] = {"ROLL","PITC","YAW ","THRO","AUX1","AUX2","AUX3","AUX4"};
static char disp_buffer[32];

void gui_init(void)
{
    uint8_t i = 0;
    lcd_draw_image9patch(gImage_bg,40,40,10,2,0,236,40);
    lcd_draw_image9patch(gImage_bg,40,40,10,2,42,236,196);
    lcd_set_font(&ascii_font20);
    lcd_draw_text(50,10,"RTRemoter",gui_color,gui_bgcolor);

    for (i = 0; i < CHANNEL_MAX; ++i) {
            data.channels[i] = radio.channels[i];
            sprintf(disp_buffer,"%s:% 4d",channel_name[i],data.channels[i]);
            lcd_draw_text(10,60+i*20,disp_buffer,gui_color,gui_bgcolor);
    }
}
void gui_update (void)
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

