/**
 * @file drv_lcd.h
 *
 */

#ifndef __DRV_LCD_H
#define __DRV_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include "fonts.h"

#define LCD_PIXEL_WIDTH    (240)
#define LCD_PIXEL_HEIGHT   (240)


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void lcd_hw_init(void);

void lcd_fill_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

void lcd_draw_hline(uint16_t x, uint16_t y, uint16_t length,uint16_t color);

void lcd_draw_vline(uint16_t x, uint16_t y, uint16_t length,uint16_t color);

void lcd_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);

void lcd_draw_circle(uint16_t x,uint16_t y,uint16_t r,uint16_t color);

void lcd_draw_image(const unsigned char* pic,uint16_t x, uint16_t y, uint16_t width, uint16_t height);

void lcd_draw_image9patch(const unsigned char* pic,uint16_t src_w,uint16_t src_h,uint16_t r,uint16_t x, uint16_t y, uint16_t width, uint16_t height);

void lcd_draw_image9patch_mask(const unsigned char* pic,uint16_t src_w,uint16_t src_h,uint16_t r,uint16_t x, uint16_t y, uint16_t width, uint16_t height,uint16_t ms_c,uint16_t c);

void lcd_draw_char(uint16_t x, uint16_t y, const uint8_t ch,uint16_t fgcolor,uint16_t bgcolor);

void lcd_draw_num(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint16_t fgcolor,uint16_t bgcolor);

void lcd_draw_2num(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint8_t mode,uint16_t fgcolor,uint16_t bgcolor);

void lcd_draw_text(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fgcolor,uint16_t bgcolor);

void lcd_set_font(font_t *fnt);

font_t* lcd_get_font(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __DRV_LCD_H*/
