#include "board.h"
#include "usbd_core.h"
#include "usbd_cdc.h"
#include "usbd_hid.h"



/*!< endpoint address */
#define CDC_IN_EP  0x81
#define CDC_OUT_EP 0x01
#define CDC_INT_EP 0x82

/*!< endpoint address */
#define HID_INT_EP          0x83
#define HID_INT_EP_SIZE     19
#define HID_INT_EP_INTERVAL 1

#define USBD_VID           0x0483
#define USBD_PID           0x0110
#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 1033

/*!< report descriptor size */
#define HID_JOYSTICK_REPORT_DESC_SIZE 56

#define USB_CONFIG_SIZE (9 + CDC_ACM_DESCRIPTOR_LEN + 25)

const uint8_t cdc_acm_hid_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, 0x0200, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT(USB_CONFIG_SIZE, 0x03, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    CDC_ACM_DESCRIPTOR_INIT(0x00, CDC_INT_EP, CDC_OUT_EP, CDC_IN_EP, 0x02),
    /************** Descriptor of Joystick Mouse interface ****************/
    /* 09 */
    0x09,                          /* bLength: Interface Descriptor size */
    USB_DESCRIPTOR_TYPE_INTERFACE, /* bDescriptorType: Interface descriptor type */
    0x02,                          /* bInterfaceNumber: Number of Interface */
    0x00,                          /* bAlternateSetting: Alternate setting */
    0x01,                          /* bNumEndpoints */
    0x03,                          /* bInterfaceClass: HID */
    0x00,                          /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,                          /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,                             /* iInterface: Index of string descriptor */
    /******************** Descriptor of Joystick Mouse HID ********************/
    /* 18 */
    0x09,                    /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE_HID, /* bDescriptorType: HID */
    0x11,                    /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,                       /* bCountryCode: Hardware target country */
    0x01,                       /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                       /* bDescriptorType */
    HID_JOYSTICK_REPORT_DESC_SIZE, /* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Mouse endpoint ********************/
    /* 27 */
    0x07,                         /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
    HID_INT_EP,                   /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                         /* bmAttributes: Interrupt endpoint */
    HID_INT_EP_SIZE,              /* wMaxPacketSize: 4 Byte max */
    0x00,
    HID_INT_EP_INTERVAL, /* bInterval: Polling Interval */
    ///////////////////////////////////////
    /// string0 descriptor
    ///////////////////////////////////////
    USB_LANGID_INIT(USBD_LANGID_STRING),
    ///////////////////////////////////////
    /// string1 descriptor
    ///////////////////////////////////////
    0x14,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'S', 0x00,                  /* wcChar0 */
    'k', 0x00,                  /* wcChar1 */
    'y', 0x00,                  /* wcChar2 */
    'W', 0x00,                  /* wcChar3 */
    'a', 0x00,                  /* wcChar4 */
    'l', 0x00,                  /* wcChar5 */
    'k', 0x00,                  /* wcChar6 */
    'e', 0x00,                  /* wcChar7 */
    'r', 0x00,                  /* wcChar8 */
    ///////////////////////////////////////
    /// string2 descriptor
    ///////////////////////////////////////
    0x26,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'S', 0x00,                  /* wcChar0 */
    'k', 0x00,                  /* wcChar1 */
    'y', 0x00,                  /* wcChar2 */
    'W', 0x00,                  /* wcChar3 */
    'a', 0x00,                  /* wcChar4 */
    'l', 0x00,                  /* wcChar5 */
    'k', 0x00,                  /* wcChar6 */
    'e', 0x00,                  /* wcChar7 */
    'r', 0x00,                  /* wcChar8 */
    ' ', 0x00,                  /* wcChar9 */
    'M', 0x00,                  /* wcChar10 */
    '4', 0x00,                  /* wcChar11 */
    ' ', 0x00,                  /* wcChar12 */
    'U', 0x00,                  /* wcChar13 */
    'S', 0x00,                  /* wcChar14 */
    'B', 0x00,                  /* wcChar15 */
    ' ', 0x00,                  /* wcChar16 */
    ' ', 0x00,                  /* wcChar17 */
    ///////////////////////////////////////
    /// string3 descriptor
    ///////////////////////////////////////
    0x16,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    '2', 0x00,                  /* wcChar0 */
    '0', 0x00,                  /* wcChar1 */
    '2', 0x00,                  /* wcChar2 */
    '2', 0x00,                  /* wcChar3 */
    '1', 0x00,                  /* wcChar4 */
    '2', 0x00,                  /* wcChar5 */
    '3', 0x00,                  /* wcChar6 */
    '4', 0x00,                  /* wcChar7 */
    '5', 0x00,                  /* wcChar8 */
    '6', 0x00,                  /* wcChar9 */
#ifdef CONFIG_USB_HS
    ///////////////////////////////////////
    /// device qualifier descriptor
    ///////////////////////////////////////
    0x0a,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
#endif
    0x00
};

static const uint8_t hid_joystick_report_desc[HID_JOYSTICK_REPORT_DESC_SIZE]  =
{
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    //     USAGE (Game Pad)
    0xa1, 0x01,                    //     COLLECTION (Application)
    0xa1, 0x00,                    //       COLLECTION (Physical)
    0x05, 0x09,                    //         USAGE_PAGE (Button)
    0x19, 0x01,                    //         USAGE_MINIMUM (Button 1)
    0x29, 0x18,                    //         USAGE_MAXIMUM (Button 24)
    0x15, 0x00,                    //         LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //         LOGICAL_MAXIMUM (1)
    0x95, 0x18,                    //         REPORT_COUNT (24)
    0x75, 0x01,                    //         REPORT_SIZE (1)
    0x81, 0x02,                    //         INPUT (Data,Var,Abs)
    0x05, 0x01,                    //         USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //         USAGE (X)
    0x09, 0x31,                    //         USAGE (Y)
    0x09, 0x32,                    //         USAGE (Z)
    0x09, 0x33,                    //         USAGE (Rx)
    0x09, 0x34,                    //         USAGE (Ry)
    0x09, 0x35,                    //         USAGE (Rz)
    0x09, 0x36,                    //         USAGE (Slider)
    0x09, 0x36,                    //         USAGE (Slider)
    0x16, 0x00, 0x00,              //         LOGICAL_MINIMUM (0)
    0x26, 0xFF, 0x07,              //         LOGICAL_MAXIMUM (2047)
    0x75, 0x10,                    //         REPORT_SIZE (16)
    0x95, 0x08,                    //         REPORT_COUNT (8)
    0x81, 0x02,                    //         INPUT (Data,Var,Abs)
    0xc0,                          //       END_COLLECTION
    0xc0                           //     END_COLLECTION
};


/*!< mouse report struct */
struct hid_mouse {
    uint8_t buttons;
    int8_t x;
    int8_t y;
    int8_t wheel;
};

/*!< mouse report */
static struct hid_mouse mouse_cfg;

#define HID_STATE_IDLE 0
#define HID_STATE_BUSY 1

/*!< hid state ! Data can be sent only when state is idle  */
static volatile uint8_t hid_state = HID_STATE_IDLE;

/* function ------------------------------------------------------------------*/
static void usbd_hid_int_callback(uint8_t ep, uint32_t nbytes)
{
    hid_state = HID_STATE_IDLE;
}

/*!< endpoint call back */
static struct usbd_endpoint hid_in_ep = {
    .ep_cb = usbd_hid_int_callback,
    .ep_addr = HID_INT_EP
};

#define BUFFER_SIZE (64)
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t read_buffer[BUFFER_SIZE];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t write_buffer[BUFFER_SIZE];


static struct rt_semaphore cdc_tx_sem;

#ifdef CONFIG_USB_HS
#define CDC_MAX_MPS 512
#else
#define CDC_MAX_MPS 64
#endif

void usbd_configure_done_callback(void)
{
    /* setup first out ep read transfer */
    usbd_ep_start_read(CDC_OUT_EP, read_buffer, BUFFER_SIZE);
}

void usbd_cdc_acm_bulk_out(uint8_t ep, uint32_t nbytes)
{
    USB_LOG_RAW("actual out len:%d\r\n", nbytes);
    
    /* setup next out ep read transfer */
    usbd_ep_start_read(CDC_OUT_EP, read_buffer, BUFFER_SIZE);
}

void usbd_cdc_acm_bulk_in(uint8_t ep, uint32_t nbytes)
{
    USB_LOG_RAW("actual in len:%d\r\n", nbytes);

    if ((nbytes % CDC_MAX_MPS) == 0 && nbytes) {
        /* send zlp */
        usbd_ep_start_write(CDC_IN_EP, NULL, 0);
    } else {
        rt_sem_release(&cdc_tx_sem);
    }
}

/*!< endpoint call back */
struct usbd_endpoint cdc_out_ep = {
    .ep_addr = CDC_OUT_EP,
    .ep_cb = usbd_cdc_acm_bulk_out
};

struct usbd_endpoint cdc_in_ep = {
    .ep_addr = CDC_IN_EP,
    .ep_cb = usbd_cdc_acm_bulk_in
};

struct usbd_interface intf0;
struct usbd_interface intf1;
struct usbd_interface intf2;

void cdc_acm_hid_init(void)
{
    rt_sem_init(&cdc_tx_sem,"cdc_tx",1,RT_IPC_FLAG_FIFO);
    
    //rt_ringbuffer_init(&rb_cdc_tx,cdc_tx_buffer,sizeof(cdc_tx_buffer));
    //rt_ringbuffer_init(&rb_cdc_rx,cdc_rx_buffer,sizeof(cdc_rx_buffer));
    
    usbd_desc_register(cdc_acm_hid_descriptor);

    usbd_add_interface(usbd_cdc_acm_init_intf(&intf0));
    usbd_add_interface(usbd_cdc_acm_init_intf(&intf1));
    usbd_add_endpoint(&cdc_out_ep);
    usbd_add_endpoint(&cdc_in_ep);

//    usbd_add_interface(usbd_hid_init_intf(&intf2, hid_mouse_report_desc, HID_MOUSE_REPORT_DESC_SIZE));
//    usbd_add_endpoint(&hid_in_ep);

    /*!< init mouse report data */
    mouse_cfg.buttons = 0;
    mouse_cfg.wheel = 0;
    mouse_cfg.x = 0;
    mouse_cfg.y = 0;

    usbd_initialize();
}

/**
  * @brief            hid mouse test
  * @pre              none
  * @param[in]        none
  * @retval           none
  */
void usbd_hid_data_send(void)
{

    if(hid_state != HID_STATE_BUSY)
    {
    int ret = usbd_ep_start_write(HID_INT_EP, (uint8_t *)&mouse_cfg, 4);
    if (ret < 0) {
        return;
    }
    hid_state = HID_STATE_BUSY;
    }
}

volatile uint8_t dtr_enable = 0;

void usbd_cdc_acm_set_dtr(uint8_t intf, bool dtr)
{
    if (dtr) {
        dtr_enable = 1;
    } else {
        dtr_enable = 0;
    }
}

void usbd_cdc_acm_send(uint8_t *buffer,uint16_t len)
{
    //rt_ringbuffer_put(&rb_cdc_tx,buffer,len);
}
void usbd_cdc_acm_proccess(void)
{
    if (dtr_enable) {
        rt_sem_take(&cdc_tx_sem,RT_WAITING_FOREVER);
        int len = 0;//rt_ringbuffer_get(&rb_cdc_tx,write_buffer,256);
        if(len > 0)
        {
         usbd_ep_start_write(CDC_IN_EP, write_buffer, len);
        }
    }
}
