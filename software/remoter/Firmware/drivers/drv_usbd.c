#include "rtthread.h"
#include "CH58x_common.h"

void usb_dc_low_level_init(void)
{
    PFIC_EnableIRQ(USB_IRQn);
}
