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

void delay_ms(uint32_t ms)
{
  uint32_t i,j;
  for(i=0;i<ms;++i)
    for(j=0;j<500;++j);
}

void host_loop(tusb_host_t* host);

int main(void)
{
#if defined(STM32F723xx)
  tusb_host_t* host = tusb_get_host(1);
  // Enable HS port power
  __HAL_RCC_GPIOH_CLK_ENABLE();
  GPIOH->MODER &= ~(GPIO_MODER_MODER0 << (12*2));
  GPIOH->MODER |= (GPIO_MODER_MODER0_0 << (12*2));
  GPIOH->BSRR = GPIO_PIN_12;
#elif defined(STM32F767xx)
  __HAL_RCC_GPIOG_CLK_ENABLE();
  GPIOG->MODER &= ~(GPIO_MODER_MODER0 << (6*2));
  GPIOG->MODER |= (GPIO_MODER_MODER0_0 << (6*2));
  GPIOG->BSRR = GPIO_PIN_6;
  tusb_host_t* host = tusb_get_host(0);
#else
  tusb_host_t* host = tusb_get_host(0);
#endif
  tusb_close_host(host);
  delay_ms(100);
  tusb_open_host(host);
  delay_ms(200);
  while(1){
    host_loop(host);
  }
}


