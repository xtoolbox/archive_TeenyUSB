/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-01-03     Yi Qiu      first version
 */

#include <rtthread.h>
#include <drivers/usb_host.h>
#include "hid.h"
#include "usb_key_code.h"

#define LED_NUM_LOCK   1
#define LED_CAPS_LOCK  2

#define MOD_CTRL      (0x01 | 0x10)
#define MOD_SHIFT     (0x02 | 0x20)
#define MOD_ALT       (0x04 | 0x40)
#define MOD_WIN       (0x08 | 0x80)


#if defined(RT_USBH_HID) && defined(RT_USBH_HID_KEYBOARD)

static struct uprotocal kbd_protocal; 
static rt_device_t sh_input;
static rt_uint8_t  kbd_led = 0;
const rt_uint8_t knum[] = "1234567890";
const rt_uint8_t ksign[] = "!@#$%^&*()";
const rt_uint8_t tabA[] = "\t -=[]\\#;'`,./";
const rt_uint8_t tabB[] = "\t _+{}|~:\"~<>?";


static int translate_key_to_console(rt_uint8_t modify, rt_uint8_t key, rt_uint8_t* data)
{
  if(key >= KEY_A && key <= KEY_Z){
    if(kbd_led & LED_CAPS_LOCK){
      if(modify & MOD_SHIFT){
        *data = 'a' + key - KEY_A;
      }else{
        *data = 'A' + key - KEY_A;
      }      
    }else{
      if(modify & MOD_SHIFT){
        *data = 'A' + key - KEY_A;
      }else{
        *data = 'a' + key - KEY_A;
      }
    }
    return 1;
  }else if(key >= KEY_1 && key <= KEY_0){
    if(modify & MOD_SHIFT){
      *data = ksign[key - KEY_1];
    }else{
      *data = knum[key - KEY_1];
    }
    return 1;
  }else if(key >= KEY_TAB && key <= KEY_SLASH){
    if(modify & MOD_SHIFT){
      *data = tabB[key - KEY_TAB];
    }else{
      *data = tabA[key - KEY_TAB];
    }
    return 1;
  }else if(key == KEY_RIGHT){
    rt_memcpy(data, "\x1b\x5b\x43", 3);
    return 3;
  }else if(key == KEY_LEFT){
    rt_memcpy(data, "\x1b\x5b\x44", 3);
    return 3;
  }else if(key == KEY_DOWN){
    rt_memcpy(data, "\x1b\x5b\x42", 3);
    return 3;
  }else if(key == KEY_UP){
    rt_memcpy(data, "\x1b\x5b\x41", 3);
    return 3;
  }else if(key == KEY_ENTER){
    *data = '\r';
    return 1;
  }else if(key == KEY_BACKSPACE){
    *data = 0x08;
    return 1;
  }
  return 0;
}

static rt_err_t rt_usbh_hid_kbd_callback(struct uhintf* arg)
{
    int int1, int2;
    struct uhid* hid; 
    rt_uint8_t command[8];
    int cmd_len;
    rt_uint8_t last_led = kbd_led;
    struct uhintf* intf = (struct uhintf*)arg;
    
    hid = (struct uhid*)intf->instance;

    int1 = *(rt_uint32_t*)hid->buffer;
    int2 = *(rt_uint32_t*)(&hid->buffer[4]);
  
    if(hid->buffer[2] == KEY_NUMLOCK){
      kbd_led ^= LED_NUM_LOCK;
    }else if(hid->buffer[2] == KEY_CAPSLOCK){
      kbd_led ^= LED_CAPS_LOCK;
    }
    
    if(last_led != kbd_led){
      rt_usbh_hid_set_report(intf, &kbd_led, 1);
    }
    
    cmd_len = translate_key_to_console(hid->buffer[0], hid->buffer[2], command);
    if(sh_input != RT_NULL){
      if(cmd_len > 0){
        rt_device_write(sh_input, 0, command, cmd_len);
      }
    }else{
      if(int1 != 0 || int2 != 0)
      {
          RT_DEBUG_LOG(RT_DEBUG_USB, ("key down 0x%x, 0x%x\n", int1, int2));
          rt_kprintf("HID Key: 0x%x, 0x%x\n", int1, int2);
      }
    }
    
    return RT_EOK;
}

static rt_err_t rt_usbh_hid_kbd_init(struct uhintf* arg)
{
    struct uhintf* intf = (struct uhintf*)arg;

    RT_ASSERT(intf != RT_NULL);        
    
    rt_usbh_hid_set_protocal(intf, 0);

    rt_usbh_hid_set_idle(intf, 10, 0);
  
    sh_input = rt_device_find("sh_in");

    //RT_DEBUG_LOG(RT_DEBUG_USB, ("start usb keyboard\n"));    

    return RT_EOK;
}

/**
 * This function will define the hid keyboard protocol, it will be register to the protocol list.
 * 
 * @return the keyboard protocol structure.
 */
uprotocal_t rt_usbh_hid_protocal_kbd(void)
{
    kbd_protocal.pro_id = USB_HID_KEYBOARD;

    kbd_protocal.init = rt_usbh_hid_kbd_init;
    kbd_protocal.callback = rt_usbh_hid_kbd_callback;

    return &kbd_protocal;
}

#endif

