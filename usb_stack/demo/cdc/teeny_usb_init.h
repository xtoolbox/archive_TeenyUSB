/*
 * Name   :  teeny_usb_init.h
 * Author :  admin@xtoolbox.org
 * Date   :  2019-09-13 20:56:32
 * Desc   :  This file is auto generate by the teeny_usb script tool
 *           Visit https://github.com/xtoolbox/TeenyUSB for more info
 */

/*
  Input source name:  cdc_desc.lua  
  Content type is lua script:
  ------------- lua script begin ------------
return Device {
    strManufacture = "TeenyUSB",
    strProduct = "TeenyUSB CDC",
    strSerial = "TUSB123456",
    idVendor = 0x0483,
    idProduct = 0x0002,
    prefix = "CDC",
    Config {
        CDC_ACM{
            EndPoint(IN(3),  Interrupt,  64),
            EndPoint(IN(1),  BulkDouble, 64),
            EndPoint(OUT(2), BulkDouble, 64),
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
#define CDC_VID                                             0x0483
#define CDC_PID                                             0x0002
#define CDC_STRING_COUNT                                    (4)
// Endpoint usage:
#ifdef CDC_BTABLE_ADDRESS
#undef CDC_BTABLE_ADDRESS
#endif
#define CDC_BTABLE_ADDRESS                                  (0)
#define CDC_MAX_EP                                          (3)
#define CDC_EP_NUM                                          (CDC_MAX_EP + 1)
#define CDC_EP_BUF_DESC_TABLE_SIZE                          (8)

// PMA buffer reserved for buffer description table  
#define CDC_USB_BUF_START                                   (CDC_EP_BUF_DESC_TABLE_SIZE * CDC_EP_NUM)

// EndPoints 0 defines
#define CDC_EP0_RX_SIZE                                     (64)
#define CDC_EP0_RX_ADDR                                     (CDC_USB_BUF_START + 0)
#define CDC_EP0_TX_SIZE                                     (64)
#define CDC_EP0_TX_ADDR                                     (CDC_USB_BUF_START + 64)
#define CDC_EP0_TYPE                                        USB_EP_CONTROL

// EndPoints 1 defines
#define CDC_EP1_TX_SIZE                                     (64)
#define CDC_EP1_TX0_ADDR                                    (CDC_USB_BUF_START + 128)
#define CDC_EP1_TX1_ADDR                                    (CDC_USB_BUF_START + 192)
#define CDC_EP1_TYPE                                        USB_EP_BULK

// EndPoints 2 defines
#define CDC_EP2_RX_SIZE                                     (64)
#define CDC_EP2_RX0_ADDR                                    (CDC_USB_BUF_START + 256)
#define CDC_EP2_RX1_ADDR                                    (CDC_USB_BUF_START + 320)
#define CDC_EP2_TYPE                                        USB_EP_BULK

// EndPoints 3 defines
#define CDC_EP3_TX_SIZE                                     (64)
#define CDC_EP3_TX_ADDR                                     (CDC_USB_BUF_START + 384)
#define CDC_EP3_TYPE                                        USB_EP_INTERRUPT


// Endpoint define for OTG core
#define CDC_OTG_MAX_OUT_SIZE                                (64)
#define CDC_OTG_CONTROL_EP_NUM                              (1)
#define CDC_OTG_OUT_EP_NUM                                  (1)
// RX FIFO size / 4 > (CONTROL_EP_NUM * 5 + 8) +  (MAX_OUT_SIZE / 4 + 1) + (OUT_EP_NUM*2) + 1 = 33
#define CDC_OTG_RX_FIFO_SIZE_FS                             (256)
#define CDC_OTG_RX_FIFO_ADDR_FS                             (0)
// Sum of IN ep max packet size is 192
// Remain Fifo size is 1024 in bytes, Rx Used 256 bytes 
#define CDC_EP0_TX_FIFO_ADDR_FS                             (256)
#define CDC_EP0_TX_FIFO_SIZE_FS                             (CDC_EP0_TX_SIZE * 5)
#define CDC_EP1_TX_FIFO_ADDR_FS                             (576)
#define CDC_EP1_TX_FIFO_SIZE_FS                             (CDC_EP1_TX_SIZE * 5)
#define CDC_EP3_TX_FIFO_ADDR_FS                             (896)
#define CDC_EP3_TX_FIFO_SIZE_FS                             (CDC_EP3_TX_SIZE * 5)
// RX FIFO size / 4 > (CONTROL_EP_NUM * 5 + 8) +  (MAX_OUT_SIZE / 4 + 1) + (OUT_EP_NUM*2) + 1 = 33
#define CDC_OTG_RX_FIFO_SIZE_HS                             (512)
#define CDC_OTG_RX_FIFO_ADDR_HS                             (0)
// Sum of IN ep max packet size is 192
// Remain Fifo size is 3584 in bytes, Rx Used 512 bytes 
#define CDC_EP0_TX_FIFO_ADDR_HS                             (512)
#define CDC_EP0_TX_FIFO_SIZE_HS                             (CDC_EP0_TX_SIZE * 7)
#define CDC_EP1_TX_FIFO_ADDR_HS                             (960)
#define CDC_EP1_TX_FIFO_SIZE_HS                             (CDC_EP1_TX_SIZE * 7)
#define CDC_EP3_TX_FIFO_ADDR_HS                             (1408)
#define CDC_EP3_TX_FIFO_SIZE_HS                             (CDC_EP3_TX_SIZE * 7)

// EndPoint max packed sizes
extern const uint8_t CDC_txEpMaxSize[];
#define CDC_TXEP_MAX_SIZE                                   \
const uint8_t CDC_txEpMaxSize[] = \
{CDC_EP0_TX_SIZE, CDC_EP1_TX_SIZE, 0, CDC_EP3_TX_SIZE, };
extern const uint8_t CDC_rxEpMaxSize[];
#define CDC_RXEP_MAX_SIZE                                   \
const uint8_t CDC_rxEpMaxSize[] = \
{CDC_EP0_RX_SIZE, 0, CDC_EP2_RX_SIZE, 0, };

// EndPoints init function for USB FS core
#define CDC_TUSB_INIT_EP_FS(dev) \
  do{\
    /* Init ep0 */ \
    INIT_EP_BiDirection(dev, PCD_ENDP0, CDC_EP0_TYPE);  \
    SET_TX_ADDR(dev, PCD_ENDP0, CDC_EP0_TX_ADDR);  \
    SET_RX_ADDR(dev, PCD_ENDP0, CDC_EP0_RX_ADDR);  \
    SET_RX_CNT(dev, PCD_ENDP0, CDC_EP0_RX_SIZE);  \
    /* Init ep1 */ \
    INIT_EP_TxDouble(dev, PCD_ENDP1, CDC_EP1_TYPE);     \
    SET_DOUBLE_ADDR(dev, PCD_ENDP1, CDC_EP1_TX0_ADDR, CDC_EP1_TX1_ADDR);  \
    SET_DBL_TX_CNT(dev, PCD_ENDP1, 0);     \
    /* Init ep2 */ \
    INIT_EP_RxDouble(dev, PCD_ENDP2, CDC_EP2_TYPE);     \
    SET_DOUBLE_ADDR(dev, PCD_ENDP2, CDC_EP2_RX0_ADDR, CDC_EP2_RX1_ADDR);     \
    SET_DBL_RX_CNT(dev, PCD_ENDP2, CDC_EP2_RX_SIZE);     \
    /* Init ep3 */ \
    INIT_EP_TxOnly(dev, PCD_ENDP3, CDC_EP3_TYPE);       \
    SET_TX_ADDR(dev, PCD_ENDP3, CDC_EP3_TX_ADDR);  \
  }while(0)


// EndPoints init function for USB OTG core
#if defined(USB_OTG_FS)
#define CDC_TUSB_INIT_EP_OTG_FS(dev) \
  do{\
  if(GetUSB(dev) == USB_OTG_FS) { \
    SET_RX_FIFO(dev, CDC_OTG_RX_FIFO_ADDR_FS, CDC_OTG_RX_FIFO_SIZE_FS);  \
    /* Init ep0 */ \
    INIT_EP_Tx(dev, PCD_ENDP0, CDC_EP0_TYPE, CDC_EP0_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP0, CDC_EP0_TX_FIFO_ADDR_FS, CDC_EP0_TX_FIFO_SIZE_FS);  \
    INIT_EP_Rx(dev, PCD_ENDP0, CDC_EP0_TYPE, CDC_EP0_RX_SIZE);  \
    /* Init ep1 */ \
    INIT_EP_Tx(dev, PCD_ENDP1, CDC_EP1_TYPE, CDC_EP1_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP1, CDC_EP1_TX_FIFO_ADDR_FS, CDC_EP1_TX_FIFO_SIZE_FS);  \
    /* Init ep2 */ \
    INIT_EP_Rx(dev, PCD_ENDP2, CDC_EP2_TYPE, CDC_EP2_RX_SIZE);  \
    /* Init ep3 */ \
    INIT_EP_Tx(dev, PCD_ENDP3, CDC_EP3_TYPE, CDC_EP3_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP3, CDC_EP3_TX_FIFO_ADDR_FS, CDC_EP3_TX_FIFO_SIZE_FS);  \
  } \
  }while(0)

#else  // USB_OTG_FS
#define CDC_TUSB_INIT_EP_OTG_FS(dev) 

#endif  // USB_OTG_FS

#if defined(USB_OTG_HS)
#define CDC_TUSB_INIT_EP_OTG_HS(dev) \
  do{\
  if(GetUSB(dev) == USB_OTG_HS) { \
    SET_RX_FIFO(dev, CDC_OTG_RX_FIFO_ADDR_HS, CDC_OTG_RX_FIFO_SIZE_HS);  \
    /* Init ep0 */ \
    INIT_EP_Tx(dev, PCD_ENDP0, CDC_EP0_TYPE, CDC_EP0_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP0, CDC_EP0_TX_FIFO_ADDR_HS, CDC_EP0_TX_FIFO_SIZE_HS);  \
    INIT_EP_Rx(dev, PCD_ENDP0, CDC_EP0_TYPE, CDC_EP0_RX_SIZE);  \
    /* Init ep1 */ \
    INIT_EP_Tx(dev, PCD_ENDP1, CDC_EP1_TYPE, CDC_EP1_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP1, CDC_EP1_TX_FIFO_ADDR_HS, CDC_EP1_TX_FIFO_SIZE_HS);  \
    /* Init ep2 */ \
    INIT_EP_Rx(dev, PCD_ENDP2, CDC_EP2_TYPE, CDC_EP2_RX_SIZE);  \
    /* Init ep3 */ \
    INIT_EP_Tx(dev, PCD_ENDP3, CDC_EP3_TYPE, CDC_EP3_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP3, CDC_EP3_TX_FIFO_ADDR_HS, CDC_EP3_TX_FIFO_SIZE_HS);  \
  } \
  }while(0)

#else  // USB_OTG_HS
#define CDC_TUSB_INIT_EP_OTG_HS(dev) 

#endif // USB_OTG_HS

#define CDC_TUSB_INIT_EP_OTG(dev) \
  do{\
    CDC_TUSB_INIT_EP_OTG_FS(dev); \
    CDC_TUSB_INIT_EP_OTG_HS(dev); \
  }while(0)


#if defined(USB)
#define CDC_TUSB_INIT_EP(dev) CDC_TUSB_INIT_EP_FS(dev)

// Teeny USB device init function for FS core
#define CDC_TUSB_INIT_DEVICE(dev) \
  do{\
    /* Init device features */       \
    memset(&dev->addr, 0, TUSB_DEVICE_SIZE);    \
    dev->status = CDC_DEV_STATUS;         \
    dev->rx_max_size = CDC_rxEpMaxSize;         \
    dev->tx_max_size = CDC_txEpMaxSize;         \
    dev->descriptors = &CDC_descriptors;         \
  }while(0)

#endif

#if defined(USB_OTG_FS) || defined(USB_OTG_HS)
#define CDC_TUSB_INIT_EP(dev) CDC_TUSB_INIT_EP_OTG(dev)

// Teeny USB device init function for OTG core
#define CDC_TUSB_INIT_DEVICE(dev) \
  do{\
    /* Init device features */       \
    memset(&dev->addr, 0, TUSB_DEVICE_SIZE);    \
    dev->status = CDC_DEV_STATUS;         \
    dev->descriptors = &CDC_descriptors;         \
  }while(0)

#endif

#define CDC_TUSB_INIT(dev) \
  do{\
    CDC_TUSB_INIT_EP(dev);   \
    CDC_TUSB_INIT_DEVICE(dev);   \
  }while(0)

// Get End Point count
#ifndef  EP_NUM
#define  EP_NUM 1
#endif
#if CDC_EP_NUM > EP_NUM
#undef   EP_NUM
#define  EP_NUM  CDC_EP_NUM
#endif


// Enable double buffer related code
#define  HAS_DOUBLE_BUFFER


#define  CDC_DEVICE_DESCRIPTOR_SIZE  (18)
extern const uint8_t CDC_DeviceDescriptor [18];
#define  CDC_CONFIG_DESCRIPTOR_SIZE  (67)
extern const uint8_t CDC_ConfigDescriptor [67];
#define CDC_DEV_STATUS0      (0)
#define CDC_DEV_STATUS1      (0)
#define CDC_DEV_STATUS    ((CDC_DEV_STATUS0) |(CDC_DEV_STATUS1) )

extern const uint8_t* const CDC_StringDescriptors[CDC_STRING_COUNT];
extern const tusb_descriptors CDC_descriptors;



#endif

