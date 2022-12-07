#include "rtthread.h"
#include "CH58x_common.h"


int usbd_hw_init(void)
{
    //清中断标志
    R8_USB_INT_FG = 0xFF;

    //开启中断          挂起            传输完成         总线复位
    //R8_USB_INT_EN = RB_UIE_SUSPEND | RB_UIE_TRANSFER | RB_UIE_BUS_RST;
    R8_USB_INT_EN = RB_UIE_TRANSFER ;
    PFIC_EnableIRQ(USB_IRQn);

    //使能USB端口
    R8_UDEV_CTRL |= RB_UD_PORT_EN;

  return 0;
}


 void usb_dc_low_level_init(void)
{
     usbd_hw_init();
}
