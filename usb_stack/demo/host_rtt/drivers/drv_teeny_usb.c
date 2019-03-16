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

#include <dfs_romfs.h>
#include <dfs_fs.h>
#include "teeny_usb.h"
#include "board.h"
#include "drv_teeny_usb.h"


tusb_otg_t* otg_fs;
tusb_otg_t* otg_hs;

void init_fs_device_class(tusb_device_t* dev);
void init_hs_device_class(tusb_device_t* dev);
void init_host(uint32_t id, const char* name);
void deinit_host(const char* name);

static void change_otg_mode(tusb_otg_t* otg, uint8_t is_host)
{
  
  rt_kprintf("Change OTG %s to %s mode\n", 
  otg == otg_fs ? "FS": "HS",
  is_host ? "Host" : "Device"
  );
  if(is_host){
    tusb_close_device(&otg->device);
    if(otg == otg_fs){
      OTG_FS_PWR_ON();
      init_host(USB_CORE_ID_FS, "usbh_fs");
    }else{
      OTG_HS_PWR_ON();
      init_host(USB_CORE_ID_HS, "usbh_hs");
    }
  }else{
    if(otg == otg_fs){
      OTG_FS_PWR_OFF();
      deinit_host("usbh_fs");
      init_fs_device_class(&otg->device);
    }else{
      OTG_HS_PWR_OFF();
      deinit_host("usbh_hs");
      init_hs_device_class(&otg->device);
    }
    tusb_open_device(&otg->device);
    
  }
}

static void otg_in_line_init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /* Configure PA.10 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = OTG_FS_ID_PIN;
  HAL_GPIO_Init(OTG_FS_ID_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = OTG_HS_ID_PIN;
  HAL_GPIO_Init(OTG_HS_ID_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pull = GPIO_PULLDOWN;
  GPIO_InitStructure.Pin = OTG_FS_VBUS_PIN;
  HAL_GPIO_Init(OTG_FS_VBUS_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = OTG_HS_VBUS_PIN;
  HAL_GPIO_Init(OTG_HS_VBUS_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = OTG_FS_PWR_PIN;
  HAL_GPIO_Init(OTG_FS_PWR_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = OTG_HS_PWR_PIN;
  HAL_GPIO_Init(OTG_HS_PWR_PORT, &GPIO_InitStructure);
}

/* the system main thread */
void usb_otg_thread_entry(void *parameter)
{
#define USB_DEBOUNCE_TIEM  4
  static rt_uint8_t fs_host_cnt = 0;
  static rt_uint8_t fs_device_cnt = 0;
  static rt_uint8_t hs_host_cnt = 0;
  static rt_uint8_t hs_device_cnt = 0;
  while (1){
    if((OTG_FS_ID_PORT->IDR & OTG_FS_ID_PIN) == 0){
      fs_device_cnt = 0;
      if(fs_host_cnt<=USB_DEBOUNCE_TIEM){
        fs_host_cnt++;
        if(fs_host_cnt>USB_DEBOUNCE_TIEM){
          change_otg_mode(otg_fs, 1);
        }
      }
    }else{
      fs_host_cnt = 0;
      if(OTG_FS_VBUS_PORT->IDR & OTG_FS_VBUS_PIN){
        if(fs_device_cnt<=USB_DEBOUNCE_TIEM){
          fs_device_cnt++;
          if(fs_device_cnt>USB_DEBOUNCE_TIEM){
            change_otg_mode(otg_fs, 0);
          }
        }
      }else{
        fs_device_cnt = 0;
      }
    }
    if((OTG_HS_ID_PORT->IDR & OTG_HS_ID_PIN) == 0){
      hs_device_cnt = 0;
      if(hs_host_cnt<=USB_DEBOUNCE_TIEM){
        hs_host_cnt++;
        if(hs_host_cnt>USB_DEBOUNCE_TIEM){
          change_otg_mode(otg_hs, 1);
        }
      }
    }else{
      hs_host_cnt = 0;
      if(OTG_HS_VBUS_PORT->IDR & OTG_HS_VBUS_PIN){
        if(hs_device_cnt<=USB_DEBOUNCE_TIEM){
          hs_device_cnt++;
          if(hs_device_cnt>USB_DEBOUNCE_TIEM){
            change_otg_mode(otg_hs, 0);
          }
        }
      }else{
        hs_device_cnt = 0;
      }
    }
    rt_thread_mdelay(50);
  }
}

static int rt_hw_usb_init(void)
{
  rt_thread_t tid;
  otg_in_line_init();
  OTG_FS_PWR_OFF();
  OTG_HS_PWR_OFF();
  otg_fs = tusb_get_otg(USB_CORE_ID_FS);
  if(otg_fs){
    rt_kprintf("Get OTG FS success\n");
  }else{
    rt_kprintf("Get OTG FS fail\n");
  }
  //change_otg_mode(otg_fs, (OTG_FS_ID_PORT->IDR & OTG_FS_ID_PIN) == 0);
  
  otg_hs = tusb_get_otg(USB_CORE_ID_HS);
  if(otg_hs){
    rt_kprintf("Get OTG HS success\n");
  }else{
    rt_kprintf("Get OTG HS fail\n");
  }
  //change_otg_mode(otg_hs, (OTG_HS_ID_PORT->IDR & OTG_HS_ID_PIN) == 0);
  
  tid = rt_thread_create("usbotg", usb_otg_thread_entry, RT_NULL, 1024, 2, 5);
  RT_ASSERT(tid != RT_NULL);
  
  rt_thread_startup(tid);
  return 0;
}


INIT_DEVICE_EXPORT(rt_hw_usb_init);

