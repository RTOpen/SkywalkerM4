#ifndef __GUI_H
#define __GUI_H
#include "board.h"

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0xBC40 //��ɫ
#define BRRED 			 0xFC07 //�غ�ɫ
#define GRAY  			 0x8430 //��ɫ

#define DARKBLUE      	 0x01CF	//����ɫ
#define LIGHTBLUE      	 0x7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0x5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

void gui_init(void);

void gui_update (void);


#endif
