#include "rtthread.h"
#include "board.h"
#include "string.h"
#include "hw_config.h"
#include "fonts.h"


static font_t *default_font = NULL;

#define BUFFER_SIZE  (1600)

ALIGN(RT_ALIGN_SIZE)
uint16_t lcd_buffer[BUFFER_SIZE];

static struct rt_semaphore wait_sem;

#define LCD_WRITE_DATA8(data){\
SPI0_MasterSendByte(data&0xff);\
}

#define LCD_WRITE_DATA(data){\
SPI0_MasterSendByte((data>>8)&0xff);\
SPI0_MasterSendByte(data&0xff);\
} 


void LCD_WRITE_REG(uint8_t data)
{ 
	LCD_DC_LOW();//写地址
	SPI0_MasterSendByte(data);
    LCD_DC_HIGH();
}

void lcd_write_datas(uint8_t *data,uint16_t len)
{
  while(len--)
  {
   LCD_WRITE_DATA8(*data);
   data++;
  }
}

static void GPIO_Configuration(void)
{
  GPIOA_ModeCfg(GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14, GPIO_ModeOut_PP_5mA);
  GPIOB_ModeCfg(GPIO_Pin_9 | GPIO_Pin_17, GPIO_ModeOut_PP_5mA);
  LCD_DC_HIGH();
}

static void SPI_Configuration(void)
{
    R8_SPI0_CLOCK_DIV = 2; // 主频时钟4分频
    R8_SPI0_CTRL_MOD = RB_SPI_ALL_CLEAR;
    R8_SPI0_CTRL_MOD =  RB_SPI_MST_SCK_MOD|RB_SPI_MOSI_OE | RB_SPI_SCK_OE;
    R8_SPI0_CTRL_CFG |= RB_SPI_AUTO_IF;     // 访问BUFFER/FIFO自动清除IF_BYTE_END标志
    R8_SPI0_CTRL_CFG &= ~RB_SPI_DMA_ENABLE; // 不启动DMA方式
}

void LCD_SPIDMATrans(uint8_t *data, uint16_t len)
{
    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
    R16_SPI0_DMA_BEG = (uint32_t)data;
    R16_SPI0_DMA_END = (uint32_t)(data + len);
    R16_SPI0_TOTAL_CNT = len;
    R8_SPI0_INT_FLAG = RB_SPI_IF_CNT_END | RB_SPI_IF_DMA_END;
    R8_SPI0_CTRL_CFG |= RB_SPI_DMA_ENABLE;
    SPI0_ITCfg(1,SPI0_IT_DMA_END);
    PFIC_EnableIRQ(SPI0_IRQn);

    rt_sem_take(&wait_sem, RT_WAITING_FOREVER);

}

__HIGH_CODE
void SPI0_IRQHandler(void)
{
   if(R8_SPI0_INT_FLAG & RB_SPI_IF_DMA_END)
   {
      R8_SPI0_INT_FLAG = RB_SPI_IF_CNT_END | RB_SPI_IF_DMA_END;
      R8_SPI0_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
      PFIC_DisableIRQ(SPI0_IRQn);
      rt_sem_release(&wait_sem);
   }
}

static void lcd_reg_init(void)
{

  LCD_RST_LOW();//复位
  rt_thread_mdelay(100);
  LCD_RST_HIGH();

  LCD_BL_HIGH();//打开背光

  //************* Start Initial Sequence **********//
  LCD_WRITE_REG(0x11); //Sleep out
  rt_thread_mdelay(120);              //Delay 120ms
  //************* Start Initial Sequence **********//
  LCD_WRITE_REG(0x36);
  LCD_WRITE_DATA8(0x00);
  /*if(USE_HORIZONTAL==0)
  else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
  else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
  else LCD_WR_DATA8(0xA0);*/

  LCD_WRITE_REG(0x3A);
  LCD_WRITE_DATA8(0x05);

  LCD_WRITE_REG(0xB2);
  LCD_WRITE_DATA8(0x0C);
  LCD_WRITE_DATA8(0x0C);
  LCD_WRITE_DATA8(0x00);
  LCD_WRITE_DATA8(0x33);
  LCD_WRITE_DATA8(0x33);

  LCD_WRITE_REG(0xB7);
  LCD_WRITE_DATA8(0x35);

  LCD_WRITE_REG(0xBB);
  LCD_WRITE_DATA8(0x35); //Vcom=1.35V

  LCD_WRITE_REG(0xC0);
  LCD_WRITE_DATA8(0x2C);

  LCD_WRITE_REG(0xC2);
  LCD_WRITE_DATA8(0x01);

  LCD_WRITE_REG(0xC3);
  LCD_WRITE_DATA8(0x13); //GVDD=4.8V  颜色深度

  LCD_WRITE_REG(0xC4);
  LCD_WRITE_DATA8(0x20); //VDV, 0x20:0v

  LCD_WRITE_REG(0xC6);
  LCD_WRITE_DATA8(0x0F); //0x0F:60Hz

  LCD_WRITE_REG(0xD0);
  LCD_WRITE_DATA8(0xA4);
  LCD_WRITE_DATA8(0xA1);

  LCD_WRITE_REG(0xB0);
  LCD_WRITE_DATA8(0x00);
  LCD_WRITE_DATA8(0xf8);

  LCD_WRITE_REG(0xD6);
  LCD_WRITE_DATA8(0xA1);

  LCD_WRITE_REG(0xE0);
  LCD_WRITE_DATA8(0xF0);
  LCD_WRITE_DATA8(0x00);
  LCD_WRITE_DATA8(0x04);
  LCD_WRITE_DATA8(0x04);
  LCD_WRITE_DATA8(0x04);
  LCD_WRITE_DATA8(0x05);
  LCD_WRITE_DATA8(0x29);
  LCD_WRITE_DATA8(0x33);
  LCD_WRITE_DATA8(0x3E);
  LCD_WRITE_DATA8(0x38);
  LCD_WRITE_DATA8(0x12);
  LCD_WRITE_DATA8(0x12);
  LCD_WRITE_DATA8(0x28);
  LCD_WRITE_DATA8(0x30);

  LCD_WRITE_REG(0xE1);
  LCD_WRITE_DATA8(0xF0);
  LCD_WRITE_DATA8(0x07);
  LCD_WRITE_DATA8(0x0A);
  LCD_WRITE_DATA8(0x0D);
  LCD_WRITE_DATA8(0x0B);
  LCD_WRITE_DATA8(0x07);
  LCD_WRITE_DATA8(0x28);
  LCD_WRITE_DATA8(0x33);
  LCD_WRITE_DATA8(0x3E);
  LCD_WRITE_DATA8(0x36);
  LCD_WRITE_DATA8(0x14);
  LCD_WRITE_DATA8(0x14);
  LCD_WRITE_DATA8(0x29);
  LCD_WRITE_DATA8(0x32);

  LCD_WRITE_REG(0x21);

  LCD_WRITE_REG(0x11);

  LCD_WRITE_REG(0x29);
  
}

void lcd_hw_init(void)
{
  rt_sem_init(&wait_sem, "lcd_wait", 0x00, RT_IPC_FLAG_FIFO);
  // Initialize LCD GPIOs
  GPIO_Configuration();
  SPI_Configuration();
  lcd_reg_init();
  lcd_fill_rect(0,0,240,240,0x2AD3);
}

/**
  * @brief  Set Cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval None
  */
void lcd_set_cursor(uint16_t x, uint16_t y)
{
  uint8_t data[4] = {0};

  /*Column addresses*/
  LCD_WRITE_REG(0x2A);
  data[0] = (x >> 8) & 0xFF;
  data[1] = x & 0xFF;
  x= LCD_PIXEL_WIDTH -1;
  data[2] = (x >> 8) & 0xFF;
  data[3] = x & 0xFF;
  lcd_write_datas(data, 4);

  /*Page addresses*/
  LCD_WRITE_REG(0x2B);
  data[0] = (y >> 8) & 0xFF;
  data[1] = y & 0xFF;
  data[2] = (y >> 8) & 0xFF;
  data[3] = y & 0xFF;
  lcd_write_datas(data, 4);   
}

/**
  * @brief  lcd draw pixel.   
  * @param  x: specifies the x position.
  * @param  y: specifies the y position.
  * @param  color: the RGB pixel color
  * @retval None
  */
static inline void lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
  if (x >= LCD_PIXEL_WIDTH) return;
  if (y >= LCD_PIXEL_HEIGHT) return;
  uint8_t data[4] = {0};

  /*Column addresses*/
  LCD_WRITE_REG(0x2A);
  data[0] = (x >> 8) & 0xFF;
  data[1] = x & 0xFF;
  data[2] = (x >> 8) & 0xFF;
  data[3] = x & 0xFF;
  lcd_write_datas(data, 4);

  /*Page addresses*/
  LCD_WRITE_REG(0x2B);
  data[0] = (y >> 8) & 0xFF;
  data[1] = y & 0xFF;
  data[2] = (y >> 8) & 0xFF;
  data[3] = y & 0xFF;
  lcd_write_datas(data, 4);

    /*Memory write*/
  LCD_WRITE_REG(0x2C);
  LCD_SPIDMATrans((uint8_t*)&color, 2);
}

// Draw rectangle of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color
void lcd_fill_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) 
{
  if (x1 >= LCD_PIXEL_WIDTH) return;
  if (x2 >= LCD_PIXEL_WIDTH) x2=LCD_PIXEL_WIDTH-1;
  if (y1 >= LCD_PIXEL_HEIGHT) return;
  if (y2 >= LCD_PIXEL_HEIGHT) y2= LCD_PIXEL_HEIGHT-1;

  uint8_t data[4] = {0};

  /*Column addresses*/
  LCD_WRITE_REG(0x2A);
  data[0] = (x1 >> 8) & 0xFF;
  data[1] = x1 & 0xFF;
  data[2] = (x2 >> 8) & 0xFF;
  data[3] = x2 & 0xFF;
  lcd_write_datas(data, 4);

  /*Page addresses*/
  LCD_WRITE_REG(0x2B);
  data[0] = ((y1) >> 8) & 0xFF;
  data[1] = (y1) & 0xFF;
  data[2] = ((y2) >> 8) & 0xFF;
  data[3] = (y2) & 0xFF;
  lcd_write_datas(data, 4);
    /*Memory write*/
  LCD_WRITE_REG(0x2C);
  int len = (x2 - x1 + 1) * (y2 - y1 + 1);
  for(int i=0;i< BUFFER_SIZE;i++)
  {
      lcd_buffer[i] = color;
  }
	while(len > 0)
  {
	   if(len > BUFFER_SIZE)
	   {
	       LCD_SPIDMATrans((uint8_t*)lcd_buffer, BUFFER_SIZE*2);
	   }else
	       {
	       LCD_SPIDMATrans((uint8_t*)lcd_buffer, len*2);
	       }
	       len -=BUFFER_SIZE;
  }
}
/**
  * @brief  draws horizontal line.   
  * @param  x: specifies the X position.
  * @param  y: specifies the Y position.
  * @param  length: specifies the line length.  
  * @param  color: Specifies the RGB color
  * @retval None
  */
void lcd_draw_hline(uint16_t x, uint16_t y, uint16_t length,uint16_t color)
{
  int16_t len = 0;
  uint8_t data[4] = {0};
  if(x >= LCD_PIXEL_WIDTH)
      return;
  if(x + length > LCD_PIXEL_WIDTH) 
  {
    length = LCD_PIXEL_WIDTH - x;
  }
  
  /*Column addresses*/
  LCD_WRITE_REG(0x2A);
  data[0] = (x >> 8) & 0xFF;
  data[1] = x & 0xFF;
  x=x+length-1;
  data[2] = (x >> 8) & 0xFF;
  data[3] = x & 0xFF;
  lcd_write_datas(data, 4);

  /*Page addresses*/
  LCD_WRITE_REG(0x2B);
  data[0] = (y >> 8) & 0xFF;
  data[1] = y & 0xFF;
  data[2] = (y >> 8) & 0xFF;
  data[3] = y & 0xFF;
  lcd_write_datas(data, 4);
    
  /* Memory write */
  LCD_WRITE_REG(0x2C);

  for(int i=0;i< BUFFER_SIZE;i++)
  {
      lcd_buffer[i] = color;
  }
  len = length;
    while(len > 0)
  {
       if(len > BUFFER_SIZE)
       {
           LCD_SPIDMATrans((uint8_t*)lcd_buffer, BUFFER_SIZE*2);
       }else
           {
           LCD_SPIDMATrans((uint8_t*)lcd_buffer, len*2);
           }
           len -=BUFFER_SIZE;
  }
}

/**
  * @brief  Draws vertical line.
  * @param  RGBCode: Specifies the RGB color   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Length: specifies the line length.  
  * @retval None
  */
void lcd_draw_vline(uint16_t x, uint16_t y, uint16_t length,uint16_t color)
{
  uint16_t counter = 0;
  
  if(y + length > LCD_PIXEL_HEIGHT) 
  {
      return;
  }
  for(counter = 0; counter < length; counter++)
  {
    lcd_draw_pixel(x, y + counter, color);
  }   
}

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		lcd_draw_pixel(uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//画矩形
void lcd_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	lcd_draw_hline(x1,y1,x2 - x1 + 1,color);
	lcd_draw_vline(x1,y1,y2 - y1 + 1,color);
	lcd_draw_hline(x1,y2,x2 - x1 + 1,color);
	lcd_draw_vline(x2,y1,y2 - y1 + 1,color);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void lcd_draw_circle(uint16_t x,uint16_t y,uint16_t r,uint16_t color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		lcd_draw_pixel(x-b,y-a,color);             //3           
		lcd_draw_pixel(x+b,y-a,color);             //0           
		lcd_draw_pixel(x-a,y+b,color);             //1       
		lcd_draw_pixel(x-b,y-a,color);             //7           
		lcd_draw_pixel(x-a,y-b,color);             //2             
		lcd_draw_pixel(x+b,y+a,color);             //4               
		lcd_draw_pixel(x+a,y-b,color);             //5
		lcd_draw_pixel(x+a,y+b,color);             //6 
		lcd_draw_pixel(x-b,y+a,color);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		lcd_draw_pixel(x+a,y+b,color);
	}
} 

void lcd_draw_image(const unsigned char* pic,uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
  if (x >= LCD_PIXEL_WIDTH) return;
  if (y >= LCD_PIXEL_HEIGHT) return;
 
  uint16_t x1 = x,x2 = x + width-1;
  uint16_t y1 = y,y2 = y + height-1;  
  uint16_t *color =  (uint16_t*)pic;
  int32_t len = 0;
  uint8_t data[4] = {0};
  if (x2 >= LCD_PIXEL_WIDTH) x2=LCD_PIXEL_WIDTH-1;
  if (y2 >= LCD_PIXEL_HEIGHT) y2= LCD_PIXEL_HEIGHT-1; 
  /*Column addresses*/
  LCD_WRITE_REG(0x2A);
  data[0] = (x1 >> 8) & 0xFF;
  data[1] = x1 & 0xFF;
  data[2] = (x2 >> 8) & 0xFF;
  data[3] = x2 & 0xFF;
  lcd_write_datas(data, 4);

  /*Page addresses*/
  LCD_WRITE_REG(0x2B);
  data[0] = ((y1) >> 8) & 0xFF;
  data[1] = (y1) & 0xFF;
  data[2] = ((y2) >> 8) & 0xFF;
  data[3] = (y2) & 0xFF;
  lcd_write_datas(data, 4);

    /*Memory write*/
  LCD_WRITE_REG(0x2C);
  len = width*height;
    while(len > 0)
  {
       if(len > BUFFER_SIZE)
       {
           memcpy(lcd_buffer,color,BUFFER_SIZE*2);
           LCD_SPIDMATrans((uint8_t*)lcd_buffer, BUFFER_SIZE*2);
       }else
           {
           memcpy(lcd_buffer,color,len*2);
           LCD_SPIDMATrans((uint8_t*)lcd_buffer, len*2);
           }
     len -=BUFFER_SIZE;
     color+=BUFFER_SIZE;
  }

}

void lcd_draw_image9patch(const unsigned char* pic,uint16_t src_w,uint16_t src_h,uint16_t r,uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
  if (x >= LCD_PIXEL_WIDTH) return;
  if (y >= LCD_PIXEL_HEIGHT) return;
 
  uint16_t x1 = x,x2 = x + width-1;
  uint16_t y1 = y,y2 = y + height-1;  
  uint16_t *color =  (uint16_t *)pic;
  uint8_t data[4] = {0};
  if (x2 >= LCD_PIXEL_WIDTH) x2=LCD_PIXEL_WIDTH-1;
  if (y2 >= LCD_PIXEL_HEIGHT) y2= LCD_PIXEL_HEIGHT-1; 
  
  if(r > 32)
      r = 32;
  //copy data of top left
  for(int i = 0;i< r;i++)
  {
    color = (uint16_t *)&pic[i*src_w*2];
    memcpy(&lcd_buffer[i*r],color,r*sizeof(uint16_t));
  }
  x1 = x;
  x2 = x1 + r -1;
  y1 = y;
  y2 = y1 + r -1;
  /*Column addresses*/
  LCD_WRITE_REG(0x2A);
  data[0] = (x1 >> 8) & 0xFF;
  data[1] = x1 & 0xFF;
  data[2] = (x2 >> 8) & 0xFF;
  data[3] = x2 & 0xFF;
  lcd_write_datas(data, 4);

  /*Page addresses*/
  LCD_WRITE_REG(0x2B);
  data[0] = (y1 >> 8) & 0xFF;
  data[1] = y1 & 0xFF;
  data[2] = (y2 >> 8) & 0xFF;
  data[3] = y2 & 0xFF;
  lcd_write_datas(data, 4);

    /*Memory write*/
  LCD_WRITE_REG(0x2C);
  LCD_SPIDMATrans(lcd_buffer,r*r*2);
  
    //copy data top right
  for(int i = 0;i< r;i++)
  {
    color = (uint16_t *)&pic[((i+1)*src_w - r) * 2];
    memcpy(&lcd_buffer[i*r],color,r*sizeof(uint16_t));
  }
  x1 = x + width - r;
  x2 = x1 + r -1;
  y1 = y;
  y2 = y1 + r -1;
  /*Column addresses*/
  LCD_WRITE_REG(0x2A);
  data[0] = (x1 >> 8) & 0xFF;
  data[1] = x1 & 0xFF;
  data[2] = (x2 >> 8) & 0xFF;
  data[3] = x2 & 0xFF;
  lcd_write_datas(data, 4);

  /*Page addresses*/
  LCD_WRITE_REG(0x2B);
  data[0] = (y1 >> 8) & 0xFF;
  data[1] = y1 & 0xFF;
  data[2] = (y2 >> 8) & 0xFF;
  data[3] = y2 & 0xFF;
  lcd_write_datas(data, 4);

    /*Memory write*/
  LCD_WRITE_REG(0x2C);
  LCD_SPIDMATrans(lcd_buffer,r*r*2);
   
  //copy data bottom left
  for(int i = 0;i< r;i++)
  {
    color = (uint16_t *)&pic[((src_h- r + i)*src_w) * 2];
    memcpy(&lcd_buffer[i*r],color,r*sizeof(uint16_t));
  }
  x1 = x;
  x2 = x1 + r -1;
  y1 = y + height -r;
  y2 = y1 + r -1;
  /*Column addresses*/
  LCD_WRITE_REG(0x2A);
  data[0] = (x1 >> 8) & 0xFF;
  data[1] = x1 & 0xFF;
  data[2] = (x2 >> 8) & 0xFF;
  data[3] = x2 & 0xFF;
  lcd_write_datas(data, 4);

  /*Page addresses*/
  LCD_WRITE_REG(0x2B);
  data[0] = (y1 >> 8) & 0xFF;
  data[1] = y1 & 0xFF;
  data[2] = (y2 >> 8) & 0xFF;
  data[3] = y2 & 0xFF;
  lcd_write_datas(data, 4);

    /*Memory write*/
  LCD_WRITE_REG(0x2C);
  LCD_SPIDMATrans(lcd_buffer,r*r*2);
 
  //copy data bottom right
  for(int i = 0;i< r;i++)
  {
    color = (uint16_t *)&pic[((src_h - r + i+1)*src_w-r) * 2];
    memcpy(&lcd_buffer[i*r],color,r*sizeof(uint16_t));
  }
  x1 = x + width - r;
  x2 = x1 + r -1;
  y1 = y + height -r;
  y2 = y1 + r -1;
  /*Column addresses*/
  LCD_WRITE_REG(0x2A);
  data[0] = (x1 >> 8) & 0xFF;
  data[1] = x1 & 0xFF;
  data[2] = (x2 >> 8) & 0xFF;
  data[3] = x2 & 0xFF;
  lcd_write_datas(data, 4);

  /*Page addresses*/
  LCD_WRITE_REG(0x2B);
  data[0] = (y1 >> 8) & 0xFF;
  data[1] = y1 & 0xFF;
  data[2] = (y2 >> 8) & 0xFF;
  data[3] = y2 & 0xFF;
  lcd_write_datas(data, 4);

    /*Memory write*/
  LCD_SPIDMATrans(lcd_buffer,r*r*2);
 
  //copy data top
  uint16_t line_len = (src_w - 2*r);
  uint16_t data_h = r;
  for(int i = 0;i< data_h;i++)
  {
    color = (uint16_t *)&pic[(i*src_w+r) * 2];
    memcpy(&lcd_buffer[i*line_len],color,line_len*sizeof(uint16_t));
  }

  x1 = x + r;
  x2 = x + width - r -1;
  y1 = y;
  y2 = y + r - 1;
  /*Column addresses*/
  LCD_WRITE_REG(0x2A);
  data[0] = (x1 >> 8) & 0xFF;
  data[1] = x1 & 0xFF;
  data[2] = (x2 >> 8) & 0xFF;
  data[3] = x2 & 0xFF;
  lcd_write_datas(data, 4);

  /*Page addresses*/
  LCD_WRITE_REG(0x2B);
  data[0] = (y1 >> 8) & 0xFF;
  data[1] = y1 & 0xFF;
  data[2] = (y2 >> 8) & 0xFF;
  data[3] = y2 & 0xFF;
  lcd_write_datas(data, 4);

    /*Memory write*/
  LCD_WRITE_REG(0x2C);
  LCD_SPIDMATrans(lcd_buffer,r*r*2);
	for(int i=0;i < r;i++) {
       for(int j= 0;j < (width - 2*r);j++)
        {
        LCD_WRITE_COLOR(lcd_buffer[(i%data_h)*line_len+j%line_len]);
        }
	}
    
  //copy data bottom
  line_len = (src_w - 2*r);
  data_h = r;
  for(int i = 0;i< data_h;i++)
  {
    color = (uint16_t *)&pic[((src_h - r + i)*src_w+r) * 2];
    memcpy(&lcd_buffer[i*line_len],color,line_len*sizeof(uint16_t));
  }

  x1 = x + r;
  x2 = x + width - r -1;
  y1 = y + height - r;
  y2 = y + height - 1;
  /*Column addresses*/
  LCD_WRITE_REG(0x2A);
  data[0] = (x1 >> 8) & 0xFF;
  data[1] = x1 & 0xFF;
  data[2] = (x2 >> 8) & 0xFF;
  data[3] = x2 & 0xFF;
  lcd_write_datas(data, 4);

  /*Page addresses*/
  LCD_WRITE_REG(0x2B);
  data[0] = (y1 >> 8) & 0xFF;
  data[1] = y1 & 0xFF;
  data[2] = (y2 >> 8) & 0xFF;
  data[3] = y2 & 0xFF;
  lcd_write_datas(data, 4);

    /*Memory write*/
  LCD_WRITE_REG(0x2C);
	for(int i=0;i < r;i++) {
       for(int j= 0;j < (width - 2*r);j++)
        {
        LCD_WRITE_COLOR(lcd_buffer[(i%data_h)*line_len+j%line_len]);
        }
	}
    
   //copy data left
  line_len = r;
  data_h = (src_h - 2*r);
  for(int i = 0;i< data_h;i++)
  {
    color = (uint16_t *)&pic[((r+i)*src_w) * 2];
    memcpy(&lcd_buffer[i*line_len],color,line_len*sizeof(uint16_t));
  }

  x1 = x;
  x2 = x + r -1;
  y1 = y + r;
  y2 = y + height - r -1;
  /*Column addresses*/
  LCD_WRITE_REG(0x2A);
  data[0] = (x1 >> 8) & 0xFF;
  data[1] = x1 & 0xFF;
  data[2] = (x2 >> 8) & 0xFF;
  data[3] = x2 & 0xFF;
  lcd_write_datas(data, 4);

  /*Page addresses*/
  LCD_WRITE_REG(0x2B);
  data[0] = (y1 >> 8) & 0xFF;
  data[1] = y1 & 0xFF;
  data[2] = (y2 >> 8) & 0xFF;
  data[3] = y2 & 0xFF;
  lcd_write_datas(data, 4);

    /*Memory write*/
  LCD_WRITE_REG(0x2C);
	for(int i=0;i < (height - 2*r);i++) {
       for(int j= 0;j < r;j++)
        {
        LCD_WRITE_COLOR(lcd_buffer[(i%data_h)*line_len+j%line_len]);
        }
	}
 
   //copy data right
  line_len = r;
  data_h = (src_h - 2*r);
  for(int i = 0;i< data_h;i++)
  {
    color = (uint16_t *)&pic[((r+i+1)*src_w-r) * 2];
    memcpy(&lcd_buffer[i*line_len],color,line_len*sizeof(uint16_t));
  }

  x1 = x + width - r;
  x2 = x + width -1;
  y1 = y + r;
  y2 = y + height - r -1;
  /*Column addresses*/
  LCD_WRITE_REG(0x2A);
  data[0] = (x1 >> 8) & 0xFF;
  data[1] = x1 & 0xFF;
  data[2] = (x2 >> 8) & 0xFF;
  data[3] = x2 & 0xFF;
  lcd_write_datas(data, 4);

  /*Page addresses*/
  LCD_WRITE_REG(0x2B);
  data[0] = (y1 >> 8) & 0xFF;
  data[1] = y1 & 0xFF;
  data[2] = (y2 >> 8) & 0xFF;
  data[3] = y2 & 0xFF;
  lcd_write_datas(data, 4);

    /*Memory write*/
  LCD_WRITE_REG(0x2C);
	for(int i=0;i < (height - 2*r);i++) {
       for(int j= 0;j < r;j++)
        {
        LCD_WRITE_COLOR(lcd_buffer[(i%data_h)*line_len+j%line_len]);
        }
	}
    
   //copy data main
  line_len = (src_w - 2*r);
  data_h = (src_h - 2*r);
  for(int i = 0;i< data_h;i++)
  {
    color = (uint16_t *)&pic[((r+i)*src_w + r) * 2];
    memcpy(&lcd_buffer[i*line_len],color,line_len*sizeof(uint16_t));
  }

  x1 = x + r;
  x2 = x + width - r -1;
  y1 = y + r;
  y2 = y + height - r -1;
  /*Column addresses*/
  LCD_WRITE_REG(0x2A);
  data[0] = (x1 >> 8) & 0xFF;
  data[1] = x1 & 0xFF;
  data[2] = (x2 >> 8) & 0xFF;
  data[3] = x2 & 0xFF;
  lcd_write_datas(data, 4);

  /*Page addresses*/
  LCD_WRITE_REG(0x2B);
  data[0] = (y1 >> 8) & 0xFF;
  data[1] = y1 & 0xFF;
  data[2] = (y2 >> 8) & 0xFF;
  data[3] = y2 & 0xFF;
  lcd_write_datas(data, 4);

    /*Memory write*/
  LCD_WRITE_REG(0x2C);
	for(int i=0;i < (height - 2*r);i++) {
       for(int j= 0;j < (width - 2*r);j++)
        {
        LCD_WRITE_COLOR(lcd_buffer[(i%data_h)*line_len+j%line_len]);
        }
	}
  
}

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: Line where to display the character shape
  * @param  Ypos: Start column address
  * @param  c: Pointer to the character data
  */
void lcd_draw_char(uint16_t x, uint16_t y, const uint8_t ch,uint16_t fgcolor,uint16_t bgcolor)
{
  uint32_t i = 0, j = 0;
  uint16_t height, width;
  uint8_t offset;
  uint8_t *pchar;
  uint8_t *data;
  uint8_t cmd[4];
  uint32_t line;
  if(default_font == NULL)
      return;
  
  height =default_font->height;
  width  = default_font->width;
  data = (uint8_t*)&default_font->table[(ch-' ')*height * ((width + 7) / 8)];
  offset =  8 *((width + 7)/8) -  width ;
  
  for(i = 0; i < height; i++)
  {
    pchar = ((uint8_t *)data + (width + 7)/8 * i);
    
    switch(((width + 7)/8))
    {
    case 1:
      line =  pchar[0];
      break;    

    case 2:
      line =  (pchar[0]<< 8) | pchar[1];
      break;
      
    case 3:
    default:
      line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
      break;
    }  
    
    for (j = 0; j < width; j++)
    {
      if(line & (1 << (width- j + offset- 1))) 
      {
         lcd_buffer[j + i * width] = fgcolor;
      }
      else
      {
        lcd_buffer[j + i * width] = bgcolor;
      } 
    }
    y++;
  }
  /*Column addresses*/
  LCD_WRITE_REG(0x2A);
  cmd[0] = (x >> 8) & 0xFF;
  cmd[1] = x & 0xFF;
  cmd[2] = ((x+width-1) >> 8) & 0xFF;
  cmd[3] = (x+width-1) & 0xFF;
  lcd_write_datas(cmd, 4);

  /*Page addresses*/
  LCD_WRITE_REG(0x2B);
  cmd[0] = ((y) >> 8) & 0xFF;
  cmd[1] = (y) & 0xFF;
  cmd[2] = ((y+height - 1) >> 8) & 0xFF;
  cmd[3] = (y+height - 1) & 0xFF;
  lcd_write_datas(cmd, 4);

    /*Memory write*/
  LCD_WRITE_REG(0x2C);
  LCD_SPIDMATrans(lcd_buffer, width*height*2);

}


//m^n函数
uint32_t mypow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
	while(n--)result*=m;    
	return result;
}			 
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);	 
void lcd_draw_num(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint16_t fgcolor,uint16_t bgcolor)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				lcd_draw_char(x+(default_font->width)*t,y,' ',fgcolor,bgcolor);
				continue;
			}else enshow=1; 
		 	 
		}
	 	lcd_draw_char(x+(default_font->width)*t,y,temp+'0',fgcolor,bgcolor); 
	}
} 
//显示2个数字
//x,y:起点坐标
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~99);	 
void lcd_draw_2num(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint8_t mode,uint16_t fgcolor,uint16_t bgcolor)
{         	
	uint8_t t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	lcd_draw_char(x+(default_font->width)*t,y,temp+'0',fgcolor,bgcolor); 
	}
} 
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void lcd_draw_text(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fgcolor,uint16_t bgcolor)
{         
    while(*p!='\0')
    {       
        if(x>(LCD_PIXEL_WIDTH-default_font->width)){x=0;y+=default_font->height;}
        if(y>(LCD_PIXEL_HEIGHT-default_font->height)){y=x=0;}
        lcd_draw_char(x,y,*p,fgcolor,bgcolor);
        x+=default_font->width;
        p++;
    }  
}

void lcd_set_font(font_t *fnt)
{
   default_font = fnt;
}

font_t* lcd_get_font(void)
{
   return default_font;
}

