#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H


/**
 * @brief LCD gpio define 
 * 
 */
#define LCD_RST_PIN   GPIO_Pin_5
#define LCD_DC_PIN    GPIO_Pin_4
#define LCD_CS_PIN    GPIO_Pin_12
#define LCD_BL_PIN    GPIO_Pin_11

#define LCD_BL_HIGH()    R32_PA_OUT |= LCD_BL_PIN
#define LCD_BL_LOW()     R32_PA_CLR |= LCD_BL_PIN
#define LCD_RST_HIGH()   R32_PA_OUT |= LCD_RST_PIN
#define LCD_RST_LOW()    R32_PA_CLR |= LCD_RST_PIN
#define LCD_DC_HIGH()    R32_PA_OUT |= LCD_DC_PIN
#define LCD_DC_LOW()     R32_PA_CLR |= LCD_DC_PIN
#define LCD_CS_HIGH()    R32_PA_OUT |= LCD_CS_PIN
#define LCD_CS_LOW()     R32_PA_CLR |= LCD_CS_PIN


#endif /* __HW_CONFIG_H */
