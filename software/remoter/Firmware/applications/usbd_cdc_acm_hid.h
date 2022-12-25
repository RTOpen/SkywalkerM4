#ifndef __USBD_CDC_ACM_HID_MSC_H
#define __USBD_CDC_ACM_HID_MSC_H
#include "board.h"

extern void cdc_acm_hid_init(void);

extern void usbd_cdc_acm_write(uint8_t *buffer,uint16_t len);

extern void usbd_cdc_acm_proccess(void);

#endif  /* __USBD_CDC_ACM_HID_MSC_H */
