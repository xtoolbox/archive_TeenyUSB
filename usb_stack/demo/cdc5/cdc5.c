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
#include "string.h"

#define  MAX_EP  5

typedef struct _cdc
{
  uint8_t   buf[256];
  uint32_t  data_cnt;  
}cdc_t;

cdc_t cdc[MAX_EP];
// if data tx done, set rx valid again
void tusb_on_tx_done(tusb_device_t* dev, uint8_t EPn)
{
  if(EPn>0 && EPn<=MAX_EP){
    tusb_set_rx_valid(dev, EPn);
  }
}

int tusb_on_rx_done(tusb_device_t* dev, uint8_t EPn, const void* data, uint16_t len)
{
  if(EPn>0 && EPn<=MAX_EP){
    uint8_t idx = EPn-1;
    cdc[idx].data_cnt = len;
    return len;
  }
  return 0;
}

void tusb_reconfig(tusb_device_t* dev)
{
  uint32_t i;
  // call the BULK device init macro
  CDC5_TUSB_INIT(dev);
  for(i=1; i<= MAX_EP;i++){
    tusb_set_recv_buffer(dev, i, cdc[i-1].buf, sizeof(cdc[i-1].buf));
    cdc[i-1].data_cnt = 0;
    tusb_set_rx_valid(dev, i);
  }
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
    uint32_t idx;
    for(idx=0;idx<MAX_EP;idx++){
      cdc_t* pc = &cdc[idx];
      if(pc->data_cnt){
        for(int i=0;i<pc->data_cnt;i++){
          pc->buf[i] += (idx+1);
        }
        tusb_send_data(dev, idx+1, pc->buf, pc->data_cnt, TUSB_TXF_ZLP);
        pc->data_cnt = 0;
      }
    }
  }
}

