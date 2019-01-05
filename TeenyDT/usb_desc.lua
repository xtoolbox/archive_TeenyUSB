-- usb_desc.lua
--[[ 
 Author:  admin@xtoolbox.org
 
 USB descriptor base class, all other descriptors are generate by Descriptor
 example:
 Create a Device descriptor class
 local DeviceDescriptorClass = Descriptor {
          -- field              default value, DUMMY means must filled by user
        {   bLength            = DUMMY                    },
        {   bDescriptorType    = DEVICE_DESCRIPTOR_TYPE   },
        {   bcdUSB             = 0x200                    },
        {   bDeviceClass       = 0                        },
        {   bDeviceSubClass    = 0                        },
        {   bDeviceProtocol    = 0                        },
        {   bMaxPacketSize     = 8                        },
        {   idVendor           = 0x0483                   },
        {   idProduct          = 0x3748                   },
        {   bcdDevice          = 0x100                    },
        {   iManufacture       = 0                        },
        {   iProduct           = 0                        },
        {   iSerial            = 0                        },
        {   bNumConfigurations = 1                        },
    }
    
  Create a real device descriptor

  local deviceDescriptor = DeviceDescriptorClass {
    bcdUSB = 0x110,
    idVendor = 0x1234,
    idProduct = 0x5678,
    strManufacture = "Test Manufacture",
    strProduct = "Test Product",
    strSerial = "Test Serial",
  }
 
  print( tostring(deviceDescriptor) )

]]

SIMPLE = SIMPLE or false
VERBOSE = VERBOSE or false

DEVICE_DESCRIPTOR_TYPE           =   SIMPLE and 0x01 or "USB_DEVICE_DESCRIPTOR_TYPE"
CONFIGURATION_DESCRIPTOR_TYPE    =   SIMPLE and 0x02 or "USB_CONFIGURATION_DESCRIPTOR_TYPE"
STRING_DESCRIPTOR_TYPE           =   SIMPLE and 0x03 or "USB_STRING_DESCRIPTOR_TYPE"
INTERFACE_DESCRIPTOR_TYPE        =   SIMPLE and 0x04 or "USB_INTERFACE_DESCRIPTOR_TYPE"
ENDPOINT_DESCRIPTOR_TYPE         =   SIMPLE and 0x05 or "USB_ENDPOINT_DESCRIPTOR_TYPE"
IAD_DESCRIPTOR_TYPE              =   SIMPLE and 0x0B or "USB_IAD_DESCRIPTOR_TYPE"
DUMMY = {}
function info(...)
    if not VERBOSE then return end
    io.write("Info: ", ...)
    io.write("\n")
end
function warning(...)
    io.write("Warning: ", ...)
    io.write("\n")
end


function splitU16(v)
    return v&0xff, v>>8
end
function splitU32(v)
    return v & 0xff, (v>>8)&0xff, (v>>16)&0xff, (v>>24)&0xff
end


local comment_col = 50
local fmt = string.format
function output8(v, desc, ident)
    local r = fmt("0x%02x, ",v)
    if SIMPLE then return r end
    ident = ident or 0
    r = r .. string.rep(" ", comment_col - #r - ident)
    return r .. "/* " .. desc .. " */\n"
end
function output16(v, desc, ident)
    local l,h = splitU16(v)
    local r = fmt("0x%02x, 0x%02x, ", l, h)
    if SIMPLE then return r end
    ident = ident or 0
    r = r .. string.rep(" ", comment_col - #r - ident)
    return r .. "/* " .. desc .. " */\n"
end
function output32(v, desc, ident)
    local l,h,hl,hh = splitU32(v)
    local r = fmt("0x%02x, 0x%02x, 0x%02x, 0x%02x, ", l, h, hl, hh)
    if SIMPLE then return r end
    ident = ident or 0
    r = r .. string.rep(" ", comment_col - #r - ident)
    return r .. "/* " .. desc .. " */\n"
end

function outputChar(ch, desc, ident)
    local r = ""
    if ch>=0x20 and ch<=0x7f then
        r = r .. fmt("'%s', ", string.char(ch))
    else
        r = r .. fmt("0x%02x, ", ch)
    end
    if SIMPLE then return r end
    ident = ident or 0
    r = r .. string.rep(" ", comment_col - #r - ident)
    return r .. "/* " .. desc .. " */\n"
end

function outputWChar(v, desc, ident)
    local l,h = splitU16(v)
    local r = ""
    if h == 0 and l>=0x20 and l<=0x7f then
        r = r .. fmt("'%s', 0x%02x, ", string.char(l), h)
    else
        r = r .. fmt("0x%02x, 0x%02x, ", l, h)
    end
    if SIMPLE then return r end
    ident = ident or 0
    r = r .. string.rep(" ", comment_col - #r - ident)
    return r .. "/* " .. desc .. " */\n"
end

local fieldSize
-- String means the value is a macro define
function outputString(v, desc, ident)
    ident = ident or 0
    if fieldSize(desc) == 2 then
        v = "LOBYTE("..v.."), HIBYTE(" .. v .. ")"
    end
    if SIMPLE then return v .. ", " end
    return v ..','.. string.rep(" ", comment_col - #v - 1 - ident) .. "/* " .. desc .. " */\n"
end

local field_prefix_table = {
    b = output8,
    bm = output8,
    i = output8,
    id = output16,
    w = output16,
    bcd = output16,
    wc = outputWChar,
    dw = output32,
}

local function fieldPrefix(field)
    local pre = string.find(field, "[A-Z]")
    local post = nil
    if pre and pre>1 then
        post = field:sub(pre)
        pre = field:sub(1,pre-1)
    end
    --if not pre then error("unknown prefix of field \"" .. field .. '"') end
    return pre, post
end

fieldSize = function(field)
    local pre = fieldPrefix(field)
    if field_prefix_table[pre] == output16 then return 2 end
    if field_prefix_table[pre] == outputWChar then return 2 end
    if field_prefix_table[pre] == output8 then return 1 end
    if field_prefix_table[pre] == output32 then return 4 end
    --error("unknown field size " .. tostring(filed))
    return -1
end

-- every cnt output a newline
local function newLine(cnt)
    cnt = cnt or 16
    local init = 0
    return function()
        init = init + 1
        if init == cnt then
            init = 0
            return "\n"
        end
        return ""
    end
end

function outputField(desc, field)
    local ident = desc.ident or "    "
    if type(field) == "table" then
        -- parameter is layout table
        local r = ""
        local nl = newLine(8)
        for i,v in ipairs(field) do
            if SIMPLE then
                r = r .. outputField(desc, v) .. nl()
            else
                r = r .. ident .. outputField(desc, v)
            end
        end
        return r
    end
    if not field then
        if not desc.layout then error("Descriptor layout not set for " .. tostring(desc.bDescriptorType) .. " descriptor") end
        return outputField(desc, desc.layout)
    end
    local pre = fieldPrefix(field)
    local output = field_prefix_table[pre]
    if not output then error("Unknown prefix " .. tostring(pre) .. " of field " .. tostring(field)) end
    -- output string field, output direct
    if type(desc[field]) == "string" then return outputString(desc[field],field,#ident) end
    -- output function field, evalute it then output
    if type(desc[field]) == "function" then return output(desc[field](),field,#ident) end
    -- output number field
    return output(desc[field],field,#ident)
end

-- descriptor init function, bind to __call meta method
local function descInit(desc, param)
    if param.varData then
        for i,v in ipairs(param.varData) do
            for name, value in pairs(v) do
                desc.layout[#desc.layout+1] = name
                desc[name] = value
                desc.paramLength = desc.paramLength + fieldSize(name)
                break
            end
        end
    end
    for k,v in pairs(desc.default) do
        if param[k] then
            desc.default[k] = param[k]
        end
    end
    desc.bLength = desc.headerLength + desc.paramLength
    
    for i,v in ipairs(param) do
        desc[i] = v
    end
    -- copy strings in param to descriptor
    for k,v in pairs(param) do
        local pre,post = fieldPrefix(k)
        local field = "i" .. tostring(post)
        -- field prefix is strXXX, and iXXX exist in the layout
        if pre == "str" and desc.default[field] then
            desc[k] = v
        end
    end
    return desc
end

-- descriptor to string, bind to __tostring meta method
local function descToString(desc)
    local r = ""
    if desc.outputHeader then
        if type(desc.outputHeader) == "string" then r = desc.outputHeader end
        if type(desc.outputHeader) == "function" then r = desc:outputHeader() end
    end
    r = r .. outputField(desc)
    if desc.outputTail then
        if type(desc.outputTail) == "string" then r = r .. desc.outputTail end
        if type(desc.outputTail) == "function" then r = r .. desc:outputTail() end
    end
    return r
end

-- check the descriptor default value, this function is bind to __index meta method
local function check_default(desc, field)
    -- field is number, means not k-v pair, ignore
    if type(field) == "number" then return nil end
    -- find the value in default table
    local v = desc.default and desc.default[field] or nil
    if v then
        -- set to default value
        info("Set ["..field.."] to default value " .. tostring(v) )
        desc[field] = v
        return v
    end
    -- otherwise rise an error
    if desc.layoutMap[field] then
        error('Fied "'..field..'" not set in ' .. tostring(desc.default.bDescriptorType) .. " descriptor")
    end
end

function Descriptor(initTable)
    local desc = {}
    desc.layout = {}
    desc.default = {}
    desc.layoutMap = {}
    desc.headerLength = 0
    desc.paramLength = 0
    for i,v in ipairs(initTable) do
        for field, value in pairs(v) do
            desc.default[field] = value
            if value == DUMMY then desc.default[field] = false end
            desc.layout[i] = field
            desc.layoutMap[field] = true
            desc.headerLength = desc.headerLength + fieldSize(field)
            break
        end
    end
    desc.ident = "    "
    setmetatable(desc, {
        __call = descInit,
        __tostring = descToString,
        __index = check_default,
    })
    return desc
end

