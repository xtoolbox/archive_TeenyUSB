/*
 * Name   :  teeny_usb_init.h
 * Author :  admin@xtoolbox.org
 * Date   :  2019-01-26 21:55:31
 * Desc   :  This file is auto generate by the teeny_usb script tool
 *           Visit https://github.com/xtoolbox/TeenyUSB for more info
 */

/*
  Input source name:  cdc5_desc.lua  
  Content type is lua script:
  ------------- lua script begin ------------
return Device {
    strManufacture = "TeenyUSB",
    strProduct = "TeenyUSB CDC",
    strSerial = "TUSB123456",
    idVendor = 0x0483,
    idProduct = 0x0005,
    prefix = "CDC5",
    Config {
        CDC_ACM{
            EndPoint(IN(8),  Interrupt,  8),
            EndPoint(IN(1),  BulkDouble, 64),
            EndPoint(OUT(1), BulkDouble, 64),
        },
        CDC_ACM{
            EndPoint(IN(9),  Interrupt,  8),
            EndPoint(IN(2),  BulkDouble, 32),
            EndPoint(OUT(2), BulkDouble, 32),
        },
        CDC_ACM{
            EndPoint(IN(10),  Interrupt,  8),
            EndPoint(IN(3),  BulkDouble, 32),
            EndPoint(OUT(3), BulkDouble, 32),
        },
        CDC_ACM{
            EndPoint(IN(11),  Interrupt,  8),
            EndPoint(IN(4),  BulkDouble, 16),
            EndPoint(OUT(4), BulkDouble, 16),
        },
        CDC_ACM{
            EndPoint(IN(12),  Interrupt,  8),
            EndPoint(IN(5),  BulkDouble, 16),
            EndPoint(OUT(5), BulkDouble, 16),
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
#define CDC5_VID                                            0x0483
#define CDC5_PID                                            0x0005
#define CDC5_STRING_COUNT                                   (4)
// Endpoint usage:
#ifdef CDC5_BTABLE_ADDRESS
#undef CDC5_BTABLE_ADDRESS
#endif
#define CDC5_BTABLE_ADDRESS                                 (0)
#define CDC5_MAX_EP                                         (5)
#define CDC5_EP_NUM                                         (CDC5_MAX_EP + 1)
#define CDC5_EP_BUF_DESC_TABLE_SIZE                         (8)

// PMA buffer reserved for buffer description table  
#define CDC5_USB_BUF_START                                  (CDC5_EP_BUF_DESC_TABLE_SIZE * CDC5_EP_NUM)

// EndPoints 0 defines
#define CDC5_EP0_RX_SIZE                                    (64)
#define CDC5_EP0_RX_ADDR                                    (CDC5_USB_BUF_START + 0)
#define CDC5_EP0_TX_SIZE                                    (64)
#define CDC5_EP0_TX_ADDR                                    (CDC5_USB_BUF_START + 64)
#define CDC5_EP0_TYPE                                       USB_EP_CONTROL

// EndPoints 1 defines
#define CDC5_EP1_RX_SIZE                                    (64)
#define CDC5_EP1_RX_ADDR                                    (CDC5_USB_BUF_START + 128)
#define CDC5_EP1_TX_SIZE                                    (64)
#define CDC5_EP1_TX_ADDR                                    (CDC5_USB_BUF_START + 192)
#define CDC5_EP1_TYPE                                       USB_EP_BULK

// EndPoints 2 defines
#define CDC5_EP2_RX_SIZE                                    (32)
#define CDC5_EP2_RX_ADDR                                    (CDC5_USB_BUF_START + 256)
#define CDC5_EP2_TX_SIZE                                    (32)
#define CDC5_EP2_TX_ADDR                                    (CDC5_USB_BUF_START + 288)
#define CDC5_EP2_TYPE                                       USB_EP_BULK

// EndPoints 3 defines
#define CDC5_EP3_RX_SIZE                                    (32)
#define CDC5_EP3_RX_ADDR                                    (CDC5_USB_BUF_START + 320)
#define CDC5_EP3_TX_SIZE                                    (32)
#define CDC5_EP3_TX_ADDR                                    (CDC5_USB_BUF_START + 352)
#define CDC5_EP3_TYPE                                       USB_EP_BULK

// EndPoints 4 defines
#define CDC5_EP4_RX_SIZE                                    (16)
#define CDC5_EP4_RX_ADDR                                    (CDC5_USB_BUF_START + 384)
#define CDC5_EP4_TX_SIZE                                    (16)
#define CDC5_EP4_TX_ADDR                                    (CDC5_USB_BUF_START + 400)
#define CDC5_EP4_TYPE                                       USB_EP_BULK

// EndPoints 5 defines
#define CDC5_EP5_RX_SIZE                                    (16)
#define CDC5_EP5_RX_ADDR                                    (CDC5_USB_BUF_START + 416)
#define CDC5_EP5_TX_SIZE                                    (16)
#define CDC5_EP5_TX_ADDR                                    (CDC5_USB_BUF_START + 432)
#define CDC5_EP5_TYPE                                       USB_EP_BULK


// Endpoint define for OTG core
#define CDC5_OTG_MAX_OUT_SIZE                               (64)
#define CDC5_OTG_CONTROL_EP_NUM                             (1)
#define CDC5_OTG_OUT_EP_NUM                                 (5)
// RX FIFO size / 4 > (CONTROL_EP_NUM * 5 + 8) +  (MAX_OUT_SIZE / 4 + 1) + (OUT_EP_NUM*2) + 1 = 41
#define CDC5_OTG_RX_FIFO_SIZE_FS                            (256)
#define CDC5_OTG_RX_FIFO_ADDR_FS                            (0)
// Sum of IN ep max packet size is 224
// Remain Fifo size is 1024 in bytes, Rx Used 256 bytes 
#define CDC5_EP0_TX_FIFO_ADDR_FS                            (256)
#define CDC5_EP0_TX_FIFO_SIZE_FS                            (CDC5_EP0_TX_SIZE * 4)
#define CDC5_EP1_TX_FIFO_ADDR_FS                            (512)
#define CDC5_EP1_TX_FIFO_SIZE_FS                            (CDC5_EP1_TX_SIZE * 4)
#define CDC5_EP2_TX_FIFO_ADDR_FS                            (768)
#define CDC5_EP2_TX_FIFO_SIZE_FS                            (CDC5_EP2_TX_SIZE * 4)
#define CDC5_EP3_TX_FIFO_ADDR_FS                            (896)
#define CDC5_EP3_TX_FIFO_SIZE_FS                            (CDC5_EP3_TX_SIZE * 4)
#define CDC5_EP4_TX_FIFO_ADDR_FS                            (1024)
#define CDC5_EP4_TX_FIFO_SIZE_FS                            (CDC5_EP4_TX_SIZE * 4)
#define CDC5_EP5_TX_FIFO_ADDR_FS                            (1088)
#define CDC5_EP5_TX_FIFO_SIZE_FS                            (CDC5_EP5_TX_SIZE * 4)
// RX FIFO size / 4 > (CONTROL_EP_NUM * 5 + 8) +  (MAX_OUT_SIZE / 4 + 1) + (OUT_EP_NUM*2) + 1 = 41
#define CDC5_OTG_RX_FIFO_SIZE_HS                            (512)
#define CDC5_OTG_RX_FIFO_ADDR_HS                            (0)
// Sum of IN ep max packet size is 224
// Remain Fifo size is 3584 in bytes, Rx Used 512 bytes 
#define CDC5_EP0_TX_FIFO_ADDR_HS                            (512)
#define CDC5_EP0_TX_FIFO_SIZE_HS                            (CDC5_EP0_TX_SIZE * 7)
#define CDC5_EP1_TX_FIFO_ADDR_HS                            (960)
#define CDC5_EP1_TX_FIFO_SIZE_HS                            (CDC5_EP1_TX_SIZE * 7)
#define CDC5_EP2_TX_FIFO_ADDR_HS                            (1408)
#define CDC5_EP2_TX_FIFO_SIZE_HS                            (CDC5_EP2_TX_SIZE * 7)
#define CDC5_EP3_TX_FIFO_ADDR_HS                            (1632)
#define CDC5_EP3_TX_FIFO_SIZE_HS                            (CDC5_EP3_TX_SIZE * 7)
#define CDC5_EP4_TX_FIFO_ADDR_HS                            (1856)
#define CDC5_EP4_TX_FIFO_SIZE_HS                            (CDC5_EP4_TX_SIZE * 7)
#define CDC5_EP5_TX_FIFO_ADDR_HS                            (1968)
#define CDC5_EP5_TX_FIFO_SIZE_HS                            (CDC5_EP5_TX_SIZE * 7)

// EndPoint max packed sizes
extern const uint8_t CDC5_txEpMaxSize[];
#define CDC5_TXEP_MAX_SIZE                                  \
const uint8_t CDC5_txEpMaxSize[] = \
{CDC5_EP0_TX_SIZE, CDC5_EP1_TX_SIZE, CDC5_EP2_TX_SIZE, CDC5_EP3_TX_SIZE, CDC5_EP4_TX_SIZE, CDC5_EP5_TX_SIZE, };
extern const uint8_t CDC5_rxEpMaxSize[];
#define CDC5_RXEP_MAX_SIZE                                  \
const uint8_t CDC5_rxEpMaxSize[] = \
{CDC5_EP0_RX_SIZE, CDC5_EP1_RX_SIZE, CDC5_EP2_RX_SIZE, CDC5_EP3_RX_SIZE, CDC5_EP4_RX_SIZE, CDC5_EP5_RX_SIZE, };

// EndPoints init function for USB FS core
#define CDC5_TUSB_INIT_EP_FS(dev) \
  do{\
    /* Init ep0 */ \
    INIT_EP_BiDirection(dev, PCD_ENDP0, CDC5_EP0_TYPE);  \
    SET_TX_ADDR(dev, PCD_ENDP0, CDC5_EP0_TX_ADDR);  \
    SET_RX_ADDR(dev, PCD_ENDP0, CDC5_EP0_RX_ADDR);  \
    SET_RX_CNT(dev, PCD_ENDP0, CDC5_EP0_RX_SIZE);  \
    /* Init ep1 */ \
    INIT_EP_BiDirection(dev, PCD_ENDP1, CDC5_EP1_TYPE);  \
    SET_TX_ADDR(dev, PCD_ENDP1, CDC5_EP1_TX_ADDR);  \
    SET_RX_ADDR(dev, PCD_ENDP1, CDC5_EP1_RX_ADDR);  \
    SET_RX_CNT(dev, PCD_ENDP1, CDC5_EP1_RX_SIZE);  \
    /* Init ep2 */ \
    INIT_EP_BiDirection(dev, PCD_ENDP2, CDC5_EP2_TYPE);  \
    SET_TX_ADDR(dev, PCD_ENDP2, CDC5_EP2_TX_ADDR);  \
    SET_RX_ADDR(dev, PCD_ENDP2, CDC5_EP2_RX_ADDR);  \
    SET_RX_CNT(dev, PCD_ENDP2, CDC5_EP2_RX_SIZE);  \
    /* Init ep3 */ \
    INIT_EP_BiDirection(dev, PCD_ENDP3, CDC5_EP3_TYPE);  \
    SET_TX_ADDR(dev, PCD_ENDP3, CDC5_EP3_TX_ADDR);  \
    SET_RX_ADDR(dev, PCD_ENDP3, CDC5_EP3_RX_ADDR);  \
    SET_RX_CNT(dev, PCD_ENDP3, CDC5_EP3_RX_SIZE);  \
    /* Init ep4 */ \
    INIT_EP_BiDirection(dev, PCD_ENDP4, CDC5_EP4_TYPE);  \
    SET_TX_ADDR(dev, PCD_ENDP4, CDC5_EP4_TX_ADDR);  \
    SET_RX_ADDR(dev, PCD_ENDP4, CDC5_EP4_RX_ADDR);  \
    SET_RX_CNT(dev, PCD_ENDP4, CDC5_EP4_RX_SIZE);  \
    /* Init ep5 */ \
    INIT_EP_BiDirection(dev, PCD_ENDP5, CDC5_EP5_TYPE);  \
    SET_TX_ADDR(dev, PCD_ENDP5, CDC5_EP5_TX_ADDR);  \
    SET_RX_ADDR(dev, PCD_ENDP5, CDC5_EP5_RX_ADDR);  \
    SET_RX_CNT(dev, PCD_ENDP5, CDC5_EP5_RX_SIZE);  \
  }while(0)


// EndPoints init function for USB OTG core
#define CDC5_TUSB_INIT_EP_OTG(dev) \
  do{\
  if(GetUSB(dev) == USB_OTG_FS) { \
    SET_RX_FIFO(dev, CDC5_OTG_RX_FIFO_ADDR_FS, CDC5_OTG_RX_FIFO_SIZE_FS);  \
    /* Init ep0 */ \
    INIT_EP_Tx(dev, PCD_ENDP0, CDC5_EP0_TYPE, CDC5_EP0_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP0, CDC5_EP0_TX_FIFO_ADDR_FS, CDC5_EP0_TX_FIFO_SIZE_FS);  \
    INIT_EP_Rx(dev, PCD_ENDP0, CDC5_EP0_TYPE, CDC5_EP0_RX_SIZE);  \
    /* Init ep1 */ \
    INIT_EP_Tx(dev, PCD_ENDP1, CDC5_EP1_TYPE, CDC5_EP1_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP1, CDC5_EP1_TX_FIFO_ADDR_FS, CDC5_EP1_TX_FIFO_SIZE_FS);  \
    INIT_EP_Rx(dev, PCD_ENDP1, CDC5_EP1_TYPE, CDC5_EP1_RX_SIZE);  \
    /* Init ep2 */ \
    INIT_EP_Tx(dev, PCD_ENDP2, CDC5_EP2_TYPE, CDC5_EP2_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP2, CDC5_EP2_TX_FIFO_ADDR_FS, CDC5_EP2_TX_FIFO_SIZE_FS);  \
    INIT_EP_Rx(dev, PCD_ENDP2, CDC5_EP2_TYPE, CDC5_EP2_RX_SIZE);  \
    /* Init ep3 */ \
    INIT_EP_Tx(dev, PCD_ENDP3, CDC5_EP3_TYPE, CDC5_EP3_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP3, CDC5_EP3_TX_FIFO_ADDR_FS, CDC5_EP3_TX_FIFO_SIZE_FS);  \
    INIT_EP_Rx(dev, PCD_ENDP3, CDC5_EP3_TYPE, CDC5_EP3_RX_SIZE);  \
    /* Init ep4 */ \
    INIT_EP_Tx(dev, PCD_ENDP4, CDC5_EP4_TYPE, CDC5_EP4_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP4, CDC5_EP4_TX_FIFO_ADDR_FS, CDC5_EP4_TX_FIFO_SIZE_FS);  \
    INIT_EP_Rx(dev, PCD_ENDP4, CDC5_EP4_TYPE, CDC5_EP4_RX_SIZE);  \
    /* Init ep5 */ \
    INIT_EP_Tx(dev, PCD_ENDP5, CDC5_EP5_TYPE, CDC5_EP5_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP5, CDC5_EP5_TX_FIFO_ADDR_FS, CDC5_EP5_TX_FIFO_SIZE_FS);  \
    INIT_EP_Rx(dev, PCD_ENDP5, CDC5_EP5_TYPE, CDC5_EP5_RX_SIZE);  \
  } \
  if(GetUSB(dev) == USB_OTG_HS) { \
    SET_RX_FIFO(dev, CDC5_OTG_RX_FIFO_ADDR_HS, CDC5_OTG_RX_FIFO_SIZE_HS);  \
    /* Init ep0 */ \
    INIT_EP_Tx(dev, PCD_ENDP0, CDC5_EP0_TYPE, CDC5_EP0_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP0, CDC5_EP0_TX_FIFO_ADDR_HS, CDC5_EP0_TX_FIFO_SIZE_HS);  \
    INIT_EP_Rx(dev, PCD_ENDP0, CDC5_EP0_TYPE, CDC5_EP0_RX_SIZE);  \
    /* Init ep1 */ \
    INIT_EP_Tx(dev, PCD_ENDP1, CDC5_EP1_TYPE, CDC5_EP1_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP1, CDC5_EP1_TX_FIFO_ADDR_HS, CDC5_EP1_TX_FIFO_SIZE_HS);  \
    INIT_EP_Rx(dev, PCD_ENDP1, CDC5_EP1_TYPE, CDC5_EP1_RX_SIZE);  \
    /* Init ep2 */ \
    INIT_EP_Tx(dev, PCD_ENDP2, CDC5_EP2_TYPE, CDC5_EP2_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP2, CDC5_EP2_TX_FIFO_ADDR_HS, CDC5_EP2_TX_FIFO_SIZE_HS);  \
    INIT_EP_Rx(dev, PCD_ENDP2, CDC5_EP2_TYPE, CDC5_EP2_RX_SIZE);  \
    /* Init ep3 */ \
    INIT_EP_Tx(dev, PCD_ENDP3, CDC5_EP3_TYPE, CDC5_EP3_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP3, CDC5_EP3_TX_FIFO_ADDR_HS, CDC5_EP3_TX_FIFO_SIZE_HS);  \
    INIT_EP_Rx(dev, PCD_ENDP3, CDC5_EP3_TYPE, CDC5_EP3_RX_SIZE);  \
    /* Init ep4 */ \
    INIT_EP_Tx(dev, PCD_ENDP4, CDC5_EP4_TYPE, CDC5_EP4_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP4, CDC5_EP4_TX_FIFO_ADDR_HS, CDC5_EP4_TX_FIFO_SIZE_HS);  \
    INIT_EP_Rx(dev, PCD_ENDP4, CDC5_EP4_TYPE, CDC5_EP4_RX_SIZE);  \
    /* Init ep5 */ \
    INIT_EP_Tx(dev, PCD_ENDP5, CDC5_EP5_TYPE, CDC5_EP5_TX_SIZE);  \
    SET_TX_FIFO(dev, PCD_ENDP5, CDC5_EP5_TX_FIFO_ADDR_HS, CDC5_EP5_TX_FIFO_SIZE_HS);  \
    INIT_EP_Rx(dev, PCD_ENDP5, CDC5_EP5_TYPE, CDC5_EP5_RX_SIZE);  \
  } \
  }while(0)


#if defined(USB)
#define CDC5_TUSB_INIT_EP(dev) CDC5_TUSB_INIT_EP_FS(dev)

// Teeny USB device init function for FS core
#define CDC5_TUSB_INIT_DEVICE(dev) \
  do{\
    /* Init device features */       \
    memset(&dev->addr, 0, TUSB_DEVICE_SIZE);    \
    dev->status = CDC5_DEV_STATUS;         \
    dev->rx_max_size = CDC5_rxEpMaxSize;         \
    dev->tx_max_size = CDC5_txEpMaxSize;         \
    dev->descriptors = &CDC5_descriptors;         \
  }while(0)

#endif

#if defined(USB_OTG_FS) || defined(USB_OTG_HS)
#define CDC5_TUSB_INIT_EP(dev) CDC5_TUSB_INIT_EP_OTG(dev)

// Teeny USB device init function for OTG core
#define CDC5_TUSB_INIT_DEVICE(dev) \
  do{\
    /* Init device features */       \
    memset(&dev->addr, 0, TUSB_DEVICE_SIZE);    \
    dev->status = CDC5_DEV_STATUS;         \
    dev->descriptors = &CDC5_descriptors;         \
  }while(0)

#endif

#define CDC5_TUSB_INIT(dev) \
  do{\
    CDC5_TUSB_INIT_EP(dev);   \
    CDC5_TUSB_INIT_DEVICE(dev);   \
  }while(0)

// Get End Point count
#ifndef  EP_NUM
#define  EP_NUM 1
#endif
#if CDC5_EP_NUM > EP_NUM
#undef   EP_NUM
#define  EP_NUM  CDC5_EP_NUM
#endif


#define  CDC5_DEVICE_DESCRIPTOR_SIZE  (18)
extern const uint8_t CDC5_DeviceDescriptor [18];
#define  CDC5_CONFIG_DESCRIPTOR_SIZE  (339)
extern const uint8_t CDC5_ConfigDescriptor [339];
#define CDC5_DEV_STATUS0      (0)
#define CDC5_DEV_STATUS1      (0)
#define CDC5_DEV_STATUS    ((CDC5_DEV_STATUS0) |(CDC5_DEV_STATUS1) )

extern const uint8_t* const CDC5_StringDescriptors[CDC5_STRING_COUNT];
extern const tusb_descriptors CDC5_descriptors;



#endif

