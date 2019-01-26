/*
 * Name   :  teeny_usb_init.h
 * Author :  admin@xtoolbox.org
 * Date   :  2019-01-13 21:20:14
 * Desc   :  This file is auto generate by the teeny_usb script tool
 *           Visit https://github.com/xtoolbox/TeenyUSB for more info
 */

/*
  Input source name:  hid_desc.lua  
  Content type is lua script:
  ------------- lua script begin ------------
return Device {
    strManufacture = "TeenyUSB",
    strProduct = "TeenyUSB Custom HID",
    strSerial = "TUSB123456",
    idVendor = 0x0483,
    idProduct = 0x0004,
    prefix = "HID",
    Config {
        USB_HID{
            ReadEp = EndPoint(IN(1),  Interrupt, 64),
            WriteEp = EndPoint(OUT(1), Interrupt, 64),
            report = HID_BuildReport([[
  // report descriptor for general input/output
  0x06, 0x00, 0xFF,  // Usage Page (Vendor Defined 0xFF00)
  0x09, 0x01,        // Usage (0x01)
  0xA1, 0x01,        // Collection (Application)
  0x09, 0x02,        //   Usage (0x02)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0xFF,        //   Logical Maximum (255)
  0x75, 0x08,        //   Report Size (8)
  0x95, 0x40,        //   Report Count 64
  0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x09, 0x03,        //   Usage (0x03)
  0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0xC0               // End Collection
]]),
        },
   }
}


















--[==[
USB_HID{
    report = {
    0x06, 0x00, 0xFF, 0x09, 0x01 ,0xA1, 0x01, 0x09, 
    0x02, 0x15, 0x00, 0x25, 0xFF, 0x75, 0x08, 0x95,
    0x40, 0x81, 0x02, 0x09, 0x03, 0x91, 0x02, 0xC0
    }
}
            --[=[
            report = HID_BuildReport([[
  // report descriptor for general input/output
  0x06, 0x00, 0xFF,  // Usage Page (Vendor Defined 0xFF00)
  0x09, 0x01,        // Usage (0x01)
  0xA1, 0x01,        // Collection (Application)
  0x09, 0x02,        //   Usage (0x02)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0xFF,        //   Logical Maximum (255)
  0x75, 0x08,        //   Report Size (8)
  0x95, 0x40,        //   Report Count 64
  0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x09, 0x03,        //   Usage (0x03)
  0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0xC0               // End Collection
]]),]=]
report = {0x01,0x02,0x03,0x04},
-- report = HID_InOut(64),
]==]  

  ------------- lua script end   ------------
 */
#ifndef _TEENY_USB_INIT_H_
#define _TEENY_USB_INIT_H_
// include this file in "usbd_conf.h"

// forward declare the tusb_descriptors struct
typedef struct _tusb_descriptors tusb_descriptors;
#define HID_VID                                             0x0483
#define HID_PID                                             0x0004
#define HID_STRING_COUNT                                    (4)
// Endpoint usage:
#ifdef HID_BTABLE_ADDRESS
#undef HID_BTABLE_ADDRESS
#endif
#define HID_BTABLE_ADDRESS                                  (0)
#define HID_MAX_EP                                          (1)
#define HID_EP_NUM                                          (HID_MAX_EP + 1)
#define HID_EP_BUF_DESC_TABLE_SIZE                          (8)

// PMA buffer reserved for buffer description table  
#define HID_USB_BUF_START                                   (HID_EP_BUF_DESC_TABLE_SIZE * HID_EP_NUM)

// EndPoints 0 defines
#define HID_EP0_RX_SIZE                                     (64)
#define HID_EP0_RX_ADDR                                     (HID_USB_BUF_START + 0)
#define HID_EP0_TX_SIZE                                     (64)
#define HID_EP0_TX_ADDR                                     (HID_USB_BUF_START + 64)
#define HID_EP0_TYPE                                        USB_EP_CONTROL

// EndPoints 1 defines
#define HID_EP1_RX_SIZE                                     (64)
#define HID_EP1_RX_ADDR                                     (HID_USB_BUF_START + 128)
#define HID_EP1_TX_SIZE                                     (64)
#define HID_EP1_TX_ADDR                                     (HID_USB_BUF_START + 192)
#define HID_EP1_TYPE                                        USB_EP_INTERRUPT


// Endpoint define for OTG core
#define HID_OTG_MAX_OUT_SIZE                                (64)
#define HID_OTG_CONTROL_EP_NUM                              (1)
#define HID_OTG_OUT_EP_NUM                                  (1)
// RX FIFO size / 4 > (CONTROL_EP_NUM * 5 + 8) +  (MAX_OUT_SIZE / 4 + 1) + (OUT_EP_NUM*2) + 1 = 33
#define HID_OTG_RX_FIFO_SIZE                                (256)
#define HID_OTG_RX_FIFO_ADDR                                (0)
// Sum of IN ep max packet size is 128
// Remain Fifo size is 768 in bytes, Rx Used 256 bytes 
#define HID_EP0_TX_FIFO_ADDR                                (256)
#define HID_EP0_TX_FIFO_SIZE                                (HID_EP0_TX_SIZE * 6)
#define HID_EP1_TX_FIFO_ADDR                                (640)
#define HID_EP1_TX_FIFO_SIZE                                (HID_EP1_TX_SIZE * 6)

// EndPoint max packed sizes
extern const uint8_t HID_txEpMaxSize[];
#define HID_TXEP_MAX_SIZE                                   \
const uint8_t HID_txEpMaxSize[] = \
{HID_EP0_TX_SIZE, HID_EP1_TX_SIZE, };
extern const uint8_t HID_rxEpMaxSize[];
#define HID_RXEP_MAX_SIZE                                   \
const uint8_t HID_rxEpMaxSize[] = \
{HID_EP0_RX_SIZE, HID_EP1_RX_SIZE, };

// EndPoints init function for USB FS core
#define HID_TUSB_INIT_EP_FS(dev) \
  do{\
    /* Init ep0 */ \
    INIT_EP_BiDirection(dev, PCD_ENDP0, HID_EP0_TYPE);  \
    SET_TX_ADDR(dev, PCD_ENDP0, HID_EP0_TX_ADDR);  \
    SET_RX_ADDR(dev, PCD_ENDP0, HID_EP0_RX_ADDR);  \
    SET_RX_CNT(dev, PCD_ENDP0, HID_EP0_RX_SIZE);  \
    /* Init ep1 */ \
    INIT_EP_BiDirection(dev, PCD_ENDP1, HID_EP1_TYPE);  \
    SET_TX_ADDR(dev, PCD_ENDP1, HID_EP1_TX_ADDR);  \
    SET_RX_ADDR(dev, PCD_ENDP1, HID_EP1_RX_ADDR);  \
    SET_RX_CNT(dev, PCD_ENDP1, HID_EP1_RX_SIZE);  \
  }while(0)


// EndPoints init function for USB OTG core
#define HID_TUSB_INIT_EP_OTG(dev) \
  do{\
    SET_RX_FIFO(dev, HID_OTG_RX_FIFO_ADDR, HID_OTG_RX_FIFO_SIZE);  \
    /* Init ep0 */ \
    INIT_EP_Tx(dev, PCD_ENDP0, HID_EP0_TYPE, HID_EP0_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP0, HID_EP0_TX_FIFO_ADDR, HID_EP0_TX_FIFO_SIZE);  \
    INIT_EP_Rx(dev, PCD_ENDP0, HID_EP0_TYPE, HID_EP0_RX_SIZE);  \
    /* Init ep1 */ \
    INIT_EP_Tx(dev, PCD_ENDP1, HID_EP1_TYPE, HID_EP1_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP1, HID_EP1_TX_FIFO_ADDR, HID_EP1_TX_FIFO_SIZE);  \
    INIT_EP_Rx(dev, PCD_ENDP1, HID_EP1_TYPE, HID_EP1_RX_SIZE);  \
  }while(0)


#if defined(USB)
#define HID_TUSB_INIT_EP(dev) HID_TUSB_INIT_EP_FS(dev)

// Teeny USB device init function for FS core
#define HID_TUSB_INIT_DEVICE(dev) \
  do{\
    /* Init device features */       \
    memset(&dev->addr, 0, TUSB_DEVICE_SIZE);    \
    dev->status = HID_DEV_STATUS;         \
    dev->rx_max_size = HID_rxEpMaxSize;         \
    dev->tx_max_size = HID_txEpMaxSize;         \
    dev->descriptors = &HID_descriptors;         \
  }while(0)

#endif

#if defined(USB_OTG_FS) || defined(USB_OTG_HS)
#define HID_TUSB_INIT_EP(dev) HID_TUSB_INIT_EP_OTG(dev)

// Teeny USB device init function for OTG core
#define HID_TUSB_INIT_DEVICE(dev) \
  do{\
    /* Init device features */       \
    memset(&dev->addr, 0, TUSB_DEVICE_SIZE);    \
    dev->status = HID_DEV_STATUS;         \
    dev->descriptors = &HID_descriptors;         \
  }while(0)

#endif

#define HID_TUSB_INIT(dev) \
  do{\
    HID_TUSB_INIT_EP(dev);   \
    HID_TUSB_INIT_DEVICE(dev);   \
  }while(0)

// Get End Point count
#ifndef  EP_NUM
#define  EP_NUM 1
#endif
#if HID_EP_NUM > EP_NUM
#undef   EP_NUM
#define  EP_NUM  HID_EP_NUM
#endif


#define  HID_DEVICE_DESCRIPTOR_SIZE  (18)
extern const uint8_t HID_DeviceDescriptor [18];
#define HID_REPORT_DESCRIPTOR_SIZE_IF0 24
extern const uint8_t HID_ReportDescriptor_if0[HID_REPORT_DESCRIPTOR_SIZE_IF0];
#define  HID_CONFIG_DESCRIPTOR_SIZE  (41)
extern const uint8_t HID_ConfigDescriptor [41];
#define HID_DEV_STATUS0      (0)
#define HID_DEV_STATUS1      (0)
#define HID_DEV_STATUS    ((HID_DEV_STATUS0) |(HID_DEV_STATUS1) )

extern const uint8_t* const HID_StringDescriptors[HID_STRING_COUNT];
extern const tusb_descriptors HID_descriptors;



#endif

