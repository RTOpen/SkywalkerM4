#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H


/**
 * @brief LCD gpio define 
 * 
 */
#define LCD_RST_PIN   GPIO_Pin_17
#define LCD_DC_PIN    GPIO_Pin_9
#define LCD_BL_PIN    GPIO_Pin_12

#define LCD_BL_HIGH()    R32_PA_OUT |= LCD_BL_PIN
#define LCD_BL_LOW()     R32_PA_CLR |= LCD_BL_PIN
#define LCD_RST_HIGH()   R32_PB_OUT |= LCD_RST_PIN
#define LCD_RST_LOW()    R32_PB_CLR |= LCD_RST_PIN
#define LCD_DC_HIGH()    R32_PB_OUT |= LCD_DC_PIN
#define LCD_DC_LOW()     R32_PB_CLR |= LCD_DC_PIN


#endif /* __HW_CONFIG_H */
