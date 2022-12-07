#include "rtthread.h"
#include "CH58x_common.h"


int usbd_hw_init(void)
{
    //���жϱ�־
    R8_USB_INT_FG = 0xFF;

    //�����ж�          ����            �������         ���߸�λ
    //R8_USB_INT_EN = RB_UIE_SUSPEND | RB_UIE_TRANSFER | RB_UIE_BUS_RST;
    R8_USB_INT_EN = RB_UIE_TRANSFER ;
    PFIC_EnableIRQ(USB_IRQn);

    //ʹ��USB�˿�
    R8_UDEV_CTRL |= RB_UD_PORT_EN;

  return 0;
}


 void usb_dc_low_level_init(void)
{
     usbd_hw_init();
}
