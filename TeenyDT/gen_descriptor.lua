-- gen_descriptor.lua
require("usb_init_ep")
require("usb_cdc_acm")
require("usb_hid")
require("usb_wcid")
local function usage()
    print("usage:")
    print("  lua usb_gen_desc.lua <inFile> [-maxep=<max ep number>] [-maxmem=<memory size in bytes>]")
    print("    inFile  - Input usb descriptor define file name")
    print("    -maxep  - max end point number")
    print("    -maxmem - max valid memory for USB core")
end

function input_name()
    local name = "<NONE>"
    string.gsub(arg[1], "([%w_%.]+)$", function(n)
        name = n
    end)
    return name
end

function outputCode(fname, script, content, isBody)
    isBody = isBody or (fname:sub(-1,-1) == 'c')
    local f = io.open(fname, "w+")
    assert(f, "Fail to open " .. fname .. " to write")
    while fname:find('[/\\]') do
        fname = fname:sub( fname:find('[/\\]') + 1 )
    end
    defname = "_" .. fname:upper() .. "_"
    defname = string.gsub(defname, '[%./\\]', '_')
    f:write([[
/*
 * Name   :  ]]..fname.."\n"..[[
 * Author :  admin@xtoolbox.org
 * Date   :  ]]..os.date("%Y-%m-%d %H:%M:%S")..[[

 * Desc   :  This file is auto generate by the teeny_usb script tool
 *           Visit https://github.com/xtoolbox/TeenyUSB for more info
 */

/*
  Input source name:  ]]..input_name()..[[
  
  Content type is lua script:
  ------------- lua script begin ------------
]]..script..[[
  

  ------------- lua script end   ------------
 */
]])

    if not isBody then
    f:write([[
#ifndef ]]..defname..[[

#define ]]..defname..[[

// include this file in "usbd_conf.h"

// forward declare the tusb_descriptors struct
typedef struct _tusb_descriptors tusb_descriptors;
]])
    else
    f:write([[
#include "teeny_usb.h"
]])
    end
    f:write(content)
    if isBody then
        f:write("\n\n")
    else
        f:write("\n#endif\n\n")
    end
    f:close()
end

function collectDescriptor(desc, maxEp, memSize)
local descriptor = ""
local epDefine = ""
if desc.bDescriptorType == DEVICE_DESCRIPTOR_TYPE then
    descriptor = tostring(desc)
    epDefine = EndPointInfo(desc, desc.maxEp or maxEp, desc.memSize or memSize):getInitCode()
else
    for i,v in ipairs(desc) do
        descriptor = descriptor .. [[
/////////////////////////////////////////////////////////////////////
//// Descriptor for device]]..i..[[ define begin
/////////////////////////////////////////////////////////////////////
]] .. tostring(v) .. [[
/////////////////////////////////////////////////////////////////////
//// Descriptor for device]]..i..[[  define end
/////////////////////////////////////////////////////////////////////



]]
        epDefine = epDefine .. [[
/////////////////////////////////////////////////////////////////////
//// EndPoint for device]]..i..[[ define begin
/////////////////////////////////////////////////////////////////////
]] .. EndPointInfo(v, v.maxEp or maxEp, v.memSize or memSize):getInitCode() .. [[
/////////////////////////////////////////////////////////////////////
//// EndPoint for device]]..i..[[ define end
/////////////////////////////////////////////////////////////////////



]]

    end -- end for
end -- end if

    return descriptor, epDefine
end -- end function


if not arg or #arg < 1 then
    usage()
    return
end

local MAX_EP = 7
local MAX_MEM = 1024
local inputName = arg[1]

for i,v in ipairs(arg) do
    local p1,p2
    p1 = v:find("-maxep=")
    p2 = v:find("-maxmem=")
    if p1 then MAX_EP = tonumber(v:sub(p1+7)) or MAX_EP end
    if p2 then MAX_MEM = tonumber(v:sub(p2+8)) or MAX_MEM end
    if not p1 and not p2 then inputName = v end
end


local f = io.open(arg[1], "r")
assert(f,"Fail to open <"..arg[1]..">")

local s = f:read("*a")
f:close()

local desc = load(s)()

local body = string.gsub(inputName, "([%w_%.]+)$", 'teeny_usb_desc.c')
local header = string.gsub(inputName, "([%w_%.]+)$", 'teeny_usb_init.h')

local descs, epDefs = collectDescriptor(desc, MAX_EP, MAX_MEM)
outputCode(body, s, descs)
outputCode(header, s, epDefs)

