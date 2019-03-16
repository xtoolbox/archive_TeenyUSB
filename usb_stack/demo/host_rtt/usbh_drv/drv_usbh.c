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
#include <rtdevice.h>
#include "board.h"
#include "teeny_usb.h"

void tusb_host_port_changed(tusb_host_t* host, uint8_t port, host_port_state_t new_state)
{
  uhcd_t hcd = (uhcd_t)host->user_data;
  if(new_state == TUSB_HOST_PORT_CONNECTED){
    rt_uint8_t speed = tusb_port_get_speed(host, port);
    rt_usbh_root_hub_connect_handler(hcd, port+1, speed);
  }else if(new_state == TUSB_HOST_PORT_ENABLED){
    rt_usbh_root_hub_enable_handler(hcd, port+1);
  }else if(new_state == TUSB_HOST_PORT_DISCONNECTED){
    rt_usbh_root_hub_disconnect_handler(hcd, port+1);
  }
}

static rt_err_t drv_reset_port(uhcd_t hcd, rt_uint8_t port)
{
  tusb_host_t* host = (tusb_host_t*)hcd->parent.user_data;
  tusb_port_set_reset(host,port-1, 1);
  tusb_delay_ms(20);
  tusb_port_set_reset(host,port-1, 0);
  return RT_EOK;
}


static struct rt_completion urb_completion;
int tusb_on_channel_event(tusb_host_t* host, uint8_t hc_num)
{
  tusb_hc_data_t* hc = &host->hc[hc_num];
  //upipe_t pipe = (upipe_t)hc->user_data;
  if(hc->xfer_done){
    rt_completion_done(&urb_completion);
  }
  return 0;
}

static int drv_pipe_xfer(upipe_t pipe, rt_uint8_t token, void *buffer, int nbytes, int timeout)
{
  tusb_pipe_t* tpipe = (tusb_pipe_t*)pipe->backend;
  tusb_hc_data_t* hc = &tpipe->host->hc[tpipe->hc_num];
  rt_completion_init(&urb_completion);
  if(token == USBH_PID_SETUP){
    tusb_pipe_setup(tpipe, buffer);
  }else{
    tusb_pipe_xfer_data(tpipe, buffer, nbytes);
  }
  rt_err_t err = rt_completion_wait(&urb_completion, timeout);
  if(hc->state == TUSB_CS_TRANSFER_COMPLETE){
    return nbytes;
  }else if(hc->state == TUSB_CS_STALL){
    pipe->status = UPIPE_STATUS_STALL;
    return -RT_EIO;
  }
  return err;
}


static rt_err_t drv_open_pipe(upipe_t pipe)
{
  tusb_host_t* host = (tusb_host_t*)pipe->intf->device->hcd->parent.user_data;
  tusb_pipe_t* tpipe = (tusb_pipe_t*)rt_malloc(sizeof(tusb_pipe_t));
  uinst_t dev = pipe->intf->device;
  int r = tusb_pipe_open(
    host, 
    tpipe, 
    dev->address, 
    pipe->ep->bEndpointAddress, 
    pipe->ep->bmAttributes & EP_TYPE_MSK, 
    pipe->ep->wMaxPacketSize, 
    dev->speed);
  if(r == RT_EOK){
    pipe->backend = tpipe;
    tpipe->host->hc[tpipe->hc_num].user_data = pipe;
    return r;
  }
  rt_kprintf("Fail to allocate USBH pipe\n");
  rt_free(tpipe);
  return RT_ERROR;
}

static rt_err_t drv_close_pipe(upipe_t pipe)
{
  tusb_pipe_t* tpipe = (tusb_pipe_t*)pipe->backend;
  if(tpipe != RT_NULL){
    tusb_pipe_close(tpipe);
    rt_free(tpipe);
  }
  pipe->backend = RT_NULL;
  return RT_EOK;
}

static struct uhcd_ops _uhcd_ops =
{
    drv_reset_port,
    drv_pipe_xfer,
    drv_open_pipe,
    drv_close_pipe,
};

static rt_err_t _init(rt_device_t device)
{
  uhcd_t uhcd = (uhcd_t)device;
  tusb_host_t* host = (tusb_host_t*)uhcd->parent.user_data;
  tusb_open_host(host);
  return RT_EOK;
}

static rt_err_t _open(rt_device_t device, rt_uint16_t mode)
{
  return RT_EOK;
}

static rt_err_t _close(rt_device_t device)
{
  uhcd_t uhcd = (uhcd_t)device;
  tusb_host_t* host = (tusb_host_t*)uhcd->parent.user_data;
  tusb_close_host(host);
  return RT_EOK;
}

void init_host(uint32_t id, const char* name)
{
  
  rt_device_t uhc;
  uhc = rt_device_find(name);
  if(uhc != RT_NULL){
    return;
  }
  tusb_host_t* host = tusb_get_host(id);
  uhcd_t uhcd = (uhcd_t)rt_malloc(sizeof(struct uhcd));
  RT_ASSERT(uhcd != RT_NULL);
  rt_memset((void *)uhcd, 0, sizeof(struct uhcd));
  uhcd->parent.type = RT_Device_Class_USBHost;
  uhcd->parent.init = _init;
  uhcd->parent.open = _open;
  uhcd->parent.close = _close;
  uhcd->parent.user_data = host;
  uhcd->ops = &_uhcd_ops;
  uhcd->num_ports = 1;
  host->user_data = uhcd;
  rt_device_register((rt_device_t)uhcd, name, 0);
  rt_usb_host_init(name);
}


void deinit_host(const char* name)
{
  rt_device_t uhc;
  uhc = rt_device_find(name);
  if(uhc != RT_NULL){
    rt_usb_host_deinit(name);
    rt_device_unregister(uhc);
    rt_free(uhc);
  }
}

