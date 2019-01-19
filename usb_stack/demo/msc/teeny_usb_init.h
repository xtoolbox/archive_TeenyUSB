/*
 * Name   :  teeny_usb_init.h
 * Author :  admin@xtoolbox.org
 * Date   :  2019-01-19 18:32:24
 * Desc   :  This file is auto generate by the teeny_usb script tool
 *           Visit https://github.com/xtoolbox/TeenyUSB for more info
 */

/*
  Input source name:  msc_desc.lua  
  Content type is lua script:
  ------------- lua script begin ------------
return Device {
    strManufacture = "TeenyUSB",
    strProduct = "TeenyUSB MSC",
    strSerial = "TUSB123456",
    idVendor = 0x0483,
    idProduct = 0x0003,
    prefix = "MSC",
    Config {
        Interface{
            bInterfaceClass = 0x08,        -- MSC
            bInterfaceSubClass = 0x06,     -- SCSI
            bInterfaceProtocol = 0x50,     -- BOT
            EndPoint(IN(1),  BulkDouble, 64),
            EndPoint(OUT(1), BulkDouble, 64),
        },
   }
}  

  ------------- lua script end   ------------
 */
#ifndef _TEENY_USB_INIT_H_
#define _TEENY_USB_INIT_H_
// include this file in "usbd_conf.h"

// forward declare the tusb_descriptors struct
typedef struct _tusb_descriptors tusb_descriptors;
#define MSC_VID                                             0x0483
#define MSC_PID                                             0x0003
#define MSC_STRING_COUNT                                    (4)
// Endpoint usage:
#ifdef MSC_BTABLE_ADDRESS
#undef MSC_BTABLE_ADDRESS
#endif
#define MSC_BTABLE_ADDRESS                                  (0)
#define MSC_MAX_EP                                          (1)
#define MSC_EP_NUM                                          (MSC_MAX_EP + 1)
#define MSC_EP_BUF_DESC_TABLE_SIZE                          (8)

// PMA buffer reserved for buffer description table  
#define MSC_USB_BUF_START                                   (MSC_EP_BUF_DESC_TABLE_SIZE * MSC_EP_NUM)

// EndPoints 0 defines
#define MSC_EP0_RX_SIZE                                     (64)
#define MSC_EP0_RX_ADDR                                     (MSC_USB_BUF_START + 0)
#define MSC_EP0_TX_SIZE                                     (64)
#define MSC_EP0_TX_ADDR                                     (MSC_USB_BUF_START + 64)
#define MSC_EP0_TYPE                                        USB_EP_CONTROL

// EndPoints 1 defines
#define MSC_EP1_RX_SIZE                                     (64)
#define MSC_EP1_RX_ADDR                                     (MSC_USB_BUF_START + 128)
#define MSC_EP1_TX_SIZE                                     (64)
#define MSC_EP1_TX_ADDR                                     (MSC_USB_BUF_START + 192)
#define MSC_EP1_TYPE                                        USB_EP_BULK


// Endpoint define for OTG core
#define MSC_OTG_MAX_OUT_SIZE                                (64)
#define MSC_OTG_CONTROL_EP_NUM                              (1)
#define MSC_OTG_OUT_EP_NUM                                  (1)
// RX FIFO size / 4 > (CONTROL_EP_NUM * 5 + 8) +  (MAX_OUT_SIZE / 4 + 1) + (OUT_EP_NUM*2) + 1 = 33
#define MSC_OTG_RX_FIFO_SIZE                                (256)
#define MSC_OTG_RX_FIFO_ADDR                                (0)
// Sum of IN ep max packet size is 128
// Remain Fifo size is 768 in bytes, Rx Used 256 bytes 
#define MSC_EP0_TX_FIFO_ADDR                                (256)
#define MSC_EP0_TX_FIFO_SIZE                                (MSC_EP0_TX_SIZE * 6)
#define MSC_EP1_TX_FIFO_ADDR                                (640)
#define MSC_EP1_TX_FIFO_SIZE                                (MSC_EP1_TX_SIZE * 6)

// EndPoint max packed sizes
extern const uint8_t MSC_txEpMaxSize[];
#define MSC_TXEP_MAX_SIZE                                   \
const uint8_t MSC_txEpMaxSize[] = \
{MSC_EP0_TX_SIZE, MSC_EP1_TX_SIZE, };
extern const uint8_t MSC_rxEpMaxSize[];
#define MSC_RXEP_MAX_SIZE                                   \
const uint8_t MSC_rxEpMaxSize[] = \
{MSC_EP0_RX_SIZE, MSC_EP1_RX_SIZE, };

// EndPoints init function for USB FS core
#define MSC_TUSB_INIT_EP_FS(dev) \
  do{\
    /* Init ep0 */ \
    INIT_EP_BiDirection(dev, PCD_ENDP0, MSC_EP0_TYPE);  \
    SET_TX_ADDR(dev, PCD_ENDP0, MSC_EP0_TX_ADDR);  \
    SET_RX_ADDR(dev, PCD_ENDP0, MSC_EP0_RX_ADDR);  \
    SET_RX_CNT(dev, PCD_ENDP0, MSC_EP0_RX_SIZE);  \
    /* Init ep1 */ \
    INIT_EP_BiDirection(dev, PCD_ENDP1, MSC_EP1_TYPE);  \
    SET_TX_ADDR(dev, PCD_ENDP1, MSC_EP1_TX_ADDR);  \
    SET_RX_ADDR(dev, PCD_ENDP1, MSC_EP1_RX_ADDR);  \
    SET_RX_CNT(dev, PCD_ENDP1, MSC_EP1_RX_SIZE);  \
  }while(0)


// EndPoints init function for USB OTG core
#define MSC_TUSB_INIT_EP_OTG(dev) \
  do{\
    SET_RX_FIFO(dev, MSC_OTG_RX_FIFO_ADDR, MSC_OTG_RX_FIFO_SIZE);  \
    /* Init ep0 */ \
    INIT_EP_Tx(dev, PCD_ENDP0, MSC_EP0_TYPE, MSC_EP0_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP0, MSC_EP0_TX_FIFO_ADDR, MSC_EP0_TX_FIFO_SIZE);  \
    INIT_EP_Rx(dev, PCD_ENDP0, MSC_EP0_TYPE, MSC_EP0_RX_SIZE);  \
    /* Init ep1 */ \
    INIT_EP_Tx(dev, PCD_ENDP1, MSC_EP1_TYPE, MSC_EP1_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP1, MSC_EP1_TX_FIFO_ADDR, MSC_EP1_TX_FIFO_SIZE);  \
    INIT_EP_Rx(dev, PCD_ENDP1, MSC_EP1_TYPE, MSC_EP1_RX_SIZE);  \
  }while(0)


#if defined(USB)
#define MSC_TUSB_INIT_EP(dev) MSC_TUSB_INIT_EP_FS(dev)

// Teeny USB device init function for FS core
#define MSC_TUSB_INIT_DEVICE(dev) \
  do{\
    /* Init device features */       \
    memset(&dev->addr, 0, TUSB_DEVICE_SIZE);    \
    dev->status = MSC_DEV_STATUS;         \
    dev->rx_max_size = MSC_rxEpMaxSize;         \
    dev->tx_max_size = MSC_txEpMaxSize;         \
    dev->descriptors = &MSC_descriptors;         \
  }while(0)

#endif

#if defined(USB_OTG_FS) || defined(USB_OTG_HS)
#define MSC_TUSB_INIT_EP(dev) MSC_TUSB_INIT_EP_OTG(dev)

// Teeny USB device init function for OTG core
#define MSC_TUSB_INIT_DEVICE(dev) \
  do{\
    /* Init device features */       \
    memset(&dev->addr, 0, TUSB_DEVICE_SIZE);    \
    dev->status = MSC_DEV_STATUS;         \
    dev->descriptors = &MSC_descriptors;         \
  }while(0)

#endif

#define MSC_TUSB_INIT(dev) \
  do{\
    MSC_TUSB_INIT_EP(dev);   \
    MSC_TUSB_INIT_DEVICE(dev);   \
  }while(0)

// Get End Point count
#ifndef  EP_NUM
#define  EP_NUM 1
#endif
#if MSC_EP_NUM > EP_NUM
#undef   EP_NUM
#define  EP_NUM  MSC_EP_NUM
#endif


#define  MSC_DEVICE_DESCRIPTOR_SIZE  (18)
extern const uint8_t MSC_DeviceDescriptor [18];
#define  MSC_CONFIG_DESCRIPTOR_SIZE  (32)
extern const uint8_t MSC_ConfigDescriptor [32];
#define MSC_DEV_STATUS0      (0)
#define MSC_DEV_STATUS1      (0)
#define MSC_DEV_STATUS    ((MSC_DEV_STATUS0) |(MSC_DEV_STATUS1) )

extern const uint8_t* const MSC_StringDescriptors[MSC_STRING_COUNT];
extern const tusb_descriptors MSC_descriptors;



#endif

