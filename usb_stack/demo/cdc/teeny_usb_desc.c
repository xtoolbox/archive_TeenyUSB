/*
 * Name   :  teeny_usb_desc.c
 * Author :  admin@xtoolbox.org
 * Date   :  2019-02-10 17:20:00
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
#include "teeny_usb.h"
#define  CDC_DEVICE_DESCRIPTOR_SIZE  (18)
__ALIGN_BEGIN  const uint8_t CDC_DeviceDescriptor [18] __ALIGN_END = {
    0x12,                                         /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    0x00, 0x02,                                   /* bcdUSB */
    0x00,                                         /* bDeviceClass */
    0x00,                                         /* bDeviceSubClass */
    0x00,                                         /* bDeviceProtocol */
    0x40,                                         /* bMaxPacketSize */
    LOBYTE(CDC_VID), HIBYTE(CDC_VID),             /* idVendor */
    LOBYTE(CDC_PID), HIBYTE(CDC_PID),             /* idProduct */
    0x00, 0x01,                                   /* bcdDevice */
    0x01,                                         /* iManufacture */
    0x02,                                         /* iProduct */
    0x03,                                         /* iSerial */
    0x01,                                         /* bNumConfigurations */
};

// Configs 
#define  CDC_CONFIG_DESCRIPTOR_SIZE  (67)
__ALIGN_BEGIN const uint8_t CDC_ConfigDescriptor [67] __ALIGN_END = {
    0x09,                                         /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,            /* bDescriptorType */
    0x43, 0x00,                                   /* wTotalLength */
    0x02,                                         /* bNumInterfaces */
    0x01,                                         /* bConfigurationValue */
    0x00,                                         /* iConfiguration */
    0x80,                                         /* bmAttributes */
    0x64,                                         /* bMaxPower */
    /* Interface descriptor, len: 35*/
    0x09,                                         /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                /* bDescriptorType */
    0x00,                                         /* bInterfaceNumber */
    0x00,                                         /* bAlternateSetting */
    0x01,                                         /* bNumEndpoints */
    0x02,                                         /* bInterfaceClass */
    0x02,                                         /* bInterfaceSubClass */
    0x01,                                         /* bInterfaceProtocol */
    0x00,                                         /* iInterface */
    /* Function descriptor Header */
    0x05,                                         /* bLength */
    0x24,                                         /* bDescriptorType */
    0x00,                                         /* bDescriptorSubtype */
    0x10, 0x01,                                   /* bcdCDC */
    /* Function descriptor Call Management */
    0x05,                                         /* bLength */
    0x24,                                         /* bDescriptorType */
    0x01,                                         /* bDescriptorSubtype */
    0x00,                                         /* bmCapabilities */
    0x01,                                         /* bDataInterface */
    /* Function descriptor ACM (Abstract Control Management) */
    0x04,                                         /* bLength */
    0x24,                                         /* bDescriptorType */
    0x02,                                         /* bDescriptorSubtype */
    0x02,                                         /* bmCapabilities */
    /* Function descriptor Union */
    0x05,                                         /* bLength */
    0x24,                                         /* bDescriptorType */
    0x06,                                         /* bDescriptorSubtype */
    0x00,                                         /* bMasterInterface */
    0x01,                                         /* bSlaveInterface0 */
    /* EndPoint descriptor */
    0x07,                                         /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                 /* bDescriptorType */
    0x83,                                         /* bEndpointAddress */
    0x03,                                         /* bmAttributes */
    0x40, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
    /* Interface descriptor, len: 23*/
    0x09,                                         /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                /* bDescriptorType */
    0x01,                                         /* bInterfaceNumber */
    0x00,                                         /* bAlternateSetting */
    0x02,                                         /* bNumEndpoints */
    0x0a,                                         /* bInterfaceClass */
    0x00,                                         /* bInterfaceSubClass */
    0x00,                                         /* bInterfaceProtocol */
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
    0x02,                                         /* bEndpointAddress */
    0x02,                                         /* bmAttributes */
    0x40, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
};

// Power status
#define CDC_DEV_STATUS0      (0)
#define CDC_DEV_STATUS1      (0)
#define CDC_DEV_STATUS    ((CDC_DEV_STATUS0) |(CDC_DEV_STATUS1) )

CDC_TXEP_MAX_SIZE
CDC_RXEP_MAX_SIZE

// Strings 
#define CDC_STRING_DESCRIPTOR0_STR          "\x09\x04"
#define CDC_STRING_DESCRIPTOR0_SIZE          (4)
WEAK __ALIGN_BEGIN const uint8_t  CDC_StringDescriptor0 [4] __ALIGN_END = {
    0x04,                                         /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    0x09, 0x04,                                   /* wLangID0 */
};
#define CDC_STRING_DESCRIPTOR1_STR          "TeenyUSB"
#define CDC_STRING_DESCRIPTOR1_SIZE          (18)
WEAK __ALIGN_BEGIN const uint8_t  CDC_StringDescriptor1 [18] __ALIGN_END = {
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
#define CDC_STRING_DESCRIPTOR2_STR          "TeenyUSB CDC"
#define CDC_STRING_DESCRIPTOR2_SIZE          (26)
WEAK __ALIGN_BEGIN const uint8_t  CDC_StringDescriptor2 [26] __ALIGN_END = {
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
    'C', 0x00,                                    /* wcChar9 */
    'D', 0x00,                                    /* wcChar10 */
    'C', 0x00,                                    /* wcChar11 */
};
#define CDC_STRING_DESCRIPTOR3_STR          "TUSB123456"
#define CDC_STRING_DESCRIPTOR3_SIZE          (22)
WEAK __ALIGN_BEGIN const uint8_t  CDC_StringDescriptor3 [22] __ALIGN_END = {
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
const uint8_t* const CDC_StringDescriptors[CDC_STRING_COUNT] = {
CDC_StringDescriptor0,
CDC_StringDescriptor1,
CDC_StringDescriptor2,
CDC_StringDescriptor3,
};

//  Device descriptors
const tusb_descriptors CDC_descriptors = {
  .device = CDC_DeviceDescriptor,
  .config = CDC_ConfigDescriptor,
  .strings = CDC_StringDescriptors,
  .string_cnt = CDC_STRING_COUNT,
#if defined(HAS_WCID)
#if defined(CDC_WCID_DESCRIPTOR_SIZE)
  .wcid_desc = CDC_WCIDDescriptor,
#else
  .wcid_desc = 0,  
#endif // CDC_WCID_DESCRIPTOR_SIZE)

#if defined(CDC_WCID_PROPERTIES_SIZE)
  .wcid_properties = CDC_WCIDProperties,
#else
  .wcid_properties = 0,  
#endif // CDC_WCID_PROPERTIES_SIZE

#endif // HAS_WCID
};


