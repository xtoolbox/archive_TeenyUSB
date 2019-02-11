-- Driver.lua
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

-- driver generation source code, use the inf template from the libwdi project

require("usb_common_desc")
require("usb_wcid")

local teenyGUID = "{5F5080D8-75A4-46F5-ADF8-ED654DD1AF45}"

local tempPath = "driver_res\\template\\"
local tempBin = "driver_res\\bins\\"
local outputPath = "output\\"

local logW = _G.logW or function(...) print(...) end
local logI = _G.logI or function(...) print(...) end
local logE = _G.logE or function(...) print(...) end

function SupportDriverList()
    return {
        "Select Driver",
        "usbser",
        "WinUSB",
        "libusb0",
        "libusbk",
    }
end

local extData = {
    WinUSB = {
        DRIVER_VERSION = "6.1.7600.16385",
        KMDF_VERSION = "1.11",
        WDF_VERSION = "01011",
        USE_DEVICE_INTERFACE_GUID = "AddDeviceInterfaceGUID",
    },
    libusb0 = {
        LK_COMMA = ",",
        LK_DLL = "libusbk.dll",
        LK_X86_DLL = "libusbk_x86.dll",
        DRIVER_VERSION = "1.2.6.0",
        DRIVER_DATE = "08/02/2012",
        LK_EQ_X86 = "= 1,x86",
        LK_EQ_X64 = "= 1,x64",
    },
    libusbk = {
        DRIVER_VERSION = "3.0.7.0",
        DRIVER_DATE = "12/01/2014",
        WDF_VERSION = "01011",
        KMDF_VERSION = "1.11",
        USE_DEVICE_INTERFACE_GUID = "AddDeviceInterfaceGUID",
    },
}

local function appendExtData(drv)
    local ext = extData[drv.name]
    if ext then
        for k,v in pairs(ext) do
            drv[k] = v
        end
    end
end

local function getIfNo(IForIAD)
    if IForIAD.bDescriptorType == IAD_DESCRIPTOR_TYPE then
        return IForIAD.bFirstInterface()
    else
        return IForIAD.bInterfaceNumber
    end
end

local function GetString(desc, field, default)
    return rawget(desc, field) or default or "TeenyUSB"
end

local function append_interface(drvs, drvName, ifID, dev)
    local drv = drvs[drvName]
    if not drv then
        drv = {}
        drv.name = drvName
        drvs[drvName] = drv
        drvs[#drvs+1] = drv
    end
    drv.DEVICE_DESCRIPTION = drv.DEVICE_DESCRIPTION or dev.product
    drv.DEVICE_MANUFACTURER = drv.DEVICE_MANUFACTURER or dev.vendor
    drv.DEVICE_HARDWARE_ID = drv.DEVICE_HARDWARE_ID or ifID
    if not drv[ifID] then
        drv[#drv+1] = ifID
        drv[ifID] = true
    end
end

local function addDevices(temp, devs)
    local p1,p1e = string.find(temp, "%%DeviceName%%")
    local p2,p2e = string.find(temp, "%%DeviceID%%")
    local r = temp
    if p1 and p2 then
        local prefix = temp:sub(p1, p2-1)
        r = temp:sub(1, p1-1)
        for i,v in ipairs(devs) do
            r = r .. prefix .. v .. "\n"
        end
        r = r .. addDevices(temp:sub(p2e+1), devs)
    end
    return r
end

local function sign_driver(catPath, devName, fileList)
    local arg = 'gencat "' .. catPath .. '" "' .. devName .. '" '
    for i,v in ipairs(fileList) do
        arg = arg .. '"'..v ..'" '
    end
    local f = io.popen(arg)
    f:close()
end

local function PrepareDriver(name, r, genFiles)
    local drivers = {}
    name = string.gsub(name, "_+$", "")
    if name == "" then
        name = "TeenyUSB"
    end
    
    for i,dev in ipairs(r.devs) do
        if #dev == 1 then
            -- device contain only one interface
            append_interface(drivers, dev[1].driver, dev.id, dev)
        else
            for j,ifs in ipairs(dev) do
                append_interface(drivers, ifs.driver, ifs.id, dev)
            end
        end
    end
    local r = ""
    local copyBin = false
    for i,v in ipairs(drivers) do
        local f = io.open( tempPath..v.name .. ".inf.in", "r")
        if f then
            local template = f:read("*a")
            f:close()
            if #v > 1 then
                template = addDevices(template, v)
            end
            v.INF_FILENAME = name .. "_" .. v.name .. ".inf"
            v.CAT_FILENAME = name .. "_" .. v.name .. ".cat"
            v.DRIVER_DATE = os.date("%m/%d/%Y")
            v.DEVICE_INTERFACE_GUID = GUID(v.INF_FILENAME .. v[1] .. "device")
            appendExtData(v)
            template = string.gsub(template, "#([%w_]+)#", v)
            
            r = r .. template .. "\n\n"
            local p1,p2 = string.find(template, "#([%w_]+)#")
            if p1 then
                error("token not processed " .. template:sub(p1, p2))
            end
            if genFiles then
                if not copyBin then
                    os.execute("xcopy "..tempBin.."* " .. outputPath .. " /E /Y")
                    copyBin = true
                end
                local inf = io.open(outputPath .. v.INF_FILENAME, "w+")
                if inf then
                    inf:write(template)
                    inf:close()
                    logI("Generate inf file: " .. outputPath .. v.INF_FILENAME)
                end
            end
            local fileList = {}
            fileList[#fileList+1] = outputPath .. v.INF_FILENAME
            local catf = io.open( tempPath..v.name .. ".cat.in", "r")
            if catf then
                for c in catf:lines() do
                    c = string.gsub(c, "#([%w_]+)#", v)
                    local tf = io.open( outputPath.."x86\\" .. c, "rb")
                    if tf then
                        tf:close()
                        fileList[#fileList+1] = outputPath.."x86\\" .. c
                    end
                    tf = io.open( outputPath.."amd64\\" .. c, "rb")
                    if tf then
                        tf:close()
                        fileList[#fileList+1] = outputPath.."amd64\\" .. c
                    end
                end
                catf:close()
            end
            r = r .. "####  need sign file list: ####\n"
            for j,sFile in ipairs(fileList) do
                r = r .. "#" .. sFile .. "\n"
            end
            r = r .. "############# inf file for "..v.name.." end #############\n\n"
            if genFiles then
                sign_driver(outputPath .. v.CAT_FILENAME, name, fileList)
                logI("Generate cat file: " .. outputPath .. v.CAT_FILENAME)
            end
        end
    end
    return r
end



local function DriverCollect(r, desc)
    local cfg = desc[1]
    r.devs = r.devs or {}
    assert(cfg.bDescriptorType == CONFIGURATION_DESCRIPTOR_TYPE, "Fail to get config desc")
    assert(desc.drivers, "Fail to get driver bind info")
    assert(#desc.drivers == #cfg, "Driver count and Interface count mismatch")
    local devId = string.format("VID_%04X&PID_%04X", desc.vid, desc.pid)
    if r.devs[devId] then
        logW("Device " .. devId .. " driver already set, new interface will append to it")
    else
       r.devs[devId] = {}
       r.devs[#r.devs+1] = r.devs[devId]
    end
    local dev = r.devs[devId]
    dev.id = devId
    dev.vendor = GetString(desc, "strManufacture", "TeenyUSB")
    dev.product = GetString(desc, "strProduct", "TeenyUSB Demo")
    for i, v in ipairs(cfg) do
        local ifID = string.format("VID_%04X&PID_%04X&MI_%02X", desc.vid, desc.pid, getIfNo(v))
        
        local ifInfo = {
            id = ifID,
            driver = desc.drivers[i]
        }
        
        dev[#dev+1] = ifInfo
        if dev[ifID] then
            logW("Interface " .. ifID .. " driver already set, last one take effect")
        end
        dev[ifID] = ifInfo
    end
end


function DriverCreate(name, descs, genFiles)
    logW = _G.logW or logW
    logI = _G.logI or logI
    logE = _G.logE or logE
    local r = {}
    if descs.bDescriptorType == DEVICE_DESCRIPTOR_TYPE then
        DriverCollect(r, descs)
    else
        for i,v in ipairs(descs) do
            DriverCollect(r, v)
        end
    end
    return PrepareDriver(name, r, genFiles)
    --return s
end