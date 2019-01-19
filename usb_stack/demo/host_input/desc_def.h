#ifndef __DESC_DEF_H__
#define __DESC_DEF_H__

/* bmRequestType :D7 Data Phase Transfer Direction  */
#define  USB_REQ_DIR_MASK                                  0x80U
#define  USB_H2D                                           0x00U
#define  USB_D2H                                           0x80U

#define  USBH_MAX_NUM_INTERFACES   1
#define  USBH_MAX_NUM_ENDPOINTS    2

typedef  struct  _DescHeader
{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
}
USBH_DescHeader_t;

typedef struct _DeviceDescriptor
{
  uint8_t   bLength;
  uint8_t   bDescriptorType;
  uint16_t  bcdUSB;        /* USB Specification Number which device complies too */
  uint8_t   bDeviceClass;
  uint8_t   bDeviceSubClass;
  uint8_t   bDeviceProtocol;
  /* If equal to Zero, each interface specifies its own class
  code if equal to 0xFF, the class code is vendor specified.
  Otherwise field is valid Class Code.*/
  uint8_t   bMaxPacketSize;
  uint16_t  idVendor;      /* Vendor ID (Assigned by USB Org) */
  uint16_t  idProduct;     /* Product ID (Assigned by Manufacturer) */
  uint16_t  bcdDevice;     /* Device Release Number */
  uint8_t   iManufacturer;  /* Index of Manufacturer String Descriptor */
  uint8_t   iProduct;       /* Index of Product String Descriptor */
  uint8_t   iSerialNumber;  /* Index of Serial Number String Descriptor */
  uint8_t   bNumConfigurations; /* Number of Possible Configurations */
}
USBH_DevDescTypeDef;

typedef struct __attribute__ ((packed)) _EndpointDescriptor
{
  uint8_t   bLength;
  uint8_t   bDescriptorType;
  uint8_t   bEndpointAddress;   /* indicates what endpoint this descriptor is describing */
  uint8_t   bmAttributes;       /* specifies the transfer type. */
  uint16_t  wMaxPacketSize;    /* Maximum Packet Size this endpoint is capable of sending or receiving */
  uint8_t   bInterval;          /* is used to specify the polling interval of certain transfers. */
}
USBH_EpDescTypeDef;

typedef struct __attribute__ ((packed)) _InterfaceDescriptor
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bInterfaceNumber;
  uint8_t bAlternateSetting;    /* Value used to select alternative setting */
  uint8_t bNumEndpoints;        /* Number of Endpoints used for this interface */
  uint8_t bInterfaceClass;      /* Class Code (Assigned by USB Org) */
  uint8_t bInterfaceSubClass;   /* Subclass Code (Assigned by USB Org) */
  uint8_t bInterfaceProtocol;   /* Protocol Code */
  uint8_t iInterface;           /* Index of String Descriptor Describing this interface */
  USBH_EpDescTypeDef               Ep_Desc[USBH_MAX_NUM_ENDPOINTS];
}
USBH_InterfaceDescTypeDef;


typedef struct __attribute__ ((packed)) _ConfigurationDescriptor
{
  uint8_t   bLength;
  uint8_t   bDescriptorType;
  uint16_t  wTotalLength;        /* Total Length of Data Returned */
  uint8_t   bNumInterfaces;       /* Number of Interfaces */
  uint8_t   bConfigurationValue;  /* Value to use as an argument to select this configuration*/
  uint8_t   iConfiguration;       /*Index of String Descriptor Describing this configuration */
  uint8_t   bmAttributes;         /* D7 Bus Powered , D6 Self Powered, D5 Remote Wakeup , D4..0 Reserved (0)*/
  uint8_t   bMaxPower;            /*Maximum Power Consumption */
  USBH_InterfaceDescTypeDef        Itf_Desc[USBH_MAX_NUM_INTERFACES];
}
USBH_CfgDescTypeDef;

// USB2.0  11.23.2.1
typedef struct __attribute__ ((packed)) _USB_HUB_DESCRIPTOR
{
	uint8_t  bLength;               // Length of this descriptor.
	uint8_t  bDescriptorType;       // Descriptor Type, value: 29H for hub descriptor
	uint8_t  bNbrPorts;             // Number of downstream facing ports that this hub supports
	uint16_t wHubCharacteristics;   //
  uint8_t  bPwrOn2PwrGood;        // Time (in 2 ms intervals) from the time the power-on sequence begins on a port until power is good on that port
  uint8_t  bHubContrCurrent;      // Maximum current requirements of the Hub Controller electronics in mA
  uint8_t  DeviceRemovable;       // Indicates if a port has a removable device attached.
  uint8_t  PortPwrCtrlMask;       // This field exists for reasons of compatibility with software written for 1.0 compliant devices.
} USB_HUB_DESCRIPTOR;

// USB 2.0 11.24.2.7
typedef struct __attribute__ ((packed)) _USB_HUB_PORT_STATUS
{
  struct __attribute__ ((packed)) _port_status{
    uint8_t     PORT_CONNECTION      : 1;
    uint8_t     PORT_ENABLE          : 1;
    uint8_t     PORT_SUSPEND         : 1;
    uint8_t     PORT_OVER_CURRENT    : 1;
    uint8_t     PORT_RESET           : 1;
    uint8_t     RESERVED_1           : 3;
    uint8_t     PORT_POWER           : 1;
    uint8_t     PORT_LOW_SPEED       : 1;
    uint8_t     PORT_HIGH_SPEED      : 1;
    uint8_t     PORT_TEST            : 1;
    uint8_t     PORT_INDICATOR       : 1;
    uint8_t     RESERVED_2           : 3;
  }wPortStatus;
  struct __attribute__ ((packed)) _port_change{
    uint8_t     C_PORT_CONNECTION    : 1;
    uint8_t     C_PORT_ENABLE        : 1;
    uint8_t     C_PORT_SUSPEND       : 1;
    uint8_t     C_PORT_OVER_CURRENT  : 1;
    uint8_t     C_PORT_RESET         : 1;
    uint16_t    RESERVED             : 11;
  }wPortChange;
}USB_HUB_PORT_STATUS;






#define USB_HUB_CLASS     					 0x09
#define USB_HID_CLASS                0x03
#define USB_USER_CALSS               0xff

/* Interface Descriptor field values for HID Boot Protocol */
#define HID_BOOT_CODE                                  0x01    
#define HID_KEYBRD_BOOT_CODE                           0x01
#define HID_MOUSE_BOOT_CODE                            0x02


#define USB_HID_GET_REPORT           0x01
#define USB_HID_GET_IDLE             0x02
#define USB_HID_GET_PROTOCOL         0x03
#define USB_HID_SET_REPORT           0x09
#define USB_HID_SET_IDLE             0x0A
#define USB_HID_SET_PROTOCOL         0x0B    


#define USB_HID_INPUT_REPORT    0x01
#define USB_HID_OUTPUT_REPORT   0x02
#define USB_HID_FEATURE_REPORT  0x03

#define  USB_REQ_RECIPIENT_OTHER       0x03


#define USB_DESCRIPTOR_HUB                   0x29
#define USB_REQUEST_GET_DESCRIPTOR           0x06

#define HUB_FEATURE_SEL_PORT_CONN         		 0x00
#define HUB_FEATURE_SEL_PORT_ENABLE             0x01
#define HUB_FEATURE_SEL_PORT_SUSPEND            0x02
#define HUB_FEATURE_SEL_PORT_OVER_CURRENT       0x03
#define HUB_FEATURE_SEL_PORT_RESET              0x04
#define HUB_FEATURE_SEL_PORT_POWER              0x08
#define HUB_FEATURE_SEL_PORT_LOW_SPEED          0x09
#define HUB_FEATURE_SEL_C_PORT_CONNECTION       0x10
#define HUB_FEATURE_SEL_C_PORT_ENABLE           0x11
#define HUB_FEATURE_SEL_C_PORT_SUSPEND          0x12
#define HUB_FEATURE_SEL_C_PORT_OVER_CURRENT     0x13
#define HUB_FEATURE_SEL_C_PORT_RESET            0x14
#define HUB_FEATURE_SEL_PORT_INDICATOR          0x16





#endif
