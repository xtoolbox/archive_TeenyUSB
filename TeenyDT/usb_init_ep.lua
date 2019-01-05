-- usb_init_ep.lua
--[[ 
 Author:  admin@xtoolbox.org
 Parse the end point usage and generate ep init code
--]]

require("usb_common_desc")

local macros = {
    EP_BUF_DESC_TABLE_SIZE = '(8)',
    
}

local ep_type_t = {
    [Bulk]      = "USB_EP_BULK",
    [Interrupt] = "USB_EP_INTERRUPT",
    [ISO]       = "USB_EP_ISOCHRONOUS",
    [Control]   = "USB_EP_CONTROL",
}
local function ep_type(t)
    if ep_type_t[t] then return ep_type_t[t] end
    return "EP_UNKNOWN"
end
local gsub = function(str, t)
    return string.gsub(str, "%$(%w+)", t)
end

local hex16 = function(v)
    return string.format("0x%04x", v)
end

local PARAM_COL = 60

local function getInitCode(epInfo)
    local r = ""
    local dev = epInfo.dev
    
    local function MACRO(name, value)
        local r = "#define " .. (dev.prefix or "") .. name
        if type(value) == "number" then value = '(' .. tostring(value) .. ')' end
        r = r .. string.rep(" ", PARAM_COL-#r) .. value
        return r .. "\n"
    end
    r = r .. MACRO("VID", hex16(dev.vid))
    r = r .. MACRO("PID", hex16(dev.pid))
    r = r .. MACRO("STRING_COUNT", #dev.strings)
    r = r .. "// Endpoint usage:\n"
    r = r .. [[
#ifdef ]]..dev.prefix..[[BTABLE_ADDRESS
#undef ]]..dev.prefix..[[BTABLE_ADDRESS
#endif
]]
    r = r ..  MACRO("BTABLE_ADDRESS", "(0)")
    r = r .. MACRO("MAX_EP", epInfo.maxEp)
    r = r .. MACRO("EP_NUM", "("..dev.prefix.."MAX_EP + 1)")
    
    for k,v in pairs(macros) do
        r = r .. MACRO(k,v)
    end
    r = r .. "\n// PMA buffer reserved for buffer description table  \n"
    r = r .. MACRO("USB_BUF_START", "("..dev.prefix.."EP_BUF_DESC_TABLE_SIZE * "..dev.prefix.."EP_NUM)")
    local start = (epInfo.maxEp+1) * 8
    local mem_usage = ""
    for i=1,epInfo.maxEp+1 do
        mem_usage = mem_usage .. "EP"..(i-1) .. " PMA used:  8  Total: " .. (i*8) .. "\n"
    end
    local addr = 0
    local epSizeTable = {}
    epSizeTable.inMax = 0
    epSizeTable.outMax = 0
    local hasDoubleBuffer = false
    local hasISO = false
    local totolInSize = 0
    for i=0,epInfo.maxEp do
        local ep = epInfo.usage[i]
        if ep then
            r = r .. "\n// EndPoints " .. i .. " defines\n"
            hasDoubleBuffer = hasDoubleBuffer or ep.doubleBuffer
            hasISO = hasISO or (ep.type == ISO)
            epSizeTable[i] = {
                inSize = ep.inSize or 0,
                outSize = ep.outSize or 0,
            }
            epSizeTable.inMax = epSizeTable.inMax < epSizeTable[i].inSize and epSizeTable[i].inSize or epSizeTable.inMax
            epSizeTable.outMax = epSizeTable.outMax < epSizeTable[i].outSize and epSizeTable[i].outSize or epSizeTable.outMax
            if ep.doubleBuffer or (ep.type == ISO) then
                if ep.outSize then
                    r = r .. MACRO('EP' .. i .. '_RX_SIZE', ep.outSize)
                    r = r .. MACRO('EP' .. i .. '_RX0_ADDR', '('..dev.prefix..'USB_BUF_START + ' .. addr ..')')
                    addr = addr + ep.outSize
                    mem_usage = mem_usage .. "EP"..i.." RX0" .. " used: "..ep.outSize .. "  Total: " .. (start+addr) .. "\n"
                    r = r .. MACRO('EP' .. i .. '_RX1_ADDR', '('..dev.prefix..'USB_BUF_START + ' .. addr ..')')
                    addr = addr + ep.outSize
                    mem_usage = mem_usage .. "EP"..i.." RX1" .. " used: "..ep.outSize .. "  Total: " .. (start+addr) .. "\n"
                end
                if ep.inSize then
                    r = r .. MACRO('EP' .. i .. '_TX_SIZE', ep.inSize)
                    r = r .. MACRO('EP' .. i .. '_TX0_ADDR', '('..dev.prefix..'USB_BUF_START + ' .. addr ..')')
                    addr = addr + ep.inSize
                    mem_usage = mem_usage .. "EP"..i.." TX0" .. " used: "..ep.inSize .. "  Total: " .. (start+addr) .. "\n"
                    r = r .. MACRO('EP' .. i .. '_TX1_ADDR', '('..dev.prefix..'USB_BUF_START + ' .. addr ..')')
                    addr = addr + ep.inSize
                    mem_usage = mem_usage .. "EP"..i.." TX1" .. " used: "..ep.inSize .. "  Total: " .. (start+addr) .. "\n"
                    totolInSize = totolInSize + ep.inSize
                end
            else
                if ep.outSize then
                    r = r .. MACRO('EP' .. i .. '_RX_SIZE', ep.outSize)
                    r = r .. MACRO('EP' .. i .. '_RX_ADDR', '('..dev.prefix..'USB_BUF_START + ' .. addr ..')')
                    addr = addr + ep.outSize
                    mem_usage = mem_usage .. "EP"..i.." RX " .. " used: "..ep.outSize .. "  Total: " .. (start+addr) .. "\n"
                end
                if ep.inSize then
                    r = r .. MACRO('EP' .. i .. '_TX_SIZE', ep.inSize)
                    r = r .. MACRO('EP' .. i .. '_TX_ADDR', '('..dev.prefix..'USB_BUF_START + ' .. addr ..')')
                    addr = addr + ep.inSize
                    mem_usage = mem_usage .. "EP"..i.." TX " .. " used: "..ep.inSize .. "  Total: " .. (start+addr) .. "\n"
                    totolInSize = totolInSize + ep.inSize
                end
            end
            r = r .. MACRO('EP' .. i .. '_TYPE', ep_type(ep.type))
        end
    end
    if addr + start > epInfo.maxMem then
        error(dev.prefix..": EndPoint buffer too large, require " .. (addr+start).. ", Only provide " .. epInfo.maxMem .. "\nMem Usage:\n".. mem_usage)
    end
    
    local 
    r = r .. "\n\n// Endpoint define for OTG core\n"
    r = r .. MACRO("OTG_MAX_OUT_SIZE", epInfo.maxOutSize)
    r = r .. MACRO("OTG_CONTROL_EP_NUM", epInfo.controlEpCount)
    r = r .. MACRO("OTG_OUT_EP_NUM", epInfo.outEpCount)
    local fifoT = {
        outMax = dev.prefix .. "OTG_MAX_OUT_SIZE",
        outCnt = dev.prefix .. "OTG_OUT_EP_NUM",
        ctrlCnt = dev.prefix .. "OTG_CONTROL_EP_NUM",
    }
    local rxFifoSize = math.floor(  (epInfo.controlEpCount * 5 + 8) + 
                                    (epInfo.maxOutSize / 4 + 1 )    +
                                    (epInfo.outEpCount * 2) + 1       )
    r = r .. "// RX FIFO size / 4 > (CONTROL_EP_NUM * 5 + 8) +  (MAX_OUT_SIZE / 4 + 1) + (OUT_EP_NUM*2) + 1 = "..rxFifoSize.."\n"
    rxFifoSize = rxFifoSize * 4
    if rxFifoSize < 256 then rxFifoSize = 256 end
    r = r .. MACRO("OTG_RX_FIFO_SIZE",  rxFifoSize)
    r = r .. MACRO("OTG_RX_FIFO_ADDR", 0 )
    local fifoUsed = rxFifoSize 
    local fifoRemain = epInfo.maxMem - fifoUsed
    r = r .. "// Sum of IN ep max packet size is ".. totolInSize .. "\n"
    r = r .. "// Remain Fifo size is ".. math.floor(fifoRemain) .. " in bytes, Rx Used "..fifoUsed.." bytes \n"
    fifoRatio = math.floor(fifoRemain/totolInSize)
    if fifoRemain < totolInSize then
        warning("Remain FIFO("..fifoRemain..") is smaller than EP packet size("..totolInSize..")")
        fifoRatio = 1
    end
    -- I don't know why the max count of fifo should <= 7 * EpMaxPkt
    if fifoRatio > 7 then fifoRatio = 7 end
    for i=0,epInfo.maxEp do
        local ep = epInfo.usage[i]
        if ep and ep.inSize then
            r = r .. MACRO("EP" .. i .. "_TX_FIFO_ADDR", fifoUsed)
            r = r .. MACRO("EP" .. i .. "_TX_FIFO_SIZE", "(" .. dev.prefix .. "EP"..i.."_TX_SIZE * " ..fifoRatio.. ")")
            fifoUsed = fifoUsed + fifoRatio * ep.inSize
        end
    end
    
    local txMax, rxMax = "",""
    for i = 0, #epSizeTable do
        local v = epSizeTable[i]
        local tx = dev.prefix .. "EP"..i.."_TX_SIZE"
        local rx = dev.prefix .. "EP"..i.."_RX_SIZE"
        txMax = txMax .. (v.inSize > 0 and tx or "0") .. ", "
        rxMax = rxMax .. (v.outSize > 0 and rx or "0") .. ", "
    end
    
    epSizeTable.inMax = epSizeTable.inMax>255 and "uint16_t " or "uint8_t "
    epSizeTable.outMax = epSizeTable.outMax>255 and "uint16_t " or "uint8_t "
    
    r = r .. "\n// EndPoint max packed sizes\n"
    r = r .. "extern const "..epSizeTable.inMax..dev.prefix .. "txEpMaxSize[];\n"
    r = r .. MACRO('TXEP_MAX_SIZE', "\\")
    r = r .. "const "..epSizeTable.inMax..dev.prefix .. "txEpMaxSize[] = \\\n"
    r = r .. "{" .. txMax .. "};\n"
    
    r = r .. "extern const "..epSizeTable.outMax..dev.prefix .. "rxEpMaxSize[];\n"
    r = r .. MACRO('RXEP_MAX_SIZE', "\\")
    r = r .. "const "..epSizeTable.outMax.. dev.prefix .. "rxEpMaxSize[] = \\\n"
    r = r .. "{" .. rxMax .. "};\n"
    
    
    r = r .. "\n// EndPoints init function for USB FS core\n"
    r = r .. "#define "..dev.prefix.."TUSB_INIT_EP_FS(dev) \\\n"
    r = r .. "  do{\\\n"
    for i=0,epInfo.maxEp do
        local ep = epInfo.usage[i]
        if ep then
        local t = {n=i, EPn = dev.prefix .."EP"..i}
        r = r .. gsub("    /* Init ep$n */ \\\n", t)
        if ep.inSize and ep.outSize then
            r = r .. gsub("    INIT_EP_BiDirection(dev, PCD_ENDP$n, $EPn_TYPE);  \\\n", t)
            r = r .. gsub("    SET_TX_ADDR(dev, PCD_ENDP$n, $EPn_TX_ADDR);  \\\n", t)
            r = r .. gsub("    SET_RX_ADDR(dev, PCD_ENDP$n, $EPn_RX_ADDR);  \\\n", t)
            r = r .. gsub("    SET_RX_CNT(dev, PCD_ENDP$n, $EPn_RX_SIZE);  \\\n", t)
        elseif ep.inSize then
            if ep.doubleBuffer or ep.type == ISO then
            r = r .. gsub("    INIT_EP_TxDouble(dev, PCD_ENDP$n, $EPn_TYPE);     \\\n", t)
            r = r .. gsub("    SET_DOUBLE_ADDR(dev, PCD_ENDP$n, $EPn_TX0_ADDR, $EPn_TX1_ADDR);  \\\n", t)
            r = r .. gsub("    SET_DBL_TX_CNT(dev, PCD_ENDP$n, 0);     \\\n", t)
            else
            r = r .. gsub("    INIT_EP_TxOnly(dev, PCD_ENDP$n, $EPn_TYPE);       \\\n", t)
            r = r .. gsub("    SET_TX_ADDR(dev, PCD_ENDP$n, $EPn_TX_ADDR);  \\\n", t)
            end
        elseif ep.outSize then
            if ep.doubleBuffer or ep.type == ISO  then
            r = r .. gsub("    INIT_EP_RxDouble(dev, PCD_ENDP$n, $EPn_TYPE);     \\\n", t)
            r = r .. gsub("    SET_DOUBLE_ADDR(dev, PCD_ENDP$n, $EPn_RX0_ADDR, $EPn_RX1_ADDR);     \\\n", t)
            r = r .. gsub("    SET_DBL_RX_CNT(dev, PCD_ENDP$n, $EPn_RX_SIZE);     \\\n", t)
            else
            r = r .. gsub("    INIT_EP_RxOnly(dev, PCD_ENDP$n, $EPn_TYPE);       \\\n", t)
            r = r .. gsub("    SET_RX_ADDR(dev, PCD_ENDP$n, $EPn_RX_ADDR);  \\\n", t)
            r = r .. gsub("    SET_RX_CNT(dev, PCD_ENDP$n, $EPn_RX_SIZE);  \\\n", t)
            end
        end
        end
    end
    r = r .. "  }while(0)\n\n"
    
    
    r = r .. "\n// EndPoints init function for USB OTG core\n"
    r = r .. "#define "..dev.prefix.."TUSB_INIT_EP_OTG(dev) \\\n"
    r = r .. "  do{\\\n"
    r = r .. "    SET_RX_FIFO(dev, "..dev.prefix.."OTG_RX_FIFO_ADDR, "..dev.prefix.."OTG_RX_FIFO_SIZE);  \\\n"
    for i=0,epInfo.maxEp do
        local ep = epInfo.usage[i]
        if ep then
        local t = {n=i, EPn = dev.prefix .."EP"..i}
        r = r .. gsub("    /* Init ep$n */ \\\n", t)
        if ep.inSize then
            r = r .. gsub("    INIT_EP_Tx(dev, PCD_ENDP$n, $EPn_TYPE, $EPn_TX_SIZE);  \\\n", t)
            r = r .. gsub("    SET_TX_FIFO(dev, PCD_ENDP$n, $EPn_TX_FIFO_ADDR, $EPn_TX_FIFO_SIZE);  \\\n", t)
        end
        if ep.outSize then
            r = r .. gsub("    INIT_EP_Rx(dev, PCD_ENDP$n, $EPn_TYPE, $EPn_RX_SIZE);  \\\n", t)
        end
        end
    end
    r = r .. "  }while(0)\n\n"
    
    
    r = r .. "\n"
    r = r .. "#if defined(USB)\n"
    r = r .. "#define " .. dev.prefix.."TUSB_INIT_EP(dev) " .. dev.prefix.."TUSB_INIT_EP_FS(dev)\n"
    r = r .. "\n// Teeny USB device init function for FS core\n"
    r = r .. "#define "..dev.prefix.."TUSB_INIT_DEVICE(dev) \\\n"
    r = r .. "  do{\\\n"
    r = r .. "    /* Init device features */       \\\n"
    r = r .. "    memset(&dev->addr, 0, TUSB_DEVICE_SIZE);    \\\n" 
    r = r .. "    dev->status = "..dev.prefix.."DEV_STATUS;         \\\n"
    r = r .. "    dev->rx_max_size = "..dev.prefix.."rxEpMaxSize;         \\\n"
    r = r .. "    dev->tx_max_size = "..dev.prefix.."txEpMaxSize;         \\\n"
    r = r .. "    dev->descriptors = &"..dev.prefix.."descriptors;         \\\n"
    r = r .. "  }while(0)\n\n"
    
    
    r = r .. "#endif\n\n"
    r = r .. "#if defined(USB_OTG_FS) || defined(USB_OTG_HS)\n"
    r = r .. "#define " .. dev.prefix.."TUSB_INIT_EP(dev) " .. dev.prefix.."TUSB_INIT_EP_OTG(dev)\n"
    r = r .. "\n// Teeny USB device init function for OTG core\n"
    r = r .. "#define "..dev.prefix.."TUSB_INIT_DEVICE(dev) \\\n"
    r = r .. "  do{\\\n"
    r = r .. "    /* Init device features */       \\\n"
    r = r .. "    memset(&dev->addr, 0, TUSB_DEVICE_SIZE);    \\\n" 
    r = r .. "    dev->status = "..dev.prefix.."DEV_STATUS;         \\\n"
    r = r .. "    dev->descriptors = &"..dev.prefix.."descriptors;         \\\n"
    r = r .. "  }while(0)\n\n"
    r = r .. "#endif\n\n"
    
    
    r = r .. "#define "..dev.prefix.."TUSB_INIT(dev) \\\n"
    r = r .. "  do{\\\n"
    r = r .. "    "..dev.prefix.."TUSB_INIT_EP(dev);   \\\n"
    r = r .. "    "..dev.prefix.."TUSB_INIT_DEVICE(dev);   \\\n"
    r = r .. "  }while(0)\n\n"
    
    r = r .. "// Get End Point count\n"
    if dev.prefix ~= "" then
    r = r .. [[
#ifndef  EP_NUM
#define  EP_NUM 1
#endif
#if ]]..dev.prefix..[[EP_NUM > EP_NUM
#undef   EP_NUM
#define  EP_NUM  ]]..dev.prefix..[[EP_NUM
#endif


]]
    end
    if hasDoubleBuffer then
    r = r .. "// Enable double buffer related code\n"
    r = r .. "#define  HAS_DOUBLE_BUFFER\n\n\n"
    end
    if hasISO then
    r = r .. "// Enable iso ep related code\n"
    r = r .. "#define  HAS_ISO_EP\n\n\n"
    end
    if dev.hasWCID then
    r = r .. "// Enable WCID related code\n"
    r = r .. "#define  HAS_WCID\n\n\n"
    r = r .. "#ifndef WCID_VENDOR_CODE\n"
    r = r .. "#define  WCID_VENDOR_CODE       0x17\n"
    r = r .. "extern const uint8_t WCID_StringDescriptor_MSOS[];\n"
    r = r .. "#endif\n\n"
    r = r .. "// Descriptor declare\n"
    end
    r = r .. (dev.declare or "") .. "\n\n"
    return r
end

function EndPointInfo(deviceDescriptor, maxEp, maxMem)
    maxEp = maxEp or 7
    maxMem = maxMem or 1024
    local dev = deviceDescriptor
    local r = {}
    r.maxEp = 0
    r.usage = {}
    r.maxMem = maxMem
    r.maxOutSize = 0
    r.controlEpCount = 1
    r.outEpCount = 0
    local ep = {}
    ep.addr = 0
    ep.inSize = dev.bMaxPacketSize
    ep.outSize = dev.bMaxPacketSize
    ep.type = Control
    r.usage[0] = ep
    local function warning(x)
        if _G.logW then
            _G.logW(deviceDescriptor.prefix .. ": " .. x)
        else
            print(deviceDescriptor.prefix .. ": " .. x)
        end
    end
    function collectEp(v)
        for i,maybeEp in ipairs(v) do
            if type(maybeEp) == 'table' and maybeEp.bDescriptorType == ENDPOINT_DESCRIPTOR_TYPE then
                local ep = {}
                local addr = maybeEp.bEndpointAddress & 0x7f
                if addr > maxEp then
                    warning("End Point Address " .. addr .. " is large than max ep number " .. maxEp .. "   ignore it")
                else
                    local ep = r.usage[addr] or {}
                    ep.addr = addr
                    if (maybeEp.bEndpointAddress & 0x80) == 0x80 then
                        if ep.inSize then
                            warning("EndPoint " .. addr .. " input size already set, the last one will take effect")
                        end
                        ep.inSize = maybeEp.wMaxPacketSize & 0x7ff
                    else
                        if ep.outSize then
                            warning("EndPoint " .. addr .. " outut size already set, the last one will take effect")
                        end
                        ep.outSize = maybeEp.wMaxPacketSize & 0x7ff
                        if r.maxOutSize < ep.outSize then r.maxOutSize = ep.outSize end
                        r.outEpCount = r.outEpCount + 1
                        if maybeEp.bmAttributes == Control then
                            r.controlEpCount = r.controlEpCount + 1
                        end
                    end
                    if ep.type and ep.type ~= maybeEp.bmAttributes then
                        warning("Change EndPoint " .. addr .. " attr, the last one will take effect")
                    end
                    if r.maxEp < addr then r.maxEp = addr end
                    ep.type = maybeEp.bmAttributes
                    ep.doubleBuffer = maybeEp.doubleBuffer
                    if ep.inSize and ep.outSize then
                        if ep.doubleBuffer then warning("Double buffer will disabled in BiDirection ep" .. i) end
                        ep.doubleBuffer = false
                    end
                    if ep.type == ISO then
                        if ep.inSize and ep.outSize then
                            warning("ISO type in BiDirection ep" .. i)
                        end
                    end
                    
                    r.usage[addr] = ep
                end
            elseif type(maybeEp) == 'table' then
                collectEp(maybeEp)
            end
        end
    end
    collectEp(dev)
    r.dev = dev
    r.getInitCode = getInitCode
    return r
end