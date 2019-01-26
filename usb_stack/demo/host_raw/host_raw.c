/*
 * teeny usb - light weight usb stack for STM32 micro controllers
 * 
 * Copyright (c) 2018 XToolBox  - admin@xtoolbox.org
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

#include "teeny_usb_host.h"

#define  READ_EP  0x81
#define  WRITE_EP 0x02
#define  EP_MPS   64

uint8_t buf[4096];
__IO host_port_state_t state = TUSB_HOST_PORT_DUMMY;

void tusb_host_port_changed(tusb_host_t* host, uint8_t port, host_port_state_t new_state)
{
  state = new_state;
}

void tusb_delay_ms(uint32_t ms)
{
  uint32_t i,j;
  for(i=0;i<ms;++i)
    for(j=0;j<200;++j);
}

tusb_pipe_t pipe_read;
tusb_pipe_t pipe_write;
tusb_pipe_t pipe_ctrl_in;
tusb_pipe_t pipe_ctrl_out;

__ALIGN_BEGIN uint8_t test_data[63] __ALIGN_END = {
  1,2,3,4,5,6,7,8, 1,2,3,4,5,6,7,8, 1,2,3,4,5,6,7,8,
};

__ALIGN_BEGIN uint8_t device_descriptor[18] __ALIGN_END;
__ALIGN_BEGIN tusb_setup_packet setup __ALIGN_END = {
  .bmRequestType = 0x80,
  .bRequest = USB_REQ_GET_DESCRIPTOR,
  .wValue = USB_DESC_TYPE_DEVICE<<8,
  .wIndex = 0,
  .wLength = 18,
};

int main(void)
{
  uint8_t speed;
  tusb_host_t* host = tusb_get_host(TEST_APP_USB_CORE);
  HOST_PORT_POWER_ON();
  
  tusb_open_host(host);
  while(1){
    if(state == TUSB_HOST_PORT_CONNECTED){
      state = TUSB_HOST_PORT_DUMMY;
      // reset port0
      tusb_port_set_reset(host, 0, 1);
      tusb_delay_ms(100);
      // release port0
      tusb_port_set_reset(host, 0, 0);
      tusb_delay_ms(100);
      speed = tusb_port_get_speed(host, 0);
      
      // Open Control pipe
      tusb_pipe_open(host, &pipe_ctrl_in, 0, 0x80, EP_TYPE_CTRL, EP_MPS, speed);
      tusb_pipe_open(host, &pipe_ctrl_out, 0, 0x00, EP_TYPE_CTRL, EP_MPS, speed);
      
      // Get device descriptor
      tusb_pipe_setup(&pipe_ctrl_out, &setup);
      tusb_pipe_wait(&pipe_ctrl_out, 0xffffffff);
      
      // Prepare buffer to recv device descriptor
      tusb_pipe_xfer_data(&pipe_ctrl_in, device_descriptor, sizeof(device_descriptor));
      tusb_pipe_wait(&pipe_ctrl_in, 0xffffffff);
      
      // Send status packet
      tusb_pipe_xfer_data(&pipe_ctrl_out, 0, 0);
      tusb_pipe_wait(&pipe_ctrl_out, 0xffffffff);
      
      // Send test data
      tusb_pipe_open(host, &pipe_write, 0, WRITE_EP, EP_TYPE_BULK, EP_MPS, speed);
      tusb_pipe_xfer_data(&pipe_write, test_data, sizeof(test_data));
      tusb_pipe_wait(&pipe_write, 0xffffffff);

      // Recv test data
      tusb_pipe_open(host, &pipe_read, 0, READ_EP, EP_TYPE_BULK, EP_MPS, speed);
      tusb_pipe_xfer_data(&pipe_read, buf, sizeof(buf));
      tusb_pipe_wait(&pipe_read, 0xffffffff);
      
    }else if(state == TUSB_HOST_PORT_DISCONNECTED){
      state = TUSB_HOST_PORT_DUMMY;
      // Close all pipe when device disconnected
      tusb_pipe_close(&pipe_ctrl_in);
      tusb_pipe_close(&pipe_ctrl_out);
      tusb_pipe_close(&pipe_write);
      tusb_pipe_close(&pipe_read);
    }
  }
}


