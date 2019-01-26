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
#include "usbd_msc.h"

uint8_t msc_in = 0;
uint8_t msc_out = 0;

void msc_in_data(void);
void msc_out_data(void);

void tusb_on_tx_done(tusb_device_t* dev, uint8_t EPn)
{
  if(EPn == (MSC_EPIN_ADDR & 0x7f) ){
    msc_in = 1;
  }
}

int tusb_on_rx_done(tusb_device_t* dev, uint8_t EPn, const void* data, uint16_t len)
{
  if(EPn == (MSC_EPOUT_ADDR & 0x7f) ){
    msc_out = 1;
    return 1;
  }
  return 0;
}

void init_hal_msc_class(tusb_device_t* dev);

void tusb_reconfig(tusb_device_t* dev)
{
  // call the MSC device init macro
  MSC_TUSB_INIT(dev);
  // Init the HAL MSC class
  init_hal_msc_class(dev);
}

void tusb_delay_ms(uint32_t ms)
{
  uint32_t i,j;
  for(i=0;i<ms;++i)
    for(j=0;j<200;++j);
}

int main(void)
{
  tusb_device_t* dev = tusb_get_device(TEST_APP_USB_CORE);
  tusb_open_device(dev);
  while(1){
    if(msc_in){
      msc_in_data();
      msc_in = 0;
    }
    if(msc_out){
      msc_out_data();
      msc_out = 0;
    }
  }
}

