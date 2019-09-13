-- usb_common_desc.lua
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

require("usb_desc")

function Device(param)
    local cfgCount = #param
    if #param ~= 1 then error("Only support 1 config") end
    -- init device descriptor
    local desc = Descriptor {
       -- field               default value
        {bLength            = DUMMY                    },
        {bDescriptorType    = DEVICE_DESCRIPTOR_TYPE   },
        {bcdUSB             = 0x200                    },
        {bDeviceClass       = 0                        },
        {bDeviceSubClass    = 0                        },
        {bDeviceProtocol    = 0                        },
        {bMaxPacketSize     = 64                       },
        {idVendor           = 0x2017                   },
        {idProduct          = 0x0924                   },
        {bcdDevice          = 0x100                    },
        {iManufacture       = 0                        },
        {iProduct           = 0                        },
        {iSerial            = 0                        },
        {bNumConfigurations = cfgCount                 },
    }(param)
    desc.hasReport = false
    -- check if IAD needed
    for i,v in ipairs(param) do
        if v.hasIAD then
            desc.bDeviceClass = 0xef
            desc.bDeviceSubClass = 0x02
            desc.bDeviceProtocol = 0x01
            break
        end
        if v.hasReport then
            desc.hasReport = true
        end
    end
    
    desc.prefix = ""
    if param.prefix then
        local t = tostring(param.prefix):upper() .. "_"
        desc.prefix = t
    end
    desc.strings = {}
    desc.stringMap = {}
    function addStr(d, field)
        local sname = "str" .. field
        local dname = "i" .. field
        if d[sname] and d.default[dname] then
            if not desc.stringMap[d[sname]] then
                desc.stringMap[d[sname]] = true
                desc.strings[#desc.strings + 1] = StringDescriptor(d[sname])
                desc.strings[#desc.strings].index = #desc.strings - 1
                desc.strings[#desc.strings].prefix = desc.prefix
            end
            d[dname] = #desc.strings - 1
        end
    end
    -- set langID descriptor
    desc.strings[1] = StringDescriptor{ varData = param.langID or {{ wLangID0 = 0x0409 }}}
    desc.strings[1].index = 0
    desc.strings[1].prefix = desc.prefix
    -- check device string descriptor
    addStr(desc, "Manufacture")
    addStr(desc, "Product")
    addStr(desc, "Serial")
    for i, cfg in ipairs(desc) do
        -- check config string descriptor
        addStr(cfg, "Configuration")
        for j, v in ipairs(cfg) do
            -- check IAD or interface string descriptor
            if v.bDescriptorType == IAD_DESCRIPTOR_TYPE then
                addStr(v, "Function")
                for k, int in ipairs(v) do
                    addStr(int, "Interface")
                end
            else
                addStr(v, "Interface")
            end
        end
    end
    
    desc.vid = desc.idVendor
    desc.pid = desc.idProduct
    desc.idVendor = desc.prefix .. "VID"
    desc.idProduct = desc.prefix .. "PID"
    desc.declare = "#define  "..desc.prefix.."DEVICE_DESCRIPTOR_SIZE  (".. desc.bLength .. ")\n"
     .. "extern const uint8_t "..desc.prefix.."DeviceDescriptor ["..desc.bLength.."];\n"
    desc.outputHeader = "#define  "..desc.prefix.."DEVICE_DESCRIPTOR_SIZE  (".. desc.bLength .. ")\n"
    .. "__ALIGN_BEGIN  const uint8_t "..desc.prefix.."DeviceDescriptor ["..desc.bLength.."] __ALIGN_END = {\n"
    -- output tail and followed config descriptor and string descriptor
    desc.outputTail = function(desc)
        local r = "};\n\n"
        r = r .. "// Configs \n"
        for i,cfg in ipairs(desc) do
            cfg.prefix = desc.prefix
            r = r .. tostring(cfg)
            desc.declare = desc.declare .. (cfg.declare or "")
        end
        r = r .. "// Strings \n"
        local str_holder = ""--"#define ".. desc.prefix .."STRING_COUNT   " .. #desc.strings .. "\n"
        desc.declare = desc.declare .. "extern const uint8_t* const "..desc.prefix.."StringDescriptors[".. desc.prefix .."STRING_COUNT];\n"
        str_holder = str_holder .. "const uint8_t* const "..desc.prefix.."StringDescriptors[".. desc.prefix .."STRING_COUNT] = {\n"
        for i, str in ipairs(desc.strings) do
            str_holder = str_holder .. desc.prefix .."StringDescriptor" .. (i-1)  .. ",\n"
            r = r .. tostring(str)
        end
        r = r .. str_holder .. "};\n\n"
        r = r .. MakeWCID(desc)
        desc.declare = desc.declare .. [[extern const tusb_descriptors ]]..desc.prefix..[[descriptors;]] .. "\n"
        r = r .. "//  Device descriptors\n"
        r = r .. [[
const tusb_descriptors ]]..desc.prefix..[[descriptors = {
  .device = ]]..desc.prefix..[[DeviceDescriptor,
  .config = ]]..desc.prefix..[[ConfigDescriptor,
  .strings = ]]..desc.prefix..[[StringDescriptors,
  .string_cnt = ]]..desc.prefix..[[STRING_COUNT,
#if defined(HAS_WCID)
#if defined(]]..desc.prefix..[[WCID_DESCRIPTOR_SIZE)
  .wcid_desc = ]]..desc.prefix..[[WCIDDescriptor,
#else
  .wcid_desc = 0,  
#endif // ]]..desc.prefix..[[WCID_DESCRIPTOR_SIZE)

#if defined(]]..desc.prefix..[[WCID_PROPERTIES_SIZE)
  .wcid_properties = ]]..desc.prefix..[[WCIDProperties,
#else
  .wcid_properties = 0,  
#endif // ]]..desc.prefix..[[WCID_PROPERTIES_SIZE

#endif // HAS_WCID

#if defined(HAS_WCID_20)
#if defined(]]..desc.prefix..[[WCID_BOS_SIZE)
  .wcid_bos = ]]..desc.prefix..[[WCIDBOS,
#else
  .wcid_bos = 0,  
#endif // ]]..desc.prefix..[[WCID_BOS_SIZE)

#if defined(]]..desc.prefix..[[WCID_DESC_SET_SIZE)
  .wcid_desc_set = ]]..desc.prefix..[[WCIDDescriptorSet,
#else
  .wcid_desc_set = 0,  
#endif // ]]..desc.prefix..[[WCID_DESC_SET_SIZE


#endif // HAS_WCID_20
};
]]
        
        return r
    end
    return desc
end

function Config(param)
    local attr = 0x80
    -- parse the attr from the given param
    if param.SelfPower then attr = attr + 0x40 end
    if param.RemoteWakeup then attr = attr + 0x20 end
    if param.bmAttributes then attr = param.bmAttributes end
    local desc = Descriptor{
        {bLength           = DUMMY                     },
        {bDescriptorType   = CONFIGURATION_DESCRIPTOR_TYPE },
        {wTotalLength      = DUMMY                     },
        {bNumInterfaces    = DUMMY                     },
        {bConfigurationValue = 1                       },
        {iConfiguration    = 0                         },
        {bmAttributes      = attr                      },
        {bMaxPower         = 100                       }, 
    }(param)
    
    desc.wTotalLength = desc.bLength
    desc.hasIAD = false
    desc.hasReport = false
    desc.bNumInterfaces = 0
    for i,v in ipairs(param) do
        -- check if we need IAD or not
        if v.bDescriptorType == IAD_DESCRIPTOR_TYPE then
            desc.hasIAD = #param > 1
            v.outputIAD = #param > 1
            if v.outputIAD then desc.wTotalLength = desc.wTotalLength + v.bLength end
            for j,int in ipairs(v) do
                int.bInterfaceNumber = desc.bNumInterfaces
                desc.bNumInterfaces = desc.bNumInterfaces + 1
                desc.wTotalLength = desc.wTotalLength + int.wTotalLength
                desc.hasReport = desc.hasReport or int.hasReport
            end
        else -- otherwise, interface without IAD
            v.bInterfaceNumber = desc.bNumInterfaces
            desc.bNumInterfaces = desc.bNumInterfaces + 1
            desc.wTotalLength = desc.wTotalLength + v.wTotalLength
            desc.hasReport = desc.hasReport or v.hasReport
        end
    end
    desc.iConfiguration = 0
    desc.declare = ""
    desc.outputHeader = function()
        local r = ""
        for i,v in ipairs(desc) do
            if v.bDescriptorType == IAD_DESCRIPTOR_TYPE then
                for j,int in ipairs(v) do
                    if int.extraDesc then
                        int.prefix = desc.prefix
                        r = r .. int:extraDesc()
                        desc.declare = desc.declare .. (int.declare or "")
                    end
                end
            else
                if v.extraDesc then
                    v.prefix = desc.prefix
                    r = r .. v:extraDesc()
                    desc.declare = desc.declare .. (v.declare or "")
                end
            end
        end
        desc.declare = desc.declare .. "#define  "..desc.prefix.."CONFIG_DESCRIPTOR_SIZE  (".. desc.wTotalLength .. ")\n"
        .. "extern const uint8_t "..desc.prefix.."ConfigDescriptor ["..desc.wTotalLength.."];\n"
        local r = r .. "#define  "..desc.prefix.."CONFIG_DESCRIPTOR_SIZE  (".. desc.wTotalLength .. ")\n"
        .. "__ALIGN_BEGIN const uint8_t "..desc.prefix.."ConfigDescriptor ["..desc.wTotalLength.."] __ALIGN_END = {\n"
        return r
    end
    
    desc.outputTail = function(desc)
        local r = ""
        for i,v in ipairs(desc) do
            if v.bDescriptorType == IAD_DESCRIPTOR_TYPE then
                if v.outputIAD then r = r .. tostring(v) end
                for j,int in ipairs(v) do r = r .. tostring(int) end
            else
                r = r .. tostring(v)
            end
        end
        r = r .. "};\n"
        r = r .. "\n// Power status\n"
        if (desc.bmAttributes & 0x40) == 0x40 then
            desc.declare = desc.declare .. "#define "..desc.prefix.."DEV_STATUS0      USB_CONFIG_SELF_POWERED\n"
            r = r .. "#define "..desc.prefix.."DEV_STATUS0      USB_CONFIG_SELF_POWERED\n"
        else
            desc.declare = desc.declare .. "#define "..desc.prefix.."DEV_STATUS0      (0)\n"
            r = r .. "#define "..desc.prefix.."DEV_STATUS0      (0)\n"
        end
        if (desc.bmAttributes & 0x20) == 0x20 then
            desc.declare = desc.declare .. "#define "..desc.prefix.."DEV_STATUS1      USB_CONFIG_REMOTE_WAKEUP\n"
            r = r .. "#define "..desc.prefix.."DEV_STATUS1      USB_CONFIG_REMOTE_WAKEUP\n"
        else
            desc.declare = desc.declare .. "#define "..desc.prefix.."DEV_STATUS1      (0)\n"
            r = r .. "#define "..desc.prefix.."DEV_STATUS1      (0)\n"
        end
        desc.declare = desc.declare .. "#define "..desc.prefix.."DEV_STATUS    (("..desc.prefix.."DEV_STATUS0) |("..desc.prefix.."DEV_STATUS1) )\n\n"
        r = r .. "#define "..desc.prefix.."DEV_STATUS    (("..desc.prefix.."DEV_STATUS0) |("..desc.prefix.."DEV_STATUS1) )\n\n"
        r = r .. desc.prefix .. "TXEP_MAX_SIZE\n" .. desc.prefix .. "RXEP_MAX_SIZE\n\n"
        return r
    end
    return desc
end

function IAD(param)
    if #param < 2 then
        error("IAD need at least 2 interface")
    end
    for i,v in ipairs(param) do
        if v.bDescriptorType ~= INTERFACE_DESCRIPTOR_TYPE then
            error("Descriptor " .. i .. " in IAD is not Interface")
        end
    end
    local intFirst = param[1]
    local intLast = param[#param]
    local desc = Descriptor{
                {bLength                = DUMMY                          },
                {bDescriptorType        = IAD_DESCRIPTOR_TYPE            },
                {bFirstInterface        = function() return intFirst.bInterfaceNumber end },
                {bInterfaceCount        = #param                         },
                {bFunctionClass         = intFirst.bInterfaceClass       },
                {bFunctionSubClass      = intFirst.bInterfaceSubClass    },
                {bFunctionProtocol      = intFirst.bInterfaceProtocol    },
                {iFunction              = 0                              },
    }(param)
    desc.WCID = param.WCID
    desc.GUID = param.GUID
    desc.outputHeader = "  /* IAD descriptor */\n"
    desc.outputTail = ""
    return desc
end

function Interface(param)
    local epCnt = 0
    local epLength = 0 
    for i,v in ipairs(param) do
        if v.bDescriptorType == ENDPOINT_DESCRIPTOR_TYPE then
            epCnt = epCnt + 1
        end
        epLength = epLength + v.bLength
    end
    local desc = Descriptor{
        {bLength                  = DUMMY                        },
        {bDescriptorType          = INTERFACE_DESCRIPTOR_TYPE    }, 
        {bInterfaceNumber         = DUMMY                        }, -- assigned by the config parser
        {bAlternateSetting        = 0                            },
        {bNumEndpoints            = epCnt                        },
        {bInterfaceClass          = 0xff                         },
        {bInterfaceSubClass       = 0xff                         },
        {bInterfaceProtocol       = 0                            },
        {iInterface               = 0                            },
    }(param)
    desc.WCID = param.WCID
    desc.GUID = param.GUID
    desc.wTotalLength = epLength + desc.bLength
    desc.outputHeader = desc.ident .. "/* Interface descriptor, len: "..desc.wTotalLength.."*/\n"
    desc.outputTail = function(desc)
        local r = ""
        for i,v in ipairs(desc) do
            r = r .. tostring(v)
        end
        return r
    end
    return desc
end

BulkDouble = 4
Bulk = 2
Interrupt = 3
ISO = 1
Control = 0
function IN(ep)
    return ((ep & 0x7f) | 0x80)
end
function OUT(ep)
    return ep & 0x7f
end

function EndPoint(param, attr, size, interval)
    if attr then
        return EndPoint{
            bEndpointAddress = param,
            bmAttributes = attr,
            wMaxPacketSize = size,
            bInterval = interval,
        }
    end
    
    local doubleBuffer = false
    if param.bmAttributes == BulkDouble then
        param.bmAttributes = Bulk
        doubleBuffer = true
    end
    
    local desc = Descriptor{
        {bLength              = DUMMY                        },
        {bDescriptorType      = ENDPOINT_DESCRIPTOR_TYPE     }, 
        {bEndpointAddress     = DUMMY                        },
        {bmAttributes         = DUMMY                        },
        {wMaxPacketSize       = 64                           },
        {bInterval            = 1                            },
    }(param)
    desc.outputHeader = desc.ident .. "/* EndPoint descriptor */\n"
    desc.outputTail = ""
    desc.doubleBuffer = doubleBuffer
    return desc
end

function Function(param)
    local desc = Descriptor{
        {bLength              = DUMMY                        },
        {bDescriptorType      = 0x24                         },
        {bDescriptorSubtype   = DUMMY                        },
    }(param)
    desc.outputHeader = desc.ident .. "/* Function descriptor " .. (param.alias or "") .. " */\n"
    desc.outputTail = ""
    return desc
end

local function disStr(str)
    local r = ""
    for i=1,#str do
        local ch = str:byte(i)
        if ch >= 0x20 and ch<= 0x7f then
            r = r .. str:sub(i,i)
        else
            r = r .. string.format("\\x%02x", ch)
        end
    end
    return r
end

function StringDescriptor(param)
    if type(param) == "string" then
        local p = {}
        p.varData = {}
        local i = 1
        while i<=#param do
            if param:byte(i)>0x7f and i<#param then
                p.varData[#p.varData+1] = {
                    ["wcChar"..(i-1)] = param:byte(i) | (param:byte(i+1)<<8)
                }
                i = i + 1
            else
                p.varData[#p.varData+1] = {
                    ["wcChar"..(i-1)] = param:byte(i)
                }
            end
            i = i + 1
        end
        local d = StringDescriptor(p)
        d.string = param
        return d
    end
    
    local desc = Descriptor{
        {bLength              = DUMMY                        },
        {bDescriptorType      = STRING_DESCRIPTOR_TYPE       }, 
    }(param)
    
    desc.string = ""
    for i,var in ipairs(param.varData) do
        for k, v in pairs(var) do
            local l,h = splitU16(v)
            desc.string = desc.string .. string.format("\\x%02x\\x%02x",l,h)
        end
    end
    desc.outputHeader = function(desc)
        local prefix = desc.prefix or ""
        local r = "#define "..prefix.."STRING_DESCRIPTOR" .. (desc.index or 0) .. '_STR          "'..disStr(desc.string)..'"\n'
        r = r .. "#define "..prefix.."STRING_DESCRIPTOR" .. (desc.index or 0) .. '_SIZE          ('..desc.bLength..')\n'
        return r .. "WEAK __ALIGN_BEGIN const uint8_t  "..prefix.."StringDescriptor".. (desc.index or 0) .." [" .. desc.bLength .. "] __ALIGN_END = {\n"
    end
    desc.outputTail = "};\n"
    return desc
end


