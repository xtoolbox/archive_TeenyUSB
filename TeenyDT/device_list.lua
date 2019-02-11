-- device_list.lua
--  _______                    _____ _______ 
-- |__   __|                  |  __ \__   __|
--    | | ___  ___ _ __  _   _| |  | | | |   
--    | |/ _ \/ _ \ '_ \| | | | |  | | | |   
--    | |  __/  __/ | | | |_| | |__| | | |   
--    |_|\___|\___|_| |_|\__, |_____/  |_|   
--                        __/ |              
--                       |___/               
--
-- TeenyDT - GUI and command line tool to generate USB descriptors and drivers
-- 
-- Copyright (c) 2019 XToolBox  - admin@xtoolbox.org
--                         www.tusb.org
-- 
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
-- 
-- The above copyright notice and this permission notice shall be included in all
-- copies or substantial portions of the Software.
-- 
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
-- SOFTWARE.

return {
    {name = "STM32F0xx",      maxEp = 7,   maxMem = 1024, },
    {name = "STM32F10x_FS",   maxEp = 7,   maxMem = 512,  },
    {name = "STM32F10x_OTG",  maxEp = 4,   maxMem = 1280, },
    {name = "STM32F2xx_FS",   maxEp = 4,   maxMem = 1280, },
    {name = "STM32F2xx_HS",   maxEp = 6,   maxMem = 4096, },
    {name = "STM32F4xx_FS",   maxEp = 4,   maxMem = 1280, },
    {name = "STM32F303_BC",   maxEp = 7,   maxMem = 512,  },
    {name = "STM32F373x",     maxEp = 7,   maxMem = 512,  },
    {name = "STM32F303_DE",   maxEp = 7,   maxMem = 1024, },
    {name = "STM32F4xx_HS",   maxEp = 6,   maxMem = 4096, },
    {name = "STM32F7xx_FS",   maxEp = 6,   maxMem = 1280, },
    {name = "STM32F7xx_HS",   maxEp = 9,   maxMem = 4096, },
    {name = "STM32H7xx_FS",   maxEp = 9,   maxMem = 1280, },
    {name = "STM32H7xx_HS",   maxEp = 9,   maxMem = 4096, },
}