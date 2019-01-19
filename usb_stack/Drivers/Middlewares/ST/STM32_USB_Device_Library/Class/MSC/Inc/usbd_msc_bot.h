/**
  ******************************************************************************
  * @file    usbd_msc_bot.h
  * @author  MCD Application Team
  * @brief   Header for the usbd_msc_bot.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_MSC_BOT_H
#define __USBD_MSC_BOT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup MSC_BOT
  * @brief This file is the Header file for usbd_msc_bot.c
  * @{
  */


/** @defgroup USBD_CORE_Exported_Defines
  * @{
  */
#define USBD_BOT_IDLE                      0U       /* Idle state */
#define USBD_BOT_DATA_OUT                  1U       /* Data Out state */
#define USBD_BOT_DATA_IN                   2U       /* Data In state */
#define USBD_BOT_LAST_DATA_IN              3U       /* Last Data In Last */
#define USBD_BOT_SEND_DATA                 4U       /* Send Immediate data */
#define USBD_BOT_NO_DATA                   5U       /* No data Stage */

#define USBD_BOT_CBW_SIGNATURE             0x43425355U
#define USBD_BOT_CSW_SIGNATURE             0x53425355U
#define USBD_BOT_CBW_LENGTH                31U
#define USBD_BOT_CSW_LENGTH                13U
#define USBD_BOT_MAX_DATA                  256U

/* CSW Status Definitions */
#define USBD_CSW_CMD_PASSED                0x00U
#define USBD_CSW_CMD_FAILED                0x01U
#define USBD_CSW_PHASE_ERROR               0x02U

/* BOT Status */
#define USBD_BOT_STATUS_NORMAL             0U
#define USBD_BOT_STATUS_RECOVERY           1U
#define USBD_BOT_STATUS_ERROR              2U


#define USBD_DIR_IN                        0U
#define USBD_DIR_OUT                       1U
#define USBD_BOTH_DIR                      2U

/**
  * @}
  */

/** @defgroup MSC_CORE_Private_TypesDefinitions
  * @{
  */

typedef struct
{
  uint32_t dSignature;
  uint32_t dTag;
  uint32_t dDataLength;
  uint8_t  bmFlags;
  uint8_t  bLUN;
  uint8_t  bCBLength;
  uint8_t  CB[16];
  uint8_t  ReservedForAlign;
}
USBD_MSC_BOT_CBWTypeDef;


typedef struct
{
  uint32_t dSignature;
  uint32_t dTag;
  uint32_t dDataResidue;
  uint8_t  bStatus;
  uint8_t  ReservedForAlign[3];
}
USBD_MSC_BOT_CSWTypeDef;

/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_Types
  * @{
  */

/**
  * @}
  */
/** @defgroup USBD_CORE_Exported_FunctionsPrototypes
  * @{
  */
void MSC_BOT_Init (USBD_HandleTypeDef  *pdev);
void MSC_BOT_Reset (USBD_HandleTypeDef  *pdev);
void MSC_BOT_DeInit (USBD_HandleTypeDef  *pdev);
void MSC_BOT_DataIn (USBD_HandleTypeDef  *pdev,
                     uint8_t epnum);

void MSC_BOT_DataOut (USBD_HandleTypeDef  *pdev,
                      uint8_t epnum);

void MSC_BOT_SendCSW (USBD_HandleTypeDef  *pdev,
                             uint8_t CSW_Status);

void  MSC_BOT_CplClrFeature (USBD_HandleTypeDef  *pdev,
                             uint8_t epnum);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBD_MSC_BOT_H */
/**
  * @}
  */

/**
* @}
*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

