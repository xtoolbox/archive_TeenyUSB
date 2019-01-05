-- usb_cdc_acm.lua
--[[ 
 Author:  admin@xtoolbox.org
 USB CDC ACM descriptor class
--]]
require("usb_common_desc")

local TAG = "CDC ACM: "
-- CDC is an IAD descriptor, combined control interface and data interface
function CDC_ACM(param)
    local Ctrl, Data
    local CtrlEp = param.CtrlEp or param[1]
    local ReadEp = param.ReadEp or param[2]
    local WriteEp = param.WriteEp or param[3]
    assert(CtrlEp, TAG .. "Control endpoint not set")
    assert(ReadEp, TAG .. "Data read endpoint not set")
    assert(ReadEp, TAG .. "Data write endpoint not set")
    assert(CtrlEp.bmAttributes == Interrupt, TAG .. "Control endpoint type must be interrupt")
    assert((CtrlEp.bEndpointAddress & 0x80) == 0x80, TAG .. "Control endpoint direction must be in")
    assert(ReadEp.bmAttributes == Bulk, TAG .. "Data read endpoint type must be Bulk")
    assert((ReadEp.bEndpointAddress & 0x80) == 0x80, TAG .. "Data read endpoint direction must be in")
    assert(WriteEp.bmAttributes == Bulk, TAG .. "Data write endpoint type must be Bulk")
    assert((WriteEp.bEndpointAddress & 0x80) == 0x00, TAG .. "Data write endpoint direction must be out")
    Ctrl = Interface{
        bInterfaceClass = 2,
        bInterfaceSubClass = 2,
        bInterfaceProtocol = 1,
        Function{   bDescriptorSubtype = 0,
                    alias = "Header",
                    varData = {
                        {bcdCDC = 0x110},
                    }
                },
        Function{   bDescriptorSubtype = 1,
                    alias = "Call Management",
                    varData = {
                        {bmCapabilities = 0x00},
                        {bDataInterface = 0x01},
                    }
                },
        Function{   bDescriptorSubtype = 2,
                    alias = "ACM (Abstract Control Management)",
                    varData = {
                        {bmCapabilities = 0x02},
                    }
                },
        Function{   bDescriptorSubtype = 6,
                    alias = "Union",
                    varData = {
                        {bMasterInterface = 0x00},
                        {bSlaveInterface0 = 0x01},
                    }
                },
        param.CtrlEp or param[1],
    }
    
    Data = Interface{
        bInterfaceClass = 0x0a,
        bInterfaceSubClass = 0,
        bInterfaceProtocol = 0,
        param.ReadEp or param[2],
        param.WriteEp or param[3],
    }
    return IAD{Ctrl, Data}
end


