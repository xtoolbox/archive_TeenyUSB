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

#include <rtthread.h>
#include <dfs_romfs.h>
#include <dfs_fs.h>
#include "teeny_usb.h"
#include "usbd_msc.h"


static struct rt_messagequeue *usbd_mq;
typedef void(* device_handler_t)(tusb_device_t*);

typedef struct udevice_msg{
  device_handler_t handler;
  tusb_device_t* device;
}udevice_msg_t;

rt_err_t rt_usbd_post(tusb_device_t* dev, device_handler_t h)
{
    udevice_msg_t msg = {
      .handler = h,
      .device = dev
    };
    rt_mq_send(usbd_mq, (void*)&msg, sizeof(udevice_msg_t));
    return RT_EOK;
}

static void usb_device_thread_entry(void *parameter)
{
  while(1){
    udevice_msg_t msg;
    /* receive message */
    if(rt_mq_recv(usbd_mq, &msg, sizeof(udevice_msg_t), RT_WAITING_FOREVER) 
      != RT_EOK ) continue;
    if(msg.handler)
      msg.handler(msg.device);
  }
}

typedef struct
{
  void(*reconfig)(tusb_device_t* dev);
  int (*class_request)(tusb_device_t* dev, tusb_setup_packet* setup);
  void (*in_data)(tusb_device_t* dev, uint8_t EPn);
  int (*out_data)(tusb_device_t* dev, uint8_t EPn, const void* data, uint32_t len);
  void*  class_data;
}device_data_t;

void tusb_delay_ms(uint32_t ms)
{
  rt_thread_delay(ms * 1000 / RT_TICK_PER_SECOND);
}

void tusb_on_tx_done(tusb_device_t* dev, uint8_t EPn)
{
  device_data_t* dev_data = (device_data_t*)dev->user_data;
  if(dev_data && dev_data->in_data){
    dev_data->in_data(dev, EPn);
  }
}

int tusb_on_rx_done(tusb_device_t* dev, uint8_t EPn, const void* data, uint16_t len)
{
  device_data_t* dev_data = (device_data_t*)dev->user_data;
  if(dev_data && dev_data->out_data){
    return dev_data->out_data(dev, EPn, data, len);
  }
  return 0;
}


void tusb_reconfig(tusb_device_t* dev)
{
  device_data_t* dev_data = (device_data_t*)dev->user_data;
  if(dev_data && dev_data->reconfig){
    dev_data->reconfig(dev);
  }
}

int tusb_class_request(tusb_device_t* dev, tusb_setup_packet* setup)
{
  device_data_t* dev_data = (device_data_t*)dev->user_data;
  if(dev_data && dev_data->class_request){
    return dev_data->class_request(dev, setup);
  }
  return 0;
}


void init_hal_msc_class(tusb_device_t* dev);
int tusb_msc_class_request(tusb_device_t* dev, tusb_setup_packet* setup_req);
void msc_in_data(tusb_device_t* dev);
void msc_out_data(tusb_device_t* dev);
int tusb_cdc_class_request(tusb_device_t* dev, tusb_setup_packet* setup_req);

void config_as_msc(tusb_device_t* dev)
{
  //device_data_t* dev_data = (device_data_t*)dev->user_data;
  MSC_TUSB_INIT(dev);
  init_hal_msc_class(dev);
}


void msc_class_in_data(tusb_device_t* dev, uint8_t EPn)
{
  if(EPn == (MSC_EPIN_ADDR & 0x7f) ){
    rt_usbd_post(dev, msc_in_data);
  }
}

int msc_class_out_data(tusb_device_t* dev, uint8_t EPn, const void* data, uint32_t len)
{
  if(EPn == (MSC_EPOUT_ADDR & 0x7f) ){
    rt_usbd_post(dev, msc_out_data);
    return 1;
  }
  return 0;
}

static device_data_t  msc_data = {
  config_as_msc,
  tusb_msc_class_request,
  msc_class_in_data,
  msc_class_out_data,
};


#define  TX_EP   PCD_ENDP1
#define  RX_EP   PCD_ENDP2

static __IO uint32_t data_cnt = 0;
static uint8_t cdc_buffer[1024];

void config_as_cdc(tusb_device_t* dev)
{
  CDC_TUSB_INIT(dev);
  tusb_set_recv_buffer(dev, RX_EP, cdc_buffer, sizeof(cdc_buffer));
  tusb_set_rx_valid(dev, RX_EP);
}

void cdc_set_rx_valid(tusb_device_t* dev)
{
  tusb_set_rx_valid(dev, RX_EP);
}

void cdc_in_data(tusb_device_t* dev, uint8_t EPn)
{
  if(EPn == TX_EP){
    rt_usbd_post(dev, cdc_set_rx_valid);
  }
}

void cdc_send_data(tusb_device_t* dev)
{
  if(data_cnt){
    for(int i=0;i<data_cnt;i++){
      cdc_buffer[i]++;
    }
    tusb_send_data(dev, TX_EP, cdc_buffer, data_cnt, TUSB_TXF_ZLP);
    data_cnt = 0; 
  }
}

int cdc_out_data(tusb_device_t* dev, uint8_t EPn, const void* data, uint32_t len)
{
  if(EPn == RX_EP){
    data_cnt = len;
    rt_usbd_post(dev, cdc_send_data);
    return len;
  }
  return 0;
}

static device_data_t  cdc_data = {
  config_as_cdc,
  tusb_cdc_class_request,
  cdc_in_data,
  cdc_out_data,
};

void device_msc_init(tusb_device_t* dev)
{
  dev->user_data = &msc_data;
}

void device_cdc_init(tusb_device_t* dev)
{
  dev->user_data = &cdc_data;
}

#define UDEV_CDC   0
#define UDEV_MSC   1
static uint8_t fs_type = UDEV_CDC;
static uint8_t hs_type = UDEV_MSC;
static tusb_device_t* dev_fs = 0;
static tusb_device_t* dev_hs = 0;


void init_fs_device_class(tusb_device_t* dev)
{
  dev_fs = dev;
  if(fs_type == UDEV_CDC){
    device_cdc_init(dev);
  }else if(fs_type == UDEV_MSC){
    device_msc_init(dev);
  }
}

void init_hs_device_class(tusb_device_t* dev)
{
  dev_hs = dev;
  if(hs_type == UDEV_CDC){
    device_cdc_init(dev);
  }else if(hs_type == UDEV_MSC){
    device_msc_init(dev);
  }
}

int cmd_udevfs(int argc, char **argv)
{
  if(argc<2){
    rt_kprintf("Usage: udevfs cdc/msc\n");
    return -1;
  }
  if(strcmp(argv[1], "cdc") == 0){
    fs_type = UDEV_CDC;
  }else if(strcmp(argv[1], "msc") == 0){
    fs_type = UDEV_MSC;
  }else{
    rt_kprintf("Usage: udevfs cdc/msc\n");
  }
  init_fs_device_class(dev_fs);
  return 0;
}

FINSH_FUNCTION_EXPORT_ALIAS(cmd_udevfs, __cmd_udevfs, Set USB FS device class.);

int cmd_udevhs(int argc, char **argv)
{
  if(argc<2){
    rt_kprintf("Usage: udevhs cdc/msc\n");
    return -1;
  }
  if(strcmp(argv[1], "cdc") == 0){
    hs_type = UDEV_CDC;
  }else if(strcmp(argv[1], "msc") == 0){
    hs_type = UDEV_MSC;
  }else{
    rt_kprintf("Usage: udevhs cdc/msc\n");
  }
  init_hs_device_class(dev_hs);
  return 0;
}

FINSH_FUNCTION_EXPORT_ALIAS(cmd_udevhs, __cmd_udevhs, Set USB HS device class.);

void list_udev(void)
{
  rt_kprintf("\n");
  if(fs_type == UDEV_CDC){
    rt_kprintf("USB FS device type is CDC\n");
  }else if(fs_type == UDEV_MSC){
    rt_kprintf("USB FS device type is MSC\n");
  }else{
    rt_kprintf("USB FS device type is <Unknown>\n");
  }
  if(hs_type == UDEV_CDC){
    rt_kprintf("USB HS device type is CDC\n");
  }else if(hs_type == UDEV_MSC){
    rt_kprintf("USB HS device type is MSC\n");
  }else{
    rt_kprintf("USB HS device type is <Unknown>\n");
  }
}

static int rt_usb_class_init(void)
{
  rt_thread_t tid;
  usbd_mq = rt_mq_create("usbd", sizeof(udevice_msg_t), 16, RT_IPC_FLAG_FIFO);
  
  tid = rt_thread_create("usbd", usb_device_thread_entry, RT_NULL, 1024, 4, 5);
  RT_ASSERT(tid != RT_NULL);
  rt_thread_startup(tid);
  list_udev();
  return 0;
}

INIT_APP_EXPORT(rt_usb_class_init);

MSH_CMD_EXPORT(list_udev, list usb device type);

