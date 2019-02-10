/*
 * Name   :  teeny_usb_desc.c
 * Author :  admin@xtoolbox.org
 * Date   :  2019-02-10 17:20:30
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
#include "teeny_usb.h"
#define  CDC5_DEVICE_DESCRIPTOR_SIZE  (18)
__ALIGN_BEGIN  const uint8_t CDC5_DeviceDescriptor [18] __ALIGN_END = {
    0x12,                                         /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    0x00, 0x02,                                   /* bcdUSB */
    0xef,                                         /* bDeviceClass */
    0x02,                                         /* bDeviceSubClass */
    0x01,                                         /* bDeviceProtocol */
    0x40,                                         /* bMaxPacketSize */
    LOBYTE(CDC5_VID), HIBYTE(CDC5_VID),           /* idVendor */
    LOBYTE(CDC5_PID), HIBYTE(CDC5_PID),           /* idProduct */
    0x00, 0x01,                                   /* bcdDevice */
    0x01,                                         /* iManufacture */
    0x02,                                         /* iProduct */
    0x03,                                         /* iSerial */
    0x01,                                         /* bNumConfigurations */
};

// Configs 
#define  CDC5_CONFIG_DESCRIPTOR_SIZE  (339)
__ALIGN_BEGIN const uint8_t CDC5_ConfigDescriptor [339] __ALIGN_END = {
    0x09,                                         /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,            /* bDescriptorType */
    0x53, 0x01,                                   /* wTotalLength */
    0x0a,                                         /* bNumInterfaces */
    0x01,                                         /* bConfigurationValue */
    0x00,                                         /* iConfiguration */
    0x80,                                         /* bmAttributes */
    0x64,                                         /* bMaxPower */
  /* IAD descriptor */
    0x08,                                         /* bLength */
    USB_IAD_DESCRIPTOR_TYPE,                      /* bDescriptorType */
    0x00,                                         /* bFirstInterface */
    0x02,                                         /* bInterfaceCount */
    0x02,                                         /* bFunctionClass */
    0x02,                                         /* bFunctionSubClass */
    0x01,                                         /* bFunctionProtocol */
    0x00,                                         /* iFunction */
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
    0x88,                                         /* bEndpointAddress */
    0x03,                                         /* bmAttributes */
    0x08, 0x00,                                   /* wMaxPacketSize */
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
    0x01,                                         /* bEndpointAddress */
    0x02,                                         /* bmAttributes */
    0x40, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
  /* IAD descriptor */
    0x08,                                         /* bLength */
    USB_IAD_DESCRIPTOR_TYPE,                      /* bDescriptorType */
    0x02,                                         /* bFirstInterface */
    0x02,                                         /* bInterfaceCount */
    0x02,                                         /* bFunctionClass */
    0x02,                                         /* bFunctionSubClass */
    0x01,                                         /* bFunctionProtocol */
    0x00,                                         /* iFunction */
    /* Interface descriptor, len: 35*/
    0x09,                                         /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                /* bDescriptorType */
    0x02,                                         /* bInterfaceNumber */
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
    0x89,                                         /* bEndpointAddress */
    0x03,                                         /* bmAttributes */
    0x08, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
    /* Interface descriptor, len: 23*/
    0x09,                                         /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                /* bDescriptorType */
    0x03,                                         /* bInterfaceNumber */
    0x00,                                         /* bAlternateSetting */
    0x02,                                         /* bNumEndpoints */
    0x0a,                                         /* bInterfaceClass */
    0x00,                                         /* bInterfaceSubClass */
    0x00,                                         /* bInterfaceProtocol */
    0x00,                                         /* iInterface */
    /* EndPoint descriptor */
    0x07,                                         /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                 /* bDescriptorType */
    0x82,                                         /* bEndpointAddress */
    0x02,                                         /* bmAttributes */
    0x20, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
    /* EndPoint descriptor */
    0x07,                                         /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                 /* bDescriptorType */
    0x02,                                         /* bEndpointAddress */
    0x02,                                         /* bmAttributes */
    0x20, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
  /* IAD descriptor */
    0x08,                                         /* bLength */
    USB_IAD_DESCRIPTOR_TYPE,                      /* bDescriptorType */
    0x04,                                         /* bFirstInterface */
    0x02,                                         /* bInterfaceCount */
    0x02,                                         /* bFunctionClass */
    0x02,                                         /* bFunctionSubClass */
    0x01,                                         /* bFunctionProtocol */
    0x00,                                         /* iFunction */
    /* Interface descriptor, len: 35*/
    0x09,                                         /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                /* bDescriptorType */
    0x04,                                         /* bInterfaceNumber */
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
    0x8a,                                         /* bEndpointAddress */
    0x03,                                         /* bmAttributes */
    0x08, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
    /* Interface descriptor, len: 23*/
    0x09,                                         /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                /* bDescriptorType */
    0x05,                                         /* bInterfaceNumber */
    0x00,                                         /* bAlternateSetting */
    0x02,                                         /* bNumEndpoints */
    0x0a,                                         /* bInterfaceClass */
    0x00,                                         /* bInterfaceSubClass */
    0x00,                                         /* bInterfaceProtocol */
    0x00,                                         /* iInterface */
    /* EndPoint descriptor */
    0x07,                                         /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                 /* bDescriptorType */
    0x83,                                         /* bEndpointAddress */
    0x02,                                         /* bmAttributes */
    0x20, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
    /* EndPoint descriptor */
    0x07,                                         /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                 /* bDescriptorType */
    0x03,                                         /* bEndpointAddress */
    0x02,                                         /* bmAttributes */
    0x20, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
  /* IAD descriptor */
    0x08,                                         /* bLength */
    USB_IAD_DESCRIPTOR_TYPE,                      /* bDescriptorType */
    0x06,                                         /* bFirstInterface */
    0x02,                                         /* bInterfaceCount */
    0x02,                                         /* bFunctionClass */
    0x02,                                         /* bFunctionSubClass */
    0x01,                                         /* bFunctionProtocol */
    0x00,                                         /* iFunction */
    /* Interface descriptor, len: 35*/
    0x09,                                         /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                /* bDescriptorType */
    0x06,                                         /* bInterfaceNumber */
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
    0x8b,                                         /* bEndpointAddress */
    0x03,                                         /* bmAttributes */
    0x08, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
    /* Interface descriptor, len: 23*/
    0x09,                                         /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                /* bDescriptorType */
    0x07,                                         /* bInterfaceNumber */
    0x00,                                         /* bAlternateSetting */
    0x02,                                         /* bNumEndpoints */
    0x0a,                                         /* bInterfaceClass */
    0x00,                                         /* bInterfaceSubClass */
    0x00,                                         /* bInterfaceProtocol */
    0x00,                                         /* iInterface */
    /* EndPoint descriptor */
    0x07,                                         /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                 /* bDescriptorType */
    0x84,                                         /* bEndpointAddress */
    0x02,                                         /* bmAttributes */
    0x10, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
    /* EndPoint descriptor */
    0x07,                                         /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                 /* bDescriptorType */
    0x04,                                         /* bEndpointAddress */
    0x02,                                         /* bmAttributes */
    0x10, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
  /* IAD descriptor */
    0x08,                                         /* bLength */
    USB_IAD_DESCRIPTOR_TYPE,                      /* bDescriptorType */
    0x08,                                         /* bFirstInterface */
    0x02,                                         /* bInterfaceCount */
    0x02,                                         /* bFunctionClass */
    0x02,                                         /* bFunctionSubClass */
    0x01,                                         /* bFunctionProtocol */
    0x00,                                         /* iFunction */
    /* Interface descriptor, len: 35*/
    0x09,                                         /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                /* bDescriptorType */
    0x08,                                         /* bInterfaceNumber */
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
    0x8c,                                         /* bEndpointAddress */
    0x03,                                         /* bmAttributes */
    0x08, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
    /* Interface descriptor, len: 23*/
    0x09,                                         /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                /* bDescriptorType */
    0x09,                                         /* bInterfaceNumber */
    0x00,                                         /* bAlternateSetting */
    0x02,                                         /* bNumEndpoints */
    0x0a,                                         /* bInterfaceClass */
    0x00,                                         /* bInterfaceSubClass */
    0x00,                                         /* bInterfaceProtocol */
    0x00,                                         /* iInterface */
    /* EndPoint descriptor */
    0x07,                                         /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                 /* bDescriptorType */
    0x85,                                         /* bEndpointAddress */
    0x02,                                         /* bmAttributes */
    0x10, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
    /* EndPoint descriptor */
    0x07,                                         /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                 /* bDescriptorType */
    0x05,                                         /* bEndpointAddress */
    0x02,                                         /* bmAttributes */
    0x10, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
};

// Power status
#define CDC5_DEV_STATUS0      (0)
#define CDC5_DEV_STATUS1      (0)
#define CDC5_DEV_STATUS    ((CDC5_DEV_STATUS0) |(CDC5_DEV_STATUS1) )

CDC5_TXEP_MAX_SIZE
CDC5_RXEP_MAX_SIZE

// Strings 
#define CDC5_STRING_DESCRIPTOR0_STR          "\x09\x04"
#define CDC5_STRING_DESCRIPTOR0_SIZE          (4)
WEAK __ALIGN_BEGIN const uint8_t  CDC5_StringDescriptor0 [4] __ALIGN_END = {
    0x04,                                         /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    0x09, 0x04,                                   /* wLangID0 */
};
#define CDC5_STRING_DESCRIPTOR1_STR          "TeenyUSB"
#define CDC5_STRING_DESCRIPTOR1_SIZE          (18)
WEAK __ALIGN_BEGIN const uint8_t  CDC5_StringDescriptor1 [18] __ALIGN_END = {
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
#define CDC5_STRING_DESCRIPTOR2_STR          "TeenyUSB CDC"
#define CDC5_STRING_DESCRIPTOR2_SIZE          (26)
WEAK __ALIGN_BEGIN const uint8_t  CDC5_StringDescriptor2 [26] __ALIGN_END = {
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
#define CDC5_STRING_DESCRIPTOR3_STR          "TUSB123456"
#define CDC5_STRING_DESCRIPTOR3_SIZE          (22)
WEAK __ALIGN_BEGIN const uint8_t  CDC5_StringDescriptor3 [22] __ALIGN_END = {
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
const uint8_t* const CDC5_StringDescriptors[CDC5_STRING_COUNT] = {
CDC5_StringDescriptor0,
CDC5_StringDescriptor1,
CDC5_StringDescriptor2,
CDC5_StringDescriptor3,
};

//  Device descriptors
const tusb_descriptors CDC5_descriptors = {
  .device = CDC5_DeviceDescriptor,
  .config = CDC5_ConfigDescriptor,
  .strings = CDC5_StringDescriptors,
  .string_cnt = CDC5_STRING_COUNT,
#if defined(HAS_WCID)
#if defined(CDC5_WCID_DESCRIPTOR_SIZE)
  .wcid_desc = CDC5_WCIDDescriptor,
#else
  .wcid_desc = 0,  
#endif // CDC5_WCID_DESCRIPTOR_SIZE)

#if defined(CDC5_WCID_PROPERTIES_SIZE)
  .wcid_properties = CDC5_WCIDProperties,
#else
  .wcid_properties = 0,  
#endif // CDC5_WCID_PROPERTIES_SIZE

#endif // HAS_WCID
};


