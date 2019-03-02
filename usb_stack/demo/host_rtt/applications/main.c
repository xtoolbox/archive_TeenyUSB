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

int main(void)
{
    /* put user application code here */
  
    {
        static const rt_uint8_t _romfs_root_readme_txt[] = {
            0x52,0x54,0x2d,0x54,0x68,0x72,0x65,0x61,0x64,0x0d,0x0a,0x00
        };

        static const rt_uint8_t _romfs_root_sdcard_aaa_txt[] = {
            0x52,0x54,0x2d,0x54,0x68,0x72,0x65,0x61,0x64,0x0d,0x0a,0x00
        };

        static const struct romfs_dirent _romfs_root_sdcard[] = {
            {ROMFS_DIRENT_FILE, "aaa.txt", (rt_uint8_t *)_romfs_root_sdcard_aaa_txt, sizeof(_romfs_root_sdcard_aaa_txt)/sizeof(_romfs_root_sdcard_aaa_txt[0])}
        };

        static const rt_uint8_t _romfs_root_qspi_bbb_txt[] = {
            0x52,0x54,0x2d,0x54,0x68,0x72,0x65,0x61,0x64,0x0d,0x0a,0x00
        };

        static const struct romfs_dirent _romfs_root_qspi[] = {
            {ROMFS_DIRENT_FILE, "bbb.txt", (rt_uint8_t *)_romfs_root_qspi_bbb_txt, sizeof(_romfs_root_qspi_bbb_txt)/sizeof(_romfs_root_qspi_bbb_txt[0])}
        };

        static const struct romfs_dirent _romfs_root[] = {
            {ROMFS_DIRENT_FILE, "readme.txt", (rt_uint8_t *)_romfs_root_readme_txt, sizeof(_romfs_root_readme_txt)/sizeof(_romfs_root_readme_txt[0])},
            {ROMFS_DIRENT_DIR, "sdcard", (rt_uint8_t *)_romfs_root_sdcard, sizeof(_romfs_root_sdcard)/sizeof(_romfs_root_sdcard[0])},
            {ROMFS_DIRENT_DIR, "qspi", (rt_uint8_t *)_romfs_root_qspi, sizeof(_romfs_root_qspi)/sizeof(_romfs_root_qspi[0])},
            {ROMFS_DIRENT_DIR, "udisc0", 0, 0},
            {ROMFS_DIRENT_DIR, "udisc1", 0, 0},
            {ROMFS_DIRENT_DIR, "udisc2", 0, 0},
            {ROMFS_DIRENT_DIR, "udisc3", 0, 0},
            {ROMFS_DIRENT_DIR, "udisc4", 0, 0},
            {ROMFS_DIRENT_DIR, "udisc5", 0, 0},
            {ROMFS_DIRENT_DIR, "udisc6", 0, 0},
            {ROMFS_DIRENT_DIR, "udisc7", 0, 0},
            
        };

        static const struct romfs_dirent romfs_root = {
            ROMFS_DIRENT_DIR, "/", (rt_uint8_t *)_romfs_root, sizeof(_romfs_root)/sizeof(_romfs_root[0])
        };

        if (dfs_mount(RT_NULL, "/", "rom", 0, &(romfs_root)) == 0)
        {
            rt_kprintf("ROM file system initializated!\n");
        }
    }
  
    if (dfs_mount("qspi", "/qspi", "elm", 0, 0) == 0)
    {
        rt_kprintf("qspi mount to /qspi!\n");
    }
    else
    {
        rt_kprintf("qspi first mount fail, try to format it before mount!\n");
        if(dfs_mkfs("elm", "qspi") == 0){
          if(dfs_mount("qspi", "/qspi", "elm", 0, 0) == 0)
          {
            rt_kprintf("qspi mount to /qspi!\n");     
          }
          else
          {
            rt_kprintf("qspi mount to /qspi failed after mkfs!\n");
          }
        }
        else
        {
          rt_kprintf("mkfs elm on qspi failed!\n");
        }
    } 
    return 0;
}

