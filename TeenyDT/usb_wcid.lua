-- usb_wcid.lua
--[[
     Windows compatible ID related descriptors
     visit https://github.com/pbatard/libwdi/wiki/WCID-Devices for more info
--]]
require("usb_common_desc")


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
    for i,cfg in ipairs(dev) do
        for j,iad_or_if in ipairs(cfg) do
            if     iad_or_if.bDescriptorType == IAD_DESCRIPTOR_TYPE then
                if iad_or_if.WCID then
                    features[#features+1] = iad_or_if.WCID(iad_or_if.bFirstInterface())
                    guids[#guids+1] = GUID( "seed" .. dev.vid .. dev.pid .. iad_or_if.iFunction)
                    hasWCID = true
                end
            elseif iad_or_if.bDescriptorType == INTERFACE_DESCRIPTOR_TYPE then
                if iad_or_if.WCID then
                    features[#features+1] = iad_or_if.WCID(iad_or_if.bInterfaceNumber)
                    guids[#guids+1] = GUID( "seed" .. dev.vid .. dev.pid .. iad_or_if.iInterface)
                    hasWCID = true
                end
            end
        end
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
        local wcidProperties = WCIDProperties{WCIDDeviceIfGUID(guids)}
        wcidProperties.prefix = dev.prefix
        return "\n\n#if defined(HAS_WCID)\n"
        .. r .. tostring(wcidFeature) .. tostring(wcidProperties) .. "\n"
        .. "#endif\n\n"
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






