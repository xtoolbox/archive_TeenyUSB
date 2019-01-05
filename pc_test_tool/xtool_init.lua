function tr(x) return x end
function log(...)
    local r = ""
    for i,v in ipairs({...}) do
        r = r .. tostring(v) .. " "
    end
end
require("hidview")
require("usbview")
local subWindow = mdiArea:addSubWindow(USBView(0x0483))
subWindow:show()
subWindow.w = 600
subWindow.h = 500

subWindow = mdiArea:addSubWindow(HIDView(0x0483))
subWindow:show()
subWindow.x = 600 + 20
subWindow.w = 600
subWindow.h = 500

