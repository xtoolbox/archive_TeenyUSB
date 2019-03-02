/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-12-12     Yi Qiu      first version
 */
#include <rtthread.h>
#include <drivers/usb_host.h>

#if defined(RT_USBH_HID_KEYBOARD) || defined(RT_USBH_HID_MOUSE)
#include <hid.h>
#endif

/**
 * This function will initialize the usb host stack, all the usb class driver and
 * host controller driver are also be initialized here.
 * 
 * @return none.
 */
rt_err_t rt_usb_host_init(const char* host_name)
{
    rt_device_t uhc;    

    uhc = rt_device_find(host_name);
  
    /* initialize all supported class driver */
    rt_usbh_class_driver_init();
  
    if(uhc == RT_NULL)
    {
        rt_kprintf("can't find usb host controller %s\n", host_name);
        return -RT_ERROR;
    }

    /* initialize usb hub */
    rt_usbh_hub_init((uhcd_t)uhc);

    /* initialize usb host controller */
    rt_device_init(uhc);

    return RT_EOK;
}


rt_err_t rt_usb_host_deinit(const char* host_name)
{
    rt_device_t uhc;    

    uhc = rt_device_find(host_name);
  
    if(uhc == RT_NULL)
    {
        rt_kprintf("can't find usb host controller %s\n", host_name);
        return -RT_ERROR;
    }

    /* initialize usb hub */
    rt_usbh_hub_deinit((uhcd_t)uhc);

    /* initialize usb host controller */
    rt_device_close(uhc);

    return RT_EOK;
}
