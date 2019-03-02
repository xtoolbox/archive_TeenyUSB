/*       
 *         _______                    _    _  _____ ____  
 *        |__   __|                  | |  | |/ ____|  _ \ 
 *           | | ___  ___ _ __  _   _| |  | | (___ | |_) |
 *           | |/ _ \/ _ \ '_ \| | | | |  | |\___ \|  _ < 
 *           | |  __/  __/ | | | |_| | |__| |____) | |_) |
 *           |_|\___|\___|_| |_|\__, |\____/|_____/|____/ 
 *                               __/ |                    
 *                              |___/                     
 *
 * TeenyUSB - light weight usb stack for STM32 micro controllers
 * 
 * Copyright (c) 2019 XToolBox  - admin@xtoolbox.org
 *                         www.tusb.org
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "teeny_usb.h"
#include "desc_def.h"
#include "string.h"

typedef struct _usb_device usb_device_t;

typedef struct _control_pipe
{
  tusb_pipe_t ctrl_in;
  tusb_pipe_t ctrl_out;
}control_pipe_t;

typedef struct _usb_interface
{
  tusb_pipe_t data_in;
  tusb_pipe_t data_out;
  void (*init)(usb_device_t* dev);
  void (*deinit)(usb_device_t* dev);
  void (*handle)(usb_device_t* dev);
  uint16_t  in_len;
  uint16_t  out_len;
  uint8_t   in_buf[64];
  uint8_t   out_buf[64];
}usb_interface_t;

#define MAX_CHILD   4
struct _usb_device{
  tusb_host_t* host;
  uint8_t  device_desc[32];
  uint8_t  config_desc[256];
  control_pipe_t  ctrl_pipe;
  usb_interface_t itf; // only one interface
  usb_device_t* children[MAX_CHILD]; // max 4 child
  uint16_t  config_len;
  uint8_t   addr;
  uint8_t   speed;
  uint8_t   is_root;
};


usb_device_t* new_device(void);
void free_device(usb_device_t* dev);

#define   GetDeviceDesc(ctrl_pipe, data, len) \
  control_xfer(ctrl_pipe, \
    USB_D2H | USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD, \
    USB_REQ_GET_DESCRIPTOR,\
    USB_DEVICE_DESCRIPTOR_TYPE<<8,\
    0,\
    data,\
    len)

#define  SetAddress(ctrl_pipe, addr)\
  control_xfer(ctrl_pipe, \
    USB_H2D | USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD, \
    USB_REQ_SET_ADDRESS,\
    addr,\
    0,\
    0,\
    0)
    
#define   GetConfigDesc(ctrl_pipe, data, len) \
  control_xfer(ctrl_pipe, \
    USB_D2H | USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD, \
    USB_REQ_GET_DESCRIPTOR,\
    USB_CONFIGURATION_DESCRIPTOR_TYPE<<8,\
    0,\
    data,\
    len)
    
#define  SetConfig(ctrl_pipe)\
  control_xfer(ctrl_pipe, \
    USB_H2D | USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD, \
    USB_REQ_SET_CONFIGURATION,\
    1,\
    0,\
    0,\
    0)

#define SetBootProtocol(ctrl_pipe)  \
  control_xfer(ctrl_pipe, \
    USB_H2D | USB_REQ_RECIPIENT_INTERFACE|USB_REQ_TYPE_CLASS,\
    USB_HID_SET_PROTOCOL,\
    0,\
    0,\
    0,\
    0)

#define  GetHUBDesc(ctrl_pipe, data, len) \
  control_xfer(ctrl_pipe, \
    USB_D2H | USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_CLASS, \
    USB_REQ_GET_DESCRIPTOR,\
    USB_DESCRIPTOR_HUB<<8,\
    0,\
    data,\
    len)


#define  SetHUBPort(ctrl_pipe, port, feature) \
  control_xfer(ctrl_pipe, \
    USB_H2D | USB_REQ_RECIPIENT_OTHER | USB_REQ_TYPE_CLASS, \
    USB_REQ_SET_FEATURE,\
    feature,\
    ((port)),\
    0,\
    0)

#define  SetHUBPortPower(ctrl_pipe, port) \
  control_xfer(ctrl_pipe, \
    USB_H2D | USB_REQ_RECIPIENT_OTHER | USB_REQ_TYPE_CLASS, \
    USB_REQ_SET_FEATURE,\
    HUB_FEATURE_SEL_PORT_POWER,\
    ((port)),\
    0,\
    0)

#define GetHUBPortStatus(ctrl_pipe, port, data, len) \
  control_xfer(ctrl_pipe, \
    USB_D2H | USB_REQ_RECIPIENT_OTHER | USB_REQ_TYPE_CLASS, \
    USB_REQ_GET_STATUS,\
    0,\
    ((port)),\
    data,\
    len)

#define  ClearHUBPort(ctrl_pipe, port, feature)  \
  control_xfer(ctrl_pipe, \
    USB_H2D | USB_REQ_RECIPIENT_OTHER | USB_REQ_TYPE_CLASS, \
    USB_REQ_CLEAR_FEATURE,\
    feature,\
    ((port)),\
    0,\
    0)

#define  SetReport(ctrl_pipe, data, len) \
  control_xfer(ctrl_pipe, \
    USB_H2D | USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_CLASS, \
    USB_HID_SET_REPORT,\
    USB_HID_OUTPUT_REPORT<<8,\
    0, \
    data,\
    len)


int xfer_with_wait(tusb_pipe_t* pipe, void* data, uint32_t len)
{
  channel_state_t state;
  tusb_pipe_xfer_data(pipe, data, len);
  state = tusb_pipe_wait(pipe, 500000);
  if( state == TUSB_CS_TRANSFER_COMPLETE ){
    return 0;
  }
  return -1;
}

int control_xfer(control_pipe_t* ctrl_pipe, uint8_t bmRequest, uint8_t bRequest, uint32_t value, uint32_t index, void* data, uint32_t len)
{
  static __ALIGN_BEGIN tusb_setup_packet setup __ALIGN_END;
  setup.bmRequestType = bmRequest;
  setup.bRequest = bRequest;
  setup.wValue = value;
  setup.wIndex = index;
  setup.wLength = len;
  tusb_pipe_setup(&ctrl_pipe->ctrl_out, &setup);
  // Setup packet never fail
  if( tusb_pipe_wait(&ctrl_pipe->ctrl_out, 30000) != TUSB_CS_TRANSFER_COMPLETE){
    return -1;
  }
  
  tusb_delay_ms(500);
  if(len){
    if(bmRequest & 0x80){
      //Read data
      if(xfer_with_wait(&ctrl_pipe->ctrl_in, data, len) != 0){
        tusb_delay_ms(500);
        if(xfer_with_wait(&ctrl_pipe->ctrl_in, data, len) != 0){
          return -1;
        }
      }
      tusb_delay_ms(50);
      // send status out packet
      if(xfer_with_wait(&ctrl_pipe->ctrl_out, 0, 0) != 0){
        tusb_delay_ms(500);
        if(xfer_with_wait(&ctrl_pipe->ctrl_out, 0, 0) != 0){
          return -1;
        }
      }
    }else{
      // Write data
      if(xfer_with_wait(&ctrl_pipe->ctrl_out, data, len) != 0){
        tusb_delay_ms(500);
        if(xfer_with_wait(&ctrl_pipe->ctrl_out, data, len) != 0){
          return -1;
        }
      }
      tusb_delay_ms(50);
      // recv the status in packet
      if(xfer_with_wait(&ctrl_pipe->ctrl_in, &setup, 0) != 0){
        tusb_delay_ms(500);
        if(xfer_with_wait(&ctrl_pipe->ctrl_in, &setup, 0) != 0){
          return -1;
        }
      }
    }
  }else{
    // recv the status in packet
      if(xfer_with_wait(&ctrl_pipe->ctrl_in, &setup, 0) != 0){
        tusb_delay_ms(500);
        if(xfer_with_wait(&ctrl_pipe->ctrl_in, &setup, 0) != 0){
          return -1;
        }
      }
  }
  return 0;
}



#define LED_NUM_LOCK   1
#define LED_CAPS_LOCK  2
#define KEY_NUM_LOCK   0x53
#define KEY_CAPS_LOCK  0x39

__IO uint8_t kbd_led = 0;


void mouse_handle(usb_device_t* device)
{
  static __IO uint32_t tick = 0;
  channel_state_t state = tusb_pipe_wait(&device->itf.data_in, 0);
  if(state == TUSB_CS_TRANSFER_COMPLETE) {
    // get mouse data
    if(device->itf.in_buf[0] & 1){
      // left key down, toggle num lock
      kbd_led ^= LED_NUM_LOCK;
    }
    if(device->itf.in_buf[0] & 2){
      // right key down, toggle caps lock
      kbd_led ^= LED_CAPS_LOCK;
    }
    tusb_pipe_xfer_data(&device->itf.data_in, device->itf.in_buf, device->itf.in_len);
  }else if(state != TUSB_CS_XFER_ONGOING){
    tick++;
    if(tick>2000){
      tick = 0;
      tusb_pipe_xfer_data(&device->itf.data_in, device->itf.in_buf, device->itf.in_len);
    }
  }
}

void kbd_handle(usb_device_t* device)
{
  static __IO uint32_t tick = 0;
  channel_state_t state = tusb_pipe_wait(&device->itf.data_in, 0);
  if(state == TUSB_CS_TRANSFER_COMPLETE) {
    // get key board data
    if(device->itf.in_buf[2] == KEY_NUM_LOCK){
      // Num Lock
      kbd_led ^= LED_NUM_LOCK;
    }
    if(device->itf.in_buf[2] == KEY_CAPS_LOCK){
      // Caps lock
      kbd_led ^= LED_CAPS_LOCK;
    }
    tusb_pipe_xfer_data(&device->itf.data_in, device->itf.in_buf, device->itf.in_len);
  }else if(state != TUSB_CS_XFER_ONGOING){
    tick++;
    if(tick>2000){
      tick = 0;
      tusb_pipe_xfer_data(&device->itf.data_in, device->itf.in_buf, device->itf.in_len);
    }
  }
}

void user_handle(usb_device_t* device)
{
  
}

int enum_device(tusb_host_t* host, usb_device_t* device);
__ALIGN_BEGIN USB_HUB_PORT_STATUS port_status __ALIGN_END;
void hub_handle(usb_device_t* device)
{
  channel_state_t state = tusb_pipe_wait(&device->itf.data_in, 0);
  if(state == TUSB_CS_TRANSFER_COMPLETE){
    uint8_t port_state = device->itf.in_buf[0];
    uint8_t port = 0;
    while(port_state){
      // some port changed
      if(port_state & 1){
        // handle port change here
        if (GetHUBPortStatus(&device->ctrl_pipe, port, &port_status, sizeof(port_status)) == 0){
          // Get port status success
          if(port_status.wPortStatus.PORT_POWER) {
            // port powered
            uint8_t r = 0;
            if(port_status.wPortChange.C_PORT_CONNECTION){
              r += ClearHUBPort(&device->ctrl_pipe, port, HUB_FEATURE_SEL_C_PORT_CONNECTION);
            }
            if(port_status.wPortChange.C_PORT_ENABLE){
              r += ClearHUBPort(&device->ctrl_pipe, port, HUB_FEATURE_SEL_C_PORT_ENABLE);
            }
            if(port_status.wPortChange.C_PORT_SUSPEND){
              r += ClearHUBPort(&device->ctrl_pipe, port, HUB_FEATURE_SEL_C_PORT_SUSPEND);
            }
            if(port_status.wPortChange.C_PORT_OVER_CURRENT){
              r += ClearHUBPort(&device->ctrl_pipe, port, HUB_FEATURE_SEL_C_PORT_OVER_CURRENT);
            }
            if(port_status.wPortChange.C_PORT_RESET){
              r += ClearHUBPort(&device->ctrl_pipe, port, HUB_FEATURE_SEL_C_PORT_RESET);
            }
            if(port_status.wPortStatus.PORT_CONNECTION){
              if(port_status.wPortStatus.PORT_ENABLE){
                // Enable means reset done
                usb_device_t* dev = new_device();
                if(dev){
                  dev->is_root = 0;
                  dev->speed = PORT_SPEED_FULL;
                  if(port_status.wPortStatus.PORT_HIGH_SPEED){
                    dev->speed = PORT_SPEED_HIGH;
                  }
                  if(port_status.wPortStatus.PORT_LOW_SPEED){
                    dev->speed = PORT_SPEED_LOW;
                  }
                  if(enum_device(device->host, dev) == 0){
                    usb_device_t* child = device->children[port-1];
                    if(child){
                      free_device(child);
                    }
                    device->children[port-1] = dev;
                  }else{
                    free_device(dev);
                  }
                }
              }else{
                // not enable, need a reset
                r += SetHUBPort(&device->ctrl_pipe, port, HUB_FEATURE_SEL_PORT_RESET);
              }
            }else{
              usb_device_t* child = device->children[port-1];
              if(child){
                free_device(child);
              }
              device->children[port-1] = 0;
            }
          }
        }
      }
      port++;
      port_state >>= 1;
    }
  }else if(state == TUSB_CS_STALL){
    // TODO: clear interface feature
  }
  
  if(state != TUSB_CS_XFER_ONGOING){
    static __IO uint32_t tick = 0;
    tick++;
    if(tick>2000){
      tick = 0;
      tusb_pipe_xfer_data(&device->itf.data_in, device->itf.in_buf, device->itf.in_len);
    }
  }
  
  for(int i=0;i<MAX_CHILD;i++){
    usb_device_t* child = device->children[i];
    if(child && child->itf.handle){
      child->itf.handle(child);
    }
  }
}

void close_ep(usb_device_t* device)
{
  tusb_pipe_close(&device->ctrl_pipe.ctrl_in);
  tusb_pipe_close(&device->ctrl_pipe.ctrl_out);
  tusb_pipe_close(&device->itf.data_in);
  tusb_pipe_close(&device->itf.data_out);
  device->itf.deinit = 0;
}

void hub_deinit(usb_device_t* device)
{
  int i;
  close_ep(device);
  for(i=0;i<MAX_CHILD;i++){
    usb_device_t* child = device->children[i];
    if(child){
      if(child->itf.deinit){
        child->itf.deinit(child);
      }
    }
  }
  device->itf.deinit = 0;
}

static int open_ep(usb_device_t* device)
{
  USBH_CfgDescTypeDef* cfg = (USBH_CfgDescTypeDef*)device->config_desc;
  uint8_t in = 0;
  uint8_t out = 0;
  for(int i=0;i<cfg->Itf_Desc[0].bNumEndpoints;i++){
    USBH_EpDescTypeDef* ep = &cfg->Itf_Desc[0].Ep_Desc[i];
    if( ep->bEndpointAddress & 0x80 ){
      if(in == 0){
        if(tusb_pipe_open(device->host, 
                          &device->itf.data_in, 
                          device->addr, 
                          ep->bEndpointAddress, 
                          ep->bmAttributes & EP_TYPE_MSK, 
                          ep->wMaxPacketSize & 0x7ff,
                          device->speed) != 0){
          goto open_ep_fail;
        }
        device->itf.in_len = ep->wMaxPacketSize & 0x7ff;
      }
      in = 1;
    }else{
      if(out == 0){
        if(tusb_pipe_open(device->host, 
                          &device->itf.data_out,
                          device->addr,
                          ep->bEndpointAddress,
                          ep->bmAttributes & EP_TYPE_MSK, 
                          ep->wMaxPacketSize & 0x7ff,
                          device->speed) != 0){
          goto open_ep_fail;
        }
        device->itf.out_len = ep->wMaxPacketSize & 0x7ff;
      }
    }
  }
  return 0;
open_ep_fail:
  if(in) {
    tusb_pipe_close(&device->itf.data_in);
  }
  if(out) {
    tusb_pipe_close(&device->itf.data_out);
  }
  return -1;
}

int init_hid(usb_device_t* device)
{
  USBH_CfgDescTypeDef* cfg = (USBH_CfgDescTypeDef*)device->config_desc;
  device->itf.handle = 0;
  if(cfg->Itf_Desc[0].bInterfaceProtocol == HID_KEYBRD_BOOT_CODE){
    device->itf.handle = kbd_handle;
    
  }else if(cfg->Itf_Desc[0].bInterfaceProtocol == HID_MOUSE_BOOT_CODE){
    device->itf.handle = mouse_handle;
    device->itf.in_len = 4; // boot mouse report length is 
  }
  // TODO: parse the interfce descriptor to get the endpoint descritptor
  memcpy(cfg->Itf_Desc[0].Ep_Desc, (uint8_t*)cfg->Itf_Desc[0].Ep_Desc + 9, 14);

  if(device->itf.handle){
    if(open_ep(device) != 0){
      goto hid_fail;
    }
    device->itf.deinit = close_ep;
    if(SetBootProtocol(&device->ctrl_pipe) != 0){
      goto hid_fail;
    }
    tusb_pipe_xfer_data(&device->itf.data_in, device->itf.in_buf, device->itf.in_len);
  }
  return 0;
hid_fail:
  return -1;
}

int init_user(usb_device_t* device)
{
  device->itf.handle = user_handle;
  open_ep(device);
  device->itf.deinit = close_ep;
  return 0;
}

__ALIGN_BEGIN USB_HUB_DESCRIPTOR hubDesc __ALIGN_END;
int init_hub(usb_device_t* device)
{
  device->itf.handle = hub_handle;
  if(open_ep(device) != 0){
    goto hub_fail;
  }
  device->itf.deinit = hub_deinit;
  if(GetHUBDesc(&device->ctrl_pipe, &hubDesc, sizeof(hubDesc)) != 0){
    goto hub_fail;
  }
  
  for(uint8_t i=1;i<=hubDesc.bNbrPorts;i++){
    if(SetHUBPortPower(&device->ctrl_pipe, i) != 0){
      goto hub_fail;
    }
  }
  tusb_delay_ms(hubDesc.bPwrOn2PwrGood*2);
  
  tusb_pipe_xfer_data(&device->itf.data_in, device->itf.in_buf, device->itf.in_len);
  
  return 0;
hub_fail:
  return -1;
}

int init_interface(usb_device_t* device)
{
  USBH_CfgDescTypeDef* cfg = (USBH_CfgDescTypeDef*)device->config_desc;
  if(cfg->bNumInterfaces>0){
    if(cfg->Itf_Desc[0].bInterfaceClass == USB_HUB_CLASS){
      return init_hub(device);
    }else if(cfg->Itf_Desc[0].bInterfaceClass == USB_HID_CLASS){
      return init_hid(device);
    }else if(cfg->Itf_Desc[0].bInterfaceClass == USB_USER_CALSS){
      return init_user(device);
    }else{
      // un-supported class
      goto itf_fail;
    }
  }
  return 0;
itf_fail:
  return -1;
}

// Enum device on default address, and return 0 when success
int enum_device(tusb_host_t* host, usb_device_t* device)
{
  static control_pipe_t def_ctrl;
  static uint8_t addr = 1;
  device->host = host;
  if( tusb_pipe_open(host, &def_ctrl.ctrl_out, 0, 0x00, EP_TYPE_CTRL, 8, device->speed) == 0
    && tusb_pipe_open(host, &def_ctrl.ctrl_in, 0, 0x80, EP_TYPE_CTRL, 8, device->speed) == 0){
  }else{
    goto dev_fail;
  }
  
  if(GetDeviceDesc(&def_ctrl, device->device_desc, 8) != 0){
    goto dev_fail;
  }
  
  device->addr = addr;
  addr++;
  if(addr>100) addr = 1;

  // set device address
  if(SetAddress(&def_ctrl, device->addr) != 0){
    goto dev_fail;
  }
  
  tusb_delay_ms(100);
  
  // setup control pipe for device
  if( tusb_pipe_open(host, &device->ctrl_pipe.ctrl_in, device->addr, 0x80, EP_TYPE_CTRL, device->device_desc[7], device->speed) == 0
   && tusb_pipe_open(host, &device->ctrl_pipe.ctrl_out, device->addr, 0x00, EP_TYPE_CTRL, device->device_desc[7], device->speed) == 0){
  }else{
    // fail to open control pipe
    goto dev_fail;
  }
  
  // Get config descriptor header
  if(GetConfigDesc(&device->ctrl_pipe, device->config_desc, 9) != 0){
    goto dev_fail;
  }
  
  device->config_len = *(uint16_t*)(device->config_desc+2);
  if(device->config_len > sizeof(device->config_desc)){
    // config too long
    goto dev_fail;
  }

  // get total config descriptor
  if( GetConfigDesc(&device->ctrl_pipe, device->config_desc, device->config_len) != 0){
    goto dev_fail;
  }
 
  // Set config to 0
  if(SetConfig(&device->ctrl_pipe) != 0){
    goto dev_fail;
  }
  
  if(init_interface(device) != 0){
    goto dev_fail;
  }
  
  // not the new device is addressed and config, parse the interface class and do something
  return 0;
  
dev_fail:
  tusb_pipe_close(&def_ctrl.ctrl_out);
  tusb_pipe_close(&def_ctrl.ctrl_in);
  
  tusb_pipe_close(&device->ctrl_pipe.ctrl_in);
  tusb_pipe_close(&device->ctrl_pipe.ctrl_out);
  if(device->itf.deinit){
    device->itf.deinit(device);
  }
  
  return -1;
}

__ALIGN_BEGIN  uint8_t last_led  __ALIGN_END = 0;
usb_device_t* root = 0;
void set_kbd_led(usb_device_t* cur_dev)
{
  if(cur_dev){
    USBH_CfgDescTypeDef* cfg = (USBH_CfgDescTypeDef*)cur_dev->config_desc;
    if( cfg->Itf_Desc[0].bInterfaceClass == USB_HID_CLASS 
     && cfg->Itf_Desc[0].bInterfaceProtocol == HID_KEYBRD_BOOT_CODE ){
        //SetBootProtocol(&cur_dev->ctrl_pipe);
        SetReport(&cur_dev->ctrl_pipe, &last_led, 1);
      }
    for(int i=0;i<MAX_CHILD;i++){
      set_kbd_led(cur_dev->children[i]);
    }
  }
}

#define MAX_DEV       8
__ALIGN_BEGIN usb_device_t dev_pool[MAX_DEV] __ALIGN_END;
usb_device_t* new_device(void)
{
  int i;
  for(i=0;i<MAX_DEV;i++){
    if(dev_pool[i].addr == 0){
      return &dev_pool[i];
    }
  }
  return 0;
}

void free_device(usb_device_t* dev)
{
  if(dev){
    if(dev->itf.deinit){
      dev->itf.deinit(dev);
    }
    dev->addr = 0;
  }
}

// Main loop for the host
void host_loop(tusb_host_t* host)
{
  if( host->state == TUSB_HOST_PORT_CONNECTED){
    // reset port
    tusb_port_set_reset(host, 0, 1);
    tusb_delay_ms(50);
    tusb_port_set_reset(host, 0, 0);
    tusb_delay_ms(100);
  }else if( host->state ==  TUSB_HOST_PORT_ENABLED ){
    if(!root){
      root = new_device();
      root->is_root = 1;
      root->speed = tusb_port_get_speed(host, 0);
      if(enum_device(host, root) != 0){
        free_device(root);
        root = 0;
      }
    }
  }else if( host->state == TUSB_HOST_PORT_DISCONNECTED ){
    if(root){
      free_device(root);
      root = 0;
    }
  }
  if(root){
    root->itf.handle(root);
  }
  if(kbd_led != last_led){
    last_led = kbd_led;
    set_kbd_led(root);
  }
}
