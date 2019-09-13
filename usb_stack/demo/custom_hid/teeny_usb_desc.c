/*
 * Name   :  teeny_usb_desc.c
 * Author :  admin@xtoolbox.org
 * Date   :  2019-09-13 20:54:34
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
            -- report = HID_InOut(64),
        },
   }
}  

  ------------- lua script end   ------------
 */
#include "teeny_usb.h"
#define  HID_DEVICE_DESCRIPTOR_SIZE  (18)
__ALIGN_BEGIN  const uint8_t HID_DeviceDescriptor [18] __ALIGN_END = {
    0x12,                                         /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    0x00, 0x02,                                   /* bcdUSB */
    0x00,                                         /* bDeviceClass */
    0x00,                                         /* bDeviceSubClass */
    0x00,                                         /* bDeviceProtocol */
    0x40,                                         /* bMaxPacketSize */
    LOBYTE(HID_VID), HIBYTE(HID_VID),             /* idVendor */
    LOBYTE(HID_PID), HIBYTE(HID_PID),             /* idProduct */
    0x00, 0x01,                                   /* bcdDevice */
    0x01,                                         /* iManufacture */
    0x02,                                         /* iProduct */
    0x03,                                         /* iSerial */
    0x01,                                         /* bNumConfigurations */
};

// Configs 
#define HID_REPORT_DESCRIPTOR_SIZE_IF0 24
WEAK __ALIGN_BEGIN const uint8_t HID_ReportDescriptor_if0[HID_REPORT_DESCRIPTOR_SIZE_IF0] __ALIGN_END = {

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

};

#define  HID_CONFIG_DESCRIPTOR_SIZE  (41)
__ALIGN_BEGIN const uint8_t HID_ConfigDescriptor [41] __ALIGN_END = {
    0x09,                                         /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,            /* bDescriptorType */
    0x29, 0x00,                                   /* wTotalLength */
    0x01,                                         /* bNumInterfaces */
    0x01,                                         /* bConfigurationValue */
    0x00,                                         /* iConfiguration */
    0x80,                                         /* bmAttributes */
    0x64,                                         /* bMaxPower */
    /* Interface descriptor, len: 32*/
    0x09,                                         /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                /* bDescriptorType */
    0x00,                                         /* bInterfaceNumber */
    0x00,                                         /* bAlternateSetting */
    0x02,                                         /* bNumEndpoints */
    0x03,                                         /* bInterfaceClass */
    0x00,                                         /* bInterfaceSubClass */
    0x00,                                         /* bInterfaceProtocol */
    0x00,                                         /* iInterface */
    /* HID descriptor  */
    0x09,                                         /* bLength */
    0x21,                                         /* bDescriptorType */
    0x11, 0x01,                                   /* bcdHID */
    0x00,                                         /* bCountryCode */
    0x01,                                         /* bNumDescriptors */
    0x22,                                         /* bDescriptorType1 */
    0x18, 0x00,                                   /* wDescriptorLength1 */
    /* EndPoint descriptor */
    0x07,                                         /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                 /* bDescriptorType */
    0x81,                                         /* bEndpointAddress */
    0x03,                                         /* bmAttributes */
    0x40, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
    /* EndPoint descriptor */
    0x07,                                         /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                 /* bDescriptorType */
    0x01,                                         /* bEndpointAddress */
    0x03,                                         /* bmAttributes */
    0x40, 0x00,                                   /* wMaxPacketSize */
    0x01,                                         /* bInterval */
};

// Power status
#define HID_DEV_STATUS0      (0)
#define HID_DEV_STATUS1      (0)
#define HID_DEV_STATUS    ((HID_DEV_STATUS0) |(HID_DEV_STATUS1) )

HID_TXEP_MAX_SIZE
HID_RXEP_MAX_SIZE

// Strings 
#define HID_STRING_DESCRIPTOR0_STR          "\x09\x04"
#define HID_STRING_DESCRIPTOR0_SIZE          (4)
WEAK __ALIGN_BEGIN const uint8_t  HID_StringDescriptor0 [4] __ALIGN_END = {
    0x04,                                         /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    0x09, 0x04,                                   /* wLangID0 */
};
#define HID_STRING_DESCRIPTOR1_STR          "TeenyUSB"
#define HID_STRING_DESCRIPTOR1_SIZE          (18)
WEAK __ALIGN_BEGIN const uint8_t  HID_StringDescriptor1 [18] __ALIGN_END = {
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
#define HID_STRING_DESCRIPTOR2_STR          "TeenyUSB Custom HID"
#define HID_STRING_DESCRIPTOR2_SIZE          (40)
WEAK __ALIGN_BEGIN const uint8_t  HID_StringDescriptor2 [40] __ALIGN_END = {
    0x28,                                         /* bLength */
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
    'u', 0x00,                                    /* wcChar10 */
    's', 0x00,                                    /* wcChar11 */
    't', 0x00,                                    /* wcChar12 */
    'o', 0x00,                                    /* wcChar13 */
    'm', 0x00,                                    /* wcChar14 */
    ' ', 0x00,                                    /* wcChar15 */
    'H', 0x00,                                    /* wcChar16 */
    'I', 0x00,                                    /* wcChar17 */
    'D', 0x00,                                    /* wcChar18 */
};
#define HID_STRING_DESCRIPTOR3_STR          "TUSB123456"
#define HID_STRING_DESCRIPTOR3_SIZE          (22)
WEAK __ALIGN_BEGIN const uint8_t  HID_StringDescriptor3 [22] __ALIGN_END = {
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
const uint8_t* const HID_StringDescriptors[HID_STRING_COUNT] = {
HID_StringDescriptor0,
HID_StringDescriptor1,
HID_StringDescriptor2,
HID_StringDescriptor3,
};

//  Device descriptors
const tusb_descriptors HID_descriptors = {
  .device = HID_DeviceDescriptor,
  .config = HID_ConfigDescriptor,
  .strings = HID_StringDescriptors,
  .string_cnt = HID_STRING_COUNT,
#if defined(HAS_WCID)
#if defined(HID_WCID_DESCRIPTOR_SIZE)
  .wcid_desc = HID_WCIDDescriptor,
#else
  .wcid_desc = 0,  
#endif // HID_WCID_DESCRIPTOR_SIZE)

#if defined(HID_WCID_PROPERTIES_SIZE)
  .wcid_properties = HID_WCIDProperties,
#else
  .wcid_properties = 0,  
#endif // HID_WCID_PROPERTIES_SIZE

#endif // HAS_WCID

#if defined(HAS_WCID_20)
#if defined(HID_WCID_BOS_SIZE)
  .wcid_bos = HID_WCIDBOS,
#else
  .wcid_bos = 0,  
#endif // HID_WCID_BOS_SIZE)

#if defined(HID_WCID_DESC_SET_SIZE)
  .wcid_desc_set = HID_WCIDDescriptorSet,
#else
  .wcid_desc_set = 0,  
#endif // HID_WCID_DESC_SET_SIZE


#endif // HAS_WCID_20
};


