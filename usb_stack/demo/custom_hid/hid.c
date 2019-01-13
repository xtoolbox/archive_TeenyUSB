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

#include "teeny_usb.h"
#include "string.h"

#define  TX_EP   PCD_ENDP1
#define  RX_EP   PCD_ENDP1

uint8_t buf[64];
__IO uint32_t data_cnt = 0;

// if data tx done, set rx valid again
void tusb_on_tx_done(tusb_device_t* dev, uint8_t EPn)
{
  if(EPn == TX_EP){
    tusb_set_rx_valid(dev, RX_EP);
  }
}

int tusb_on_rx_done(tusb_device_t* dev, uint8_t EPn, const void* data, uint16_t len)
{
  if(EPn == RX_EP){
    data_cnt = len;
    return len;
  }
  return 0;
}

void tusb_reconfig(tusb_device_t* dev)
{
  // call the BULK device init macro
  HID_TUSB_INIT(dev);
  // setup recv buffer for rx end point
  tusb_set_recv_buffer(dev, RX_EP, buf, sizeof(buf));
  // enable rx ep after buffer set
  tusb_set_rx_valid(dev, RX_EP);
}

void delay_ms(uint32_t ms)
{
  uint32_t i,j;
  for(i=0;i<ms;++i)
    for(j=0;j<20;++j);
}

int main(void)
{
#if defined(STM32F723xx)
  tusb_device_t* dev = tusb_get_device(1);
#else
  tusb_device_t* dev = tusb_get_device(0);
#endif
  tusb_close_device(dev);
  delay_ms(100);
  tusb_open_device(dev);
  while(1){
    if(data_cnt){
      // every data plus 1 and echo back
      for(int i=0;i<data_cnt;i++){
        buf[i]++;
      }
      tusb_send_data(dev, TX_EP, buf, data_cnt);
      data_cnt = 0; 
    }
  }
}

const uint8_t* tusb_get_report_descriptor(tusb_device_t* dev, tusb_setup_packet* req, uint16_t* len)
{
  uint16_t ifn = req->wIndex;
  if(ifn<1){
    *len = HID_REPORT_DESCRIPTOR_SIZE_IF0;
    return HID_ReportDescriptor_if0;
  }
  *len = 0;
  return 0;
}


