-- usb_wcid.lua
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

--[[
     Windows compatible ID related descriptors
     visit https://github.com/pbatard/libwdi/wiki/WCID-Devices for more info
--]]
require("usb_common_desc")

local WIN8_1   = 0x06030000

function MSOSString(vendorCode)
    vendorCode = vendorCode or 1
    local desc = StringDescriptor("MSFT100")
    desc.index = "_MSOS"
    desc.layout[#desc.layout + 1] = 'bVendorCode'
    desc.bVendorCode = vendorCode
    desc.layout[#desc.layout + 1] = 'bReserved'
    desc.bReserved = 0
    desc.bLength = desc.bLength + 2
    return desc
end

local function WCIDFunctionToString(desc)
    local ident = desc.ident or "    "
    local r = "    /* WCID Function  */\n"
    r = r .. ident .. output8(desc.interface, "bFirstInterfaceNumber", #ident)
    r = r .. ident .. output8(1, "bReserved", #ident)
    local SIMPLE = _G.SIMPLE
    _G.SIMPLE = true
    r = r .. "    /* CID */\n    "
    for i=1,#desc.CID do
        r = r .. outputChar(desc.CID:byte(i), 'CIDs', #ident)
    end
    r = r .. "\n    /* sub CID */\n    "
    for i=1,#desc.subCID do
        r = r .. outputChar(desc.subCID:byte(i), 'subCIDs', #ident)
    end
    r = r .. "\n"
    _G.SIMPLE = SIMPLE
    r = r .. ident .. outputString("0,0,0,0,0,0", "Reserved", #ident)
    return r
end

function WCIDFunction(interface, CID, subCID)
    CID = CID or ""
    subCID = subCID or ""
    assert(#CID<9, "Compatible ID too long  >> " .. CID)
    assert(#subCID<9, "sub Compatible ID too long  >> " .. subCID)
    CID = CID .. string.rep("\x00", 8-#CID)
    subCID = subCID .. string.rep("\x00", 8-#subCID)
    local r = {
        interface = interface,
        CID = CID,
        subCID = subCID,
        
    }
    setmetatable(r, {__tostring = WCIDFunctionToString})
    return r
end

function WinUSB(interface)
    return WCIDFunction(interface, "WINUSB")
end


local REG_SZ                   = 1
local REG_EXPAND_SZ            = 2
local REG_BINARY               = 3
local REG_DWORD_LITTLE_ENDIAN  = 4
local REG_DWORD_BIG_ENDIAN     = 5
local REG_LINK                 = 6
local REG_MULTI_SZ             = 7

local function expand_ascii(s)
    local r = ""
    for i=1,#s do
        r = r .. s:sub(i,i) .. "\x00"
    end
    return r .. "\x00\x00"
end


function WCIDRegDesc(key, v)
    local desc = Descriptor{
        { wLength             = DUMMY               },
        { wDescriptorType     =  0x04               },
        { wPropertyDataType   = DUMMY               },
        { wPropertyNameLength = DUMMY               },
        
    }({})
    
    local name = key
    desc.vData = ""
    desc.wPropertyNameLength = #key * 2 + 2
    local vals = ""
    if type(v) == "string" then
        vals = v .."\x00"
        desc.vData = v
    else
        local sep = ""
        for i,s in ipairs(v) do
            vals = vals .. s .. "\x00"
            desc.vData = desc.vData .. sep .. s
            sep = "\n"
        end
    end
    
    desc.vName = name
    desc.name = expand_ascii(name)
    desc.data = expand_ascii(vals)
    desc.wPropertyNameLength = #desc.name
    
    desc.wLength = #desc.name + #desc.data + 10
    
    desc.wPropertyDataType = REG_MULTI_SZ
    
    desc.outputHeader = function(desc)
        local ident = desc.ident or "    "
        return ident .. "/*  WCID WinUSB registry value */\n" .. ident
     ..                 "/*  " .. desc.vName .. " = " .. desc.vData .. " */\n"
     end
    
    desc.outputTail = function(desc)
        local ident = desc.ident or "    "
        local r = ident
        local s = _G.SIMPLE
        _G.SIMPLE = true
        for i=1,#desc.name do
            r = r .. outputChar(desc.name:byte(i), "", #ident)
            if (i&0x7) == 0 then r = r .."\n" .. ident end
        end
        r = r .. "\n"
        _G.SIMPLE = s
        r = r .. ident .. output16(#desc.data, "wPropertyDataLength", #ident)
        _G.SIMPLE = true
        r = r .. ident
        for i=1,#desc.data do
            r = r .. outputChar(desc.data:byte(i), "", #ident)
            if (i&0x7) == 0 then r = r .."\n"  .. ident end
        end
        _G.SIMPLE = s
        r = r .. "\n"
        return r        
    end
    return desc
end

function WCIDWinUSB(CID, subCID)
    local desc = Descriptor{
        { wLength             = 20                  },
        { wDescriptorType     = 0x03                },
    }({})
    
    CID = CID or ""
    subCID = subCID or ""
    assert(#CID<9, "Compatible ID too long  >> " .. CID)
    assert(#subCID<9, "sub Compatible ID too long  >> " .. subCID)
    CID = CID .. string.rep("\x00", 8-#CID)
    subCID = subCID .. string.rep("\x00", 8-#subCID)
    
    desc.CID = CID
    desc.subCID = subCID
    
    desc.outputHeader = function(desc)
        local ident = desc.ident or "    "
        return ident .. "/* WCID WinUSB compatible ID */\n"
    end
    
    desc.outputTail = function(desc)
        local ident = desc.ident or "    "
        local SIMPLE = _G.SIMPLE
        _G.SIMPLE = true
        local r = ident .. "/* CID */\n" .. ident
        for i=1,#desc.CID do
            r = r .. outputChar(desc.CID:byte(i), 'CIDs', #ident)
        end
        r = r .. "\n" .. ident .."/* sub CID */\n" .. ident
        for i=1,#desc.subCID do
            r = r .. outputChar(desc.subCID:byte(i), 'subCIDs', #ident)
        end
        r = r .. "\n"
        _G.SIMPLE = SIMPLE
        return r
    end
    
    return desc
end


function WCIDCapWinUSB(param)
    local uuid = param.uuid or {
    	0xDF, 0x60, 0xDD, 0xD8,
        0x89, 0x45, 0xC7, 0x4C,
        0x9C, 0xD2, 0x65, 0x9D,
        0x9E, 0x64, 0x8A, 0x9F,
    }
    local desc = Descriptor{
        { dwWindowsVersion    = WIN8_1              },
        { wDescriptorSetTotalLength =  DUMMY        },
        { bVendorCode         = "WCID_VENDOR_CODE"  },
        { bAltEnumCode        = 0                   },
    }(param)
    
    desc.uuid = uuid
    desc.length = #uuid + 8
    
    desc.outputHeader = function(desc)
        local prefix = desc.prefix or ""
        desc.wDescriptorSetTotalLength = prefix .. "WCID_DESC_SET_SIZE"
        local ident = desc.ident or "    "
        local r = ident .. "/*  WCID WinUSB CAPABILITY descriptor */\n"
        local s = _G.SIMPLE
        _G.SIMPLE = true
        r = r .."    "
        for i=1,#desc.uuid do
            r = r .. output8(desc.uuid[i], "", #ident)
            if (i&0x7) == 0 then r = r .."     /* PlatformCapabilityUUID */ \n    " end
        end
        _G.SIMPLE = s
        r = r .. "\n"
        return r
    end
    
    desc.outputTail = ""
    
    return desc
end

function WCIDFunctionSet(param)
    local desc = Descriptor{
        { wLength                    = 8              },
        { wDescriptorType            = 2              },
        { bFirstInterface            = DUMMY          },
        { bReserved                  = 0              },
        { wSubsetLength              = DUMMY         },
    }(param)
    
    desc.wSubsetLength = 8
    for i,v in ipairs(param) do
        local l = nil
        l = v.wSubsetLength or v.wTotalLength or v.wLength
        if l then
            desc.wSubsetLength = desc.wSubsetLength + l
        end
    end
    desc.outputHeader = function(desc)
        local ident = desc.ident or "    "
        return ident .. "  /* Microsoft OS 2.0 function subset header */ \n"
    end
    
    desc.outputTail = function(desc)
        local ident = desc.ident or "    "
        local r = ""
        for i,v in ipairs(desc) do
            v.ident = ident .. "  "
            r = r .. tostring(v)
        end
        return r
    end
    return desc
end

function WCIDConfigSet(param)
     local desc = Descriptor{
        { wLength                    = 8              },
        { wDescriptorType            = 1              },
        { bConfiguration             = DUMMY          },
        { bReserved                  = 0              },
        { wTotalLength               = DUMMY         },
    }(param)
    
    desc.wTotalLength = 8
    for i,v in ipairs(param) do
        local l = nil
        l = v.wSubsetLength or v.wTotalLength or v.wLength
        if l then
            desc.wTotalLength = desc.wTotalLength + l
        end
    end 
    
    desc.outputHeader = function(desc)
        local ident = desc.ident or "    "
        return ident .. "  /* Microsoft OS 2.0 configuration subset header */ \n"
    end
    
    desc.outputTail = function(desc)
        local ident = desc.ident or "    "
        local r = ""
        for i,v in ipairs(desc) do
            v.ident = ident .. "  "
            r = r .. tostring(v)
        end
        return r
    end
    return desc
end


function WCIDDescSet(param)
    local desc = Descriptor{
        { wLength                    = 10              },
        { wDescriptorType            = 0               },
        { dwWindowsVersion           = WIN8_1          },
        { wDescriptorSetTotalLength  = DUMMY           },
    }(param)
        
    desc.wDescriptorSetTotalLength = 10
    for i,v in ipairs(param) do
        local l = nil
        l = v.wSubsetLength or v.wTotalLength or v.wLength
        if l then
            desc.wDescriptorSetTotalLength = desc.wDescriptorSetTotalLength + l
        end
    end
    
    desc.outputHeader = function(desc)
        local prefix = desc.prefix or ""
        return "#define  "..prefix.."WCID_DESC_SET_SIZE  (".. desc.wDescriptorSetTotalLength .. ")\n"
        .. "WEAK __ALIGN_BEGIN const uint8_t "..prefix.."WCIDDescriptorSet ["..desc.wDescriptorSetTotalLength.."] __ALIGN_END = {\n"
    end
    
    desc.outputTail = function(desc)
        local ident = desc.ident or "    "
        local r = ""
        for i,v in ipairs(desc) do
            r = r .. tostring(v)
        end
        r = r .. "};\n\n"
        return r
    end
    return desc
end



function WCIDCapDesc(param)
    local uuid = param.uuid
    local desc = Descriptor{
        { bLength             = DUMMY               },
        { bDescriptorType     = 16                  },
        { bDevCapabilityType  = 5                   },
        { bReserved           = 0                   },
        

    }(param)
    desc.bLength = 4
    for i, v in ipairs(desc) do
        desc.bLength = desc.bLength + v.length
    end
    desc.outputHeader = function(desc)
        local ident = desc.ident or "    "
        return ident .. "/*  WCID DEVICE_CAPABILITY descriptor */\n"
    end
    desc.outputTail = function(desc)
        local r = ""
        for i, v in ipairs(desc) do
            v.prefix = desc.prefix
            r = r .. tostring(v)
        end
        return r
    end
    
    return desc
end

function WCIDMSOSBOS(param)
    local caps_cnt = #param
    local desc = Descriptor{
        { bLength          = 5                   },
        { bDescriptorType  = "USB_DESC_TYPE_BOS" },
        { wTotalLength     = DUMMY               },
        { bNumDeviceCaps   = caps_cnt            },
    }(param)
    
    desc.wTotalLength = desc.bLength
    for i, v in ipairs(param) do
        desc.wTotalLength = desc.wTotalLength + v.bLength
    end
    desc.outputHeader = function(desc)
        local prefix = desc.prefix or ""
        return "#define  "..prefix.."WCID_BOS_SIZE  (".. desc.wTotalLength .. ")\n"
        .. "WEAK __ALIGN_BEGIN const uint8_t "..prefix.."WCIDBOS ["..desc.wTotalLength.."] __ALIGN_END = {\n"
    end
    
    desc.outputTail = function(desc)
        local ident = desc.ident or "    "
        local r = ""
        for i,v in ipairs(desc) do
            v.prefix = desc.prefix
            r = r .. tostring(v)
        end
        r = r .. "};\n\n"
        return r
    end
    return desc
end

function WCIDFeature(param)
    local cnt = #param
    local p = {}
    local varData = {}
    for i,v in ipairs(param) do
        p[#p+1] = WCIDFunction(v.interface, v.CID, v.subCID)
    end
    p.varData = varData
    local desc = Descriptor{
        {dwLength             = cnt*24+16    },
        {bcdVersion           = 0x100        }, 
        {wIndex               = 4            },
        {bCount               = cnt          },
    }(p)
    
    desc.outputHeader = function(desc)
        local prefix = desc.prefix or ""
        return "#define  "..prefix.."WCID_DESCRIPTOR_SIZE  (".. desc.dwLength .. ")\n"
        .. "WEAK __ALIGN_BEGIN const uint8_t "..prefix.."WCIDDescriptor ["..desc.dwLength.."] __ALIGN_END = {\n"
    end
    
    desc.outputTail = function(desc)
        local ident = desc.ident or "    "
        local r = ident .. outputString("0,0,0,0,0,0,0", "Reserved", #ident)
        for i,v in ipairs(desc) do
            r = r .. tostring(v)
        end
        r = r .. "};\n\n"
        return r
    end
    
    return desc
end


function WCIDPropertyToString(desc)
    local r = ""
    
    return r
end

function WCIDProperty(propType, name, data)
    local size = #name + #data + 14
    
    local desc = Descriptor{
        {dwSize              = size          },
        {dwPropertyDataType  = propType      }, 
        {wPropertyNameLength = #name         },
    }({})
    
    desc.name = name
    desc.data = data
    desc.outputHeader = function(desc)
        local r =  "    /*WCID property field */\n"
        if desc.descrption then
            r = r .. desc.descrption .. "\n"
        end
        return r
    end
    
    desc.outputTail = function(desc)
        local ident = desc.ident or "    "
        local r = "    "
        local s = _G.SIMPLE
        _G.SIMPLE = true
        for i=1,#desc.name do
            r = r .. outputChar(desc.name:byte(i), "", #ident)
            if (i&0x7) == 0 then r = r .."\n    " end
        end
        r = r .. "\n"
        _G.SIMPLE = s
        r = r .. ident .. output32(#desc.data, "dwPropertyDataLength", #ident)
        _G.SIMPLE = true
        r = r .."    "
        for i=1,#desc.data do
            r = r .. outputChar(desc.data:byte(i), "", #ident)
            if (i&0x7) == 0 then r = r .."\n    " end
        end
        _G.SIMPLE = s
        r = r .. "\n"
        return r
    end
    return desc
end

function WCIDProperties(param)
    local cnt = #param
    local desc = Descriptor{
        {dwLength             = DUMMY        },
        {bcdVersion           = 0x100        }, 
        {wIndex               = 5            },
        {wCount               = cnt          },
    }(param)
    
    local l = 10
    for i,v in ipairs(param) do
        l = l + v.dwSize
    end
    desc.dwLength = l
    
    desc.outputHeader = function(desc)
        local prefix = desc.prefix or ""
        return "#define  "..prefix.."WCID_PROPERTIES_SIZE  (".. desc.dwLength .. ")\n"
        .. "WEAK __ALIGN_BEGIN const uint8_t "..prefix.."WCIDProperties ["..desc.dwLength.."] __ALIGN_END = {\n"
    end
    
    desc.outputTail = function(desc)
        local ident = desc.ident or "    "
        local r = ""
        for i,v in ipairs(desc) do
            r = r .. tostring(v)
        end
        r = r .. "};\n\n"
        return r
    end
    
    return desc
end


function WCIDDeviceIfGUID(guids)
    local prop
    local name = "DeviceInterfaceGUID"
    local content = ""
    if type(guids) == "table" then
        local r = ""
        for i,v in ipairs(guids) do
            r = r .. expand_ascii(v)
            content = content .. "\n       " .. v
        end
        if #guids > 1 then
            r = r .. "\x00\x00"
            name = "DeviceInterfaceGUIDs"
            prop = WCIDProperty(REG_MULTI_SZ, expand_ascii("DeviceInterfaceGUIDs"), r)
        else
            prop = WCIDProperty(REG_SZ, expand_ascii("DeviceInterfaceGUID"), r)
        end
    else
        content = "\n       " .. guids
        prop = WCIDProperty(REG_SZ, expand_ascii("DeviceInterfaceGUID"), expand_ascii(guids))
    end
    prop.descrption = "  /* " .. name .. " = " .. content  .. "  */"
    return prop
end

function GUID(seed, hasTime)
    seed = seed or ""
    local r = ""
    local s = 0x123248
    for i=1,#seed do
        s = s * (seed:byte(i)+0x100)
        s = s & 0xffffffff
    end
    if hasTime then
        math.randomseed(os.clock())
        math.randomseed(s ~ os.time() ~ math.random(0,0xffffffff))
        s = math.random(0,0xffffffff)
    end
    math.randomseed(s)
    local vv = function() return string.format("%04x", math.random(0,0xffff)) end
    return string.format("{%s%s-%s-%s-%s-%s%s%s}", vv(),vv(),vv(),vv(),vv(),vv(),vv(),vv())    
end

function MakeWCID(dev)
    local hasWCID = false
    local features = {}
    local guids = {}
    local wcidInfo = {}
    for i,cfg in ipairs(dev) do
        for j,iad_or_if in ipairs(cfg) do
            if     iad_or_if.bDescriptorType == IAD_DESCRIPTOR_TYPE then
                if iad_or_if.WCID then
                    features[#features+1] = iad_or_if.WCID(iad_or_if.bFirstInterface())
                    local gid = iad_or_if.GUID or GUID( "seed" .. dev.vid .. dev.pid .. iad_or_if.iFunction)
                    guids[#guids+1] = gid
                    wcidInfo[#wcidInfo+1] = {
                        guid = gid,
                        interfaceId = iad_or_if.bFirstInterface()
                    }
                    hasWCID = true
                end
            elseif iad_or_if.bDescriptorType == INTERFACE_DESCRIPTOR_TYPE then
                if iad_or_if.WCID then
                    features[#features+1] = iad_or_if.WCID(iad_or_if.bInterfaceNumber)
                    local gid = iad_or_if.GUID or GUID( "seed" .. dev.vid .. dev.pid .. iad_or_if.iInterface)
                    guids[#guids+1] = gid
                    wcidInfo[#wcidInfo+1] = {
                        guid = gid,
                        interfaceId = iad_or_if.bInterfaceNumber
                    }
                    hasWCID = true
                end
            end
        end
    end
    
    if dev.bcdUSB >= 0x210 then
        dev.hasWCID_20 = true
        hasWCID = false
    end
    
    dev.hasWCID = hasWCID
    if hasWCID then
        
        local osStringDesc = MSOSString("WCID_VENDOR_CODE")
        osStringDesc.prefix = "WCID_"
        local r = "\n// Define WCID os string descriptor \n" 
        r = r .. "#ifndef WCID_MSOS_STRING\n"
        r = r .. "#define WCID_MSOS_STRING\n"
        r = r .. tostring(osStringDesc)
        r = r .. "#endif\n\n"
        
        local wcidFeature = WCIDFeature(features)
        wcidFeature.prefix = dev.prefix
        
        local props = ""
        local fields = ""
        for i, guid in ipairs(guids) do
            local wcidProperties = WCIDProperties{WCIDDeviceIfGUID(guid)}
            wcidProperties.prefix = dev.prefix .. "_IF" .. (i-1) .. "_"
            props = props .. "\n"  .. tostring(wcidProperties) .. "\n"
            fields = fields .. dev.prefix .. "_IF" .. (i-1) .. "_" .. "WCIDProperties,\n"
        end
        props = props .. "#define " .. dev.prefix .. "WCID_PROPERTIES_SIZE (" .. #guids .. ")\n"
        props = props .. "WEAK __ALIGN_BEGIN const desc_t " .. dev.prefix .. "WCIDProperties[ " .. dev.prefix .. "WCID_PROPERTIES_SIZE ] __ALIGN_END = {\n"
         .. fields .. "\n};\n"
        
        local wcidProperties = WCIDProperties{WCIDDeviceIfGUID(guids)}
        wcidProperties.prefix = dev.prefix
        return "\n\n#if defined(HAS_WCID)\n"
        .. r .. tostring(wcidFeature) .. props .. "\n"
        .. "#endif\n\n"
    end
    
    if dev.hasWCID_20 then
        local sets = {}
        if   dev[1].bNumInterfaces > 1 or #wcidInfo > 1 then
            for i,v in ipairs(wcidInfo) do
                sets[#sets+1] = WCIDFunctionSet{
                    bFirstInterface = v.interfaceId,
                    WCIDWinUSB("WINUSB"),
                    WCIDRegDesc("DeviceInterfaceGUIDs", {v.guid} ), 
                }
            end
        else
            sets = {
                WCIDWinUSB("WINUSB"),
                WCIDRegDesc("DeviceInterfaceGUIDs", {wcidInfo[1].guid} ), 
            }
        end
    
        local osDescSet = WCIDDescSet(sets)
    
    
        local osCapDesc = WCIDMSOSBOS{
            WCIDCapDesc{
                WCIDCapWinUSB{
                }
            }
        }
        osDescSet.prefix = dev.prefix
        osCapDesc.prefix = dev.prefix
        local r = ""
        r = r .. tostring(osDescSet) .. tostring(osCapDesc) .. "\n"
        return r
    end
    
    return ""
end

--[[
print(MSOSString(0x12))
print(WCIDFeature{WinUSB(1)})
print(WCIDProperties{
    WCIDDeviceIfGUID{
        GUID(),
        GUID(),
    },
})
--]]

--[[
print (WCIDMSOSBOS{
            WCIDCapDesc{
                WCIDCapWinUSB{}
            }
})

print (
    WCIDDescSet{
        WCIDConfigSet{ -- not required when only one configuration
            bConfiguration = 0,
            WCIDFunctionSet{
                bFirstInterface = 0,
                WCIDWinUSB("WINUSB"),
                WCIDRegDesc("DeviceInterfaceGUIDs", {"{CDB3B5AD-293B-4663-AA36-1AAE46463776}"} ), 
            },
            WCIDFunctionSet{
                bFirstInterface = 1,
                WCIDWinUSB("WINUSB"),
                WCIDRegDesc("DeviceInterfaceGUIDs", {"{CDB3B5AD-293B-4663-AA36-1AAE46463776}"} ), 
            },
            WCIDFunctionSet{
                bFirstInterface = 2,
                WCIDWinUSB("WINUSB"),
                WCIDRegDesc("DeviceInterfaceGUIDs", {"{CDB3B5AD-293B-4663-AA36-1AAE46463776}"} ), 
            },
        }
    }
)

--]]



