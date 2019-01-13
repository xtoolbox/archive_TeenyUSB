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
#include "usbd_msc.h"

USBD_HandleTypeDef  hDev;

extern USBD_StorageTypeDef USBD_Storage_Interface_fops_FS;

void msc_in_data(void)
{
  MSC_BOT_DataIn(&hDev, MSC_EPIN_ADDR);
}

void msc_out_data(void)
{
  MSC_BOT_DataOut(&hDev, MSC_EPOUT_ADDR);
}


USBD_MSC_BOT_HandleTypeDef  MSCData;

void init_hal_msc_class(tusb_device_t* dev)
{
  hDev.pUserData = &USBD_Storage_Interface_fops_FS;
  hDev.pClassData = &MSCData;
  hDev.pData = dev;
  
  {
    USBD_HandleTypeDef *pdev = &hDev;
    USBD_MSC_BOT_HandleTypeDef  *hmsc = (USBD_MSC_BOT_HandleTypeDef*)pdev->pClassData;
      
    hmsc->bot_state  = USBD_BOT_IDLE;
    hmsc->bot_status = USBD_BOT_STATUS_NORMAL;
    
    hmsc->scsi_sense_tail = 0;
    hmsc->scsi_sense_head = 0;
    ((USBD_StorageTypeDef *)pdev->pUserData)->Init(0);
    USBD_LL_FlushEP(pdev, MSC_EPOUT_ADDR);
    USBD_LL_FlushEP(pdev, MSC_EPIN_ADDR);
    
    /* Prapare EP to Receive First BOT Cmd */
    USBD_LL_PrepareReceive (pdev,
                            MSC_EPOUT_ADDR,
                            (uint8_t *)&hmsc->cbw,
                            USBD_BOT_CBW_LENGTH);    
  }
}

#if defined(USB_OTG_FS) || defined(USB_OTG_HS)
void flush_rx(USB_OTG_GlobalTypeDef *USBx);
void flush_tx(USB_OTG_GlobalTypeDef *USBx, uint32_t num);
#endif

USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
#if defined(USB_OTG_FS) || defined(USB_OTG_HS)
  tusb_device_t* dev = (tusb_device_t*)pdev->pData;
  if(ep_addr&0x80){
    flush_tx(GetUSB(dev), ep_addr & 0x7f);
  }else{
    flush_rx(GetUSB(dev));
  }
#endif
  return USBD_OK;
}

uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
  return USBD_BOT_CBW_LENGTH;
}

USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev, uint8_t ep_addr, uint8_t *pbuf, uint16_t size)
{
  tusb_device_t* dev = (tusb_device_t*)pdev->pData;
  tusb_set_recv_buffer(dev, ep_addr&0x7f, pbuf, size);
  tusb_set_rx_valid(dev, ep_addr&0x7f);
  return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef  *pdev, uint8_t ep)
{
  tusb_device_t* dev = (tusb_device_t*)pdev->pData;
#if defined(USB)
  (void)dev;
  if( (ep & 0x7f) == 0){
    PCD_SET_EP_TXRX_STATUS(GetUSB(dev), 0, USB_EP_RX_STALL, USB_EP_TX_STALL);
  }else{
    if(ep & 0x80){
      PCD_SET_EP_TX_STATUS(GetUSB(dev), ep &0x7f , USB_EP_TX_STALL);
    }else{
      PCD_SET_EP_RX_STATUS(GetUSB(dev), ep &0x7f , USB_EP_RX_STALL);
    }
  }
#endif
#if defined(USB_OTG_FS) || defined(USB_OTG_HS)
  PCD_TypeDef* USBx =  GetUSB(dev);
  if( (ep & 0x7f) == 0){
    USBx_INEP(0)->DIEPCTL |= USB_OTG_DIEPCTL_STALL;
    USBx_OUTEP(0)->DOEPCTL |= USB_OTG_DOEPCTL_STALL;
  }else{
    if(ep & 0x80){
      USBx_INEP(0)->DIEPCTL |= USB_OTG_DIEPCTL_STALL;
    }else{
      USBx_OUTEP(0)->DOEPCTL |= USB_OTG_DOEPCTL_STALL;
    }
  }
#endif
  return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef *pdev, uint8_t ep_addr, uint8_t *pbuf, uint16_t size)
{
  tusb_device_t* dev = (tusb_device_t*)pdev->pData;
  tusb_send_data(dev, ep_addr&0x7f, pbuf, size);
  return USBD_OK;
}

void tusb_class_request(tusb_device_t* dev, tusb_setup_packet* setup_req)
{
  USBD_HandleTypeDef* pdev = &hDev;
  switch (setup_req->bRequest){
    case BOT_GET_MAX_LUN :
      if((setup_req->wValue  == 0) && 
        (setup_req->wLength == 1) &&
      ((setup_req->bmRequestType & 0x80) == 0x80)){
        MSCData.max_lun = ((USBD_StorageTypeDef *)pdev->pUserData)->GetMaxLun();
        tusb_send_data (dev, 0, &MSCData.max_lun,1);
      }
      break;
    case BOT_RESET :
      if((setup_req->wValue  == 0) && 
        (setup_req->wLength == 0) &&
      ((setup_req->bmRequestType & 0x80) != 0x80)){
         MSC_BOT_Reset(pdev);
      }
      break;
    }
}
