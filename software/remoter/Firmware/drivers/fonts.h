
#ifndef __FONTS_H
#define __FONTS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup Common
  * @{
  */

/** @addtogroup FONTS
  * @{
  */ 

/** @defgroup FONTS_Exported_Types
  * @{
  */ 
typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t width;
  uint16_t height;
} font_t;

extern font_t ascii_font16;
extern font_t ascii_font20;
extern font_t ascii_font24;
/**
  * @}
  */ 

/** @defgroup FONTS_Exported_Constants
  * @{
  */ 
#define LINE(x) ((x) * (((font_t *)lcd_get_font())->Height))


#ifdef __cplusplus
}
#endif
  
#endif /* __FONTS_H */
 
