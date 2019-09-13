/*
 * Name   :  teeny_usb_desc.c
 * Author :  admin@xtoolbox.org
 * Date   :  2019-09-13 20:56:40
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
#include "teeny_usb.h"
#define  MSC_DEVICE_DESCRIPTOR_SIZE  (18)
__ALIGN_BEGIN  const uint8_t MSC_DeviceDescriptor [18] __ALIGN_END = {
    0x12,                                         /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    0x00, 0x02,                                   /* bcdUSB */
    0x00,                                         /* bDeviceClass */
    0x00,                                         /* bDeviceSubClass */
    0x00,                                         /* bDeviceProtocol */
    0x40,                                         /* bMaxPacketSize */
    LOBYTE(MSC_VID), HIBYTE(MSC_VID),             /* idVendor */
    LOBYTE(MSC_PID), HIBYTE(MSC_PID),             /* idProduct */
    0x00, 0x01,                                   /* bcdDevice */
    0x01,                                         /* iManufacture */
    0x02,                                         /* iProduct */
    0x03,                                         /* iSerial */
    0x01,                                         /* bNumConfigurations */
};

// Configs 
#define  MSC_CONFIG_DESCRIPTOR_SIZE  (32)
__ALIGN_BEGIN const uint8_t MSC_ConfigDescriptor [32] __ALIGN_END = {
    0x09,                                         /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,            /* bDescriptorType */
    0x20, 0x00,                                   /* wTotalLength */
    0x01,                                         /* bNumInterfaces */
    0x01,                                         /* bConfigurationValue */
    0x00,                                         /* iConfiguration */
    0x80,                                         /* bmAttributes */
    0x64,                                         /* bMaxPower */
    /* Interface descriptor, len: 23*/
    0x09,                                         /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                /* bDescriptorType */
    0x00,                                         /* bInterfaceNumber */
    0x00,                                         /* bAlternateSetting */
    0x02,                                         /* bNumEndpoints */
    0x08,                                         /* bInterfaceClass */
    0x06,                                         /* bInterfaceSubClass */
    0x50,                                         /* bInterfaceProtocol */
    0x00,                                         /* iInterface */
    /* EndPoint descriptor */
    0x07,                                         /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                 /* bDescriptorType */
    0x81,                                         /* bEndpointAddress */
    0x02,                                         /* bmAttributes */
    0x40, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
    /* EndPoint descriptor */
    0x07,                                         /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                 /* bDescriptorType */
    0x01,                                         /* bEndpointAddress */
    0x02,                                         /* bmAttributes */
    0x40, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
};

// Power status
#define MSC_DEV_STATUS0      (0)
#define MSC_DEV_STATUS1      (0)
#define MSC_DEV_STATUS    ((MSC_DEV_STATUS0) |(MSC_DEV_STATUS1) )

MSC_TXEP_MAX_SIZE
MSC_RXEP_MAX_SIZE

// Strings 
#define MSC_STRING_DESCRIPTOR0_STR          "\x09\x04"
#define MSC_STRING_DESCRIPTOR0_SIZE          (4)
WEAK __ALIGN_BEGIN const uint8_t  MSC_StringDescriptor0 [4] __ALIGN_END = {
    0x04,                                         /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    0x09, 0x04,                                   /* wLangID0 */
};
#define MSC_STRING_DESCRIPTOR1_STR          "TeenyUSB"
#define MSC_STRING_DESCRIPTOR1_SIZE          (18)
WEAK __ALIGN_BEGIN const uint8_t  MSC_StringDescriptor1 [18] __ALIGN_END = {
    0x12,                                         /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    'T', 0x00,                                    /* wcChar0 */
    'e', 0x00,                                    /* wcChar1 */
    'e', 0x00,                                    /* wcChar2 */
    'n', 0x00,                                    /* wcChar3 */
    'y', 0x00,                                    /* wcChar4 */
    'U', 0x00,                                    /* wcChar5 */
    'S', 0x00,                                    /* wcChar6 */
    'B', 0x00,                                    /* wcChar7 */
};
#define MSC_STRING_DESCRIPTOR2_STR          "TeenyUSB MSC"
#define MSC_STRING_DESCRIPTOR2_SIZE          (26)
WEAK __ALIGN_BEGIN const uint8_t  MSC_StringDescriptor2 [26] __ALIGN_END = {
    0x1a,                                         /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    'T', 0x00,                                    /* wcChar0 */
    'e', 0x00,                                    /* wcChar1 */
    'e', 0x00,                                    /* wcChar2 */
    'n', 0x00,                                    /* wcChar3 */
    'y', 0x00,                                    /* wcChar4 */
    'U', 0x00,                                    /* wcChar5 */
    'S', 0x00,                                    /* wcChar6 */
    'B', 0x00,                                    /* wcChar7 */
    ' ', 0x00,                                    /* wcChar8 */
    'M', 0x00,                                    /* wcChar9 */
    'S', 0x00,                                    /* wcChar10 */
    'C', 0x00,                                    /* wcChar11 */
};
#define MSC_STRING_DESCRIPTOR3_STR          "TUSB123456"
#define MSC_STRING_DESCRIPTOR3_SIZE          (22)
WEAK __ALIGN_BEGIN const uint8_t  MSC_StringDescriptor3 [22] __ALIGN_END = {
    0x16,                                         /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    'T', 0x00,                                    /* wcChar0 */
    'U', 0x00,                                    /* wcChar1 */
    'S', 0x00,                                    /* wcChar2 */
    'B', 0x00,                                    /* wcChar3 */
    '1', 0x00,                                    /* wcChar4 */
    '2', 0x00,                                    /* wcChar5 */
    '3', 0x00,                                    /* wcChar6 */
    '4', 0x00,                                    /* wcChar7 */
    '5', 0x00,                                    /* wcChar8 */
    '6', 0x00,                                    /* wcChar9 */
};
const uint8_t* const MSC_StringDescriptors[MSC_STRING_COUNT] = {
MSC_StringDescriptor0,
MSC_StringDescriptor1,
MSC_StringDescriptor2,
MSC_StringDescriptor3,
};

//  Device descriptors
const tusb_descriptors MSC_descriptors = {
  .device = MSC_DeviceDescriptor,
  .config = MSC_ConfigDescriptor,
  .strings = MSC_StringDescriptors,
  .string_cnt = MSC_STRING_COUNT,
#if defined(HAS_WCID)
#if defined(MSC_WCID_DESCRIPTOR_SIZE)
  .wcid_desc = MSC_WCIDDescriptor,
#else
  .wcid_desc = 0,  
#endif // MSC_WCID_DESCRIPTOR_SIZE)

#if defined(MSC_WCID_PROPERTIES_SIZE)
  .wcid_properties = MSC_WCIDProperties,
#else
  .wcid_properties = 0,  
#endif // MSC_WCID_PROPERTIES_SIZE

#endif // HAS_WCID

#if defined(HAS_WCID_20)
#if defined(MSC_WCID_BOS_SIZE)
  .wcid_bos = MSC_WCIDBOS,
#else
  .wcid_bos = 0,  
#endif // MSC_WCID_BOS_SIZE)

#if defined(MSC_WCID_DESC_SET_SIZE)
  .wcid_desc_set = MSC_WCIDDescriptorSet,
#else
  .wcid_desc_set = 0,  
#endif // MSC_WCID_DESC_SET_SIZE


#endif // HAS_WCID_20
};


