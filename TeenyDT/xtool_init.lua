-- xtool_init.lua
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

-- The TeenyDT GUI tool initial script
require("usb_hid")
require("usb_cdc_acm")
require("gen_descriptor")
require("driver")
local deviceList = require("device_list")

local rl, e = pcall(function()
    require("language")
end)
if not rl then logEdit:append(e) end

local tr = _G.tr or function(s, ctx) return s end

class "EPView" (QFrame)
function EPView:__init(name, optional)
    QFrame.__init(self)
    name = name or ""
    local w = 40
    self.packetSize = QLineEdit("64"){maxW = w, minW = w}
    self.transferPerFrame = QComboBox{
        { "1","2","3"}
    }
    self.attrMap = {
        "Interrupt",
        "Control",
        "Bulk",
        "BulkDouble",
        "ISO",
    }
    self.optional = optional
    self.valid = true
    self.attr = QComboBox{ self.attrMap }
    self.dir = QComboBox{{
        "IN",
        "OUT",
    }}
    self.holder = QLabel(""){maxW = 30, minW = 30}
    if optional then
        self.holder.maxW = 1
        self.holder.minW = 1
        self.name = QCheckBox(name) {maxW = 89, minW = 89}
        self.name.checked = true
    else
        self.name = QLabel(name) {maxW = 60, minW = 60}
    end
    self.attr.currentIndex = 0
    self.addr = QLineEdit("1"){maxW = w, minW = w}
    self.interval = QLineEdit("1") {maxW = w, minW = w}
    self.name.toggled = function()
        local e = self.name.checked
        self.addr.enabled = e
        self.dir.enabled = e
        self.attr.enabled = e
        self.packetSize.enabled = e
        self.interval.enabled = e
        self.valid = e
    end
    self.layout = QHBoxLayout {
            self.holder,
            self.name,
            --QLabel(tr("Addr","EP")), 
            self.addr, self.dir,
            --QLabel(tr("Type","EP")),
            self.attr,
            QLabel(tr("Max Pkt","EP")), self.packetSize,
            self.transferPerFrame,QLabel(tr("Trans/Frame","EP")),
            QLabel(tr("Interval", "EP")), self.interval
    }
    self.layout.contentsMargins = QMargins(0,0,0,0)
end
function EPView:setEp(addr, attr, pktSize, interval)
    pktSize = pktSize or 64
    interval = interval or 1
    self.addr.text = tostring(addr&0x7f)
    if (addr & 0x80) == 0 then
        self.dir.currentIndex = 1
    else
        self.dir.currentIndex = 0
    end
    for i,v in ipairs(self.attrMap) do
        if attr == v then self.attr.currentIndex = i - 1 end
    end
    self.packetSize.text = tostring(pktSize)
    self.interval.text = tostring(interval)
end
function EPView:getAddr()
    return (tonumber(self.addr.text) & 0x7f) + (self.dir.currentIndex == 1 and 0 or 0x80)
end
function EPView:makeDesc()
    if not self.valid then return nil end
    
    local transPerFrame = 0
    if self.attr.currentText == "Interrupt" or self.attr.currentText == "ISO" then
        transPerFrame = self.transferPerFrame.currentIndex
        if transPerFrame < 0 then transPerFrame = 0 end
        if transPerFrame > 2 then transPerFrame = 2 end
    end
    transPerFrame = tonumber(transPerFrame) << 11
    return EndPoint{
        bEndpointAddress = self:getAddr(),
        bmAttributes = _G[self.attr.currentText],
        wMaxPacketSize = tonumber(self.packetSize.text) + transPerFrame,
        bInterval = tonumber(self.interval.text),
    }
end

class "IfView" (QFrame)
function IfView:__init(p, name)
    QFrame.__init(self,p)
    name = name or "Generic"
    self.ifname = QLabel("")
    self.name = QLineEdit(){ placeHolderText = tr("Set Interface Name or leave empty") }
    self.btnRemove = QPushButton(tr("Remove")) {maxH = 30, minH = 20}
    self.drvCombo = QComboBox{ SupportDriverList() }
    self.layout = QVBoxLayout{
        QHBoxLayout{self.ifname,QLabel(" - " .. name), self.name, self.btnRemove, self.drvCombo},
    }
    self.btnRemove.clicked = function()
        if self.removeEp then self.removeEp() end
        if self.removeCallback then
            self.removeCallback(self)
        end
    end
    self.layout.contentsMargins = QMargins(2,2,2,2)
    self.objectName = "ItfFrame"
    self.styleSheet = [[
    QFrame#ItfFrame{
        border: 1px solid black;
    }
    ]]
end

function IfView:initIf(iInterface, removeCallback)
    self.ifname.text = "Interface " .. (iInterface - 1)
    self.ifIndex = iInterface
    self.removeCallback = removeCallback
end
function IfView:strInterface()
    if self.name.text == "" then return nil end
    return self.name.text
end


class "CDCView"(IfView)
function CDCView:__init(p)
    IfView.__init(self,p, "CDC")
    self.cdcCtrl = EPView(tr("Control","CDC"))
    self.cdcRead = EPView(tr("Read","CDC"))
    self.cdcWrite = EPView(tr("Write","CDC"))
    self.layout:addWidget(self.cdcCtrl)
    self.layout:addWidget(self.cdcRead)
    self.layout:addWidget(self.cdcWrite)
    local e = p.getMinEp()
    self.cdcCtrl:setEp(0x80 + p.getMinEp(), "Interrupt", 8)
    self.cdcRead:setEp(0x80 + e, "Bulk")
    self.cdcWrite:setEp(e, "Bulk")
    self.removeEp = function()
        self.freeEp(self.cdcCtrl:getAddr()&0x7f)
        self.freeEp(self.cdcRead:getAddr()&0x7f)
        self.freeEp(self.cdcWrite:getAddr()&0x7f)
    end
end

function CDCView:makeDesc()
    return CDC_ACM{
        CtrlEp = self.cdcCtrl:makeDesc(),
        ReadEp = self.cdcRead:makeDesc(),
        WriteEp = self.cdcWrite:makeDesc(),
    }
end

class "HIDView"(IfView)
function HIDView:__init(p)
    IfView.__init(self,p,"HID")
    self.epRead = EPView(tr("Read","HID"))
    self.epWrite = EPView(tr("Write","HID"), true)
    self.rptType = QComboBox{{
        "InOut",
        "BootMouse",
        "BootKeyBoard",
        "UserDefine",
    }}
    self.ioSize = QLineEdit("64") {maxW = 40, minW = 40}
    self.btnEditRpt = QPushButton(tr("Edit...", "HID")){enabled = false, maxH = 30}
    self.userRpt = HID_ReportTemp().text
    
    self.rptType.currentIndexChanged = function()
        local idx = self.rptType.currentText
        self.ioSize.enabled = idx == "InOut"
        self.btnEditRpt.enabled = idx == "UserDefine"
    end
    self.rptType.currentIndex = 0
    
    self.btnEditRpt.clicked = function()
        local dlg = QDialog(self)
        local t = QLuaEdit(){minW = 1000, minH = 600}
        local btnOk = QPushButton(tr("OK"))
        btnOk.clicked = function() dlg:accept() end
        t.plainText = self.userRpt
        dlg.layout = QVBoxLayout{t, QHBoxLayout{ btnOk }}
        if dlg:exec() == 1 then self.userRpt = t.plainText end
    end
    
    local e = p.getMinEp()
    self.epRead:setEp(0x80+e, "Interrupt", 64, 1)
    self.epWrite:setEp(e, "Interrupt", 64, 1)
    self.layout:addWidget(self.epRead)
    self.layout:addWidget(self.epWrite)
    self.reportDT = QLabel([[<a href="https://www.usb.org/document-library/hid-descriptor-tool">]]..tr("HID Report Tool")..[[</a>]])
        { openExternalLinks = true }
    
    self.layout:addLayout(QHBoxLayout{
        QLabel(tr("    Report desc")), self.rptType, QLabel("Size:"), self.ioSize, self.btnEditRpt, self.reportDT
    })
    self.removeEp = function()
        self.freeEp(self.epRead:getAddr() & 0x7f)
        self.freeEp(self.epWrite:getAddr() & 0x7f)
    end
end

function HIDView:makeDesc()
    local r = {
        strInterface = self:strInterface(),
        ReadEp = self.epRead:makeDesc(),
        WriteEp = self.epWrite:makeDesc(),
    }
    if self.rptType.currentText == "InOut" then
        r.report = HID_InOut(tonumber(self.ioSize.text) or 0x40)
    elseif self.rptType.currentText == "BootKeyBoard" then
        r.report = HID_BootKeyBoard()
    elseif self.rptType.currentText == "BootMouse" then
        r.report = HID_BootMouse()
    else
        
        r.report = HID_BuildReport(self.userRpt)
    end
    return USB_HID(r)
end

class "GenericView" (IfView)
function GenericView:__init(p)
    IfView.__init(self, p)
    self.btnAdd = QPushButton(tr("Add EndPoint")){maxH = 30, minH = 20}
    self.chkWCIDWinUSB = QCheckBox(tr("WCID WinUSB"))
    self.layout:addLayout(QHBoxLayout{
        self.btnAdd, self.chkWCIDWinUSB, 
        QLabel([[<a href="https://github.com/xtoolbox/TeenyUSB/wiki/WCID-Device">]]..tr("What is WCID ?")..[[</a>]])
        { openExternalLinks = true },
        strech = "0,0,1",
    })
    self.eps = {
        EPView(tr("GenEp")),
    }
    self.epDir = 0x80
    self.eps[1]:setEp(p.getMinEp()+self.epDir, "Bulk", 64, 0)
    self.layout:addWidget(self.eps[1])
    self.btnAdd.clicked = function()
        local epv = EPView(tr("GenEp"), true)
        self.epDir = self.epDir == 0 and 0x80 or 0
        epv:setEp(self.getMinEp()+self.epDir, "Bulk", 64, 0)
        epv.name.toggled = function()
            if not epv.name.checked then
                local idx
                for i, v in ipairs(self.eps) do
                    if v == epv then
                        idx = i
                        break
                    end
                end
                local v = table.remove(self.eps, idx)
                if v then
                    self.freeEp(v:getAddr() & 0x7f)
                    v:close()
                    self.layout:removeWidget(v)
                    self:resize(self.sizeHint)
                    if self.reset_back then self.reset_back() end
                end
            end
        end
        self.layout:addWidget(epv)
        self.eps[#self.eps+1] = epv
        self:resize(self.sizeHint)
        if self.reset_back then self.reset_back() end
    end
    self.removeEp = function()
        for i,v in ipairs(self.eps) do
            self.freeEp(v:getAddr() & 0x7f)
        end
    end
end

function GenericView:makeDesc()
    local param = {
        strInterface = self:strInterface(),
    }
    if self.chkWCIDWinUSB.checked then param.WCID = WinUSB end
    for i,v in ipairs(self.eps) do
        param[#param+1] = v:makeDesc()
    end
    return Interface(param)
end

class "UsbDescView" (QFrame)
function UsbDescView:__init(parent)
    QFrame.__init(self, parent)
    self.ifs = parent.ifs
    self.editVid = QLineEdit("0x2017"){
       placeHolderText = "VID", maxW = 60
    }
    self.editPid = QLineEdit("0x0924"){
       placeHolderText = "PID", maxW = 60
    }
    self.editPktSize = QComboBox{
        {"8","16","32","64"}, maxW = 60
    }
    self.editPktSize.currentIndex = 3
    self.editDeviceName = QLineEdit(""){
        placeHolderText = "Name"
    }
    self.editVendor = QLineEdit("Teeny USB Desc Tool"){
       placeHolderText = tr("Manufacture string or leave empty"),
    }
    self.editProduct = QLineEdit("Teeny USB DEMO"){
       placeHolderText = tr("Product string or leave empty"),
    }
    self.editSerial = QLineEdit("TeenyUSB000001"){
       placeHolderText = tr("Serial string or leave empty"),
    }
    self.chkSelfPower = QCheckBox(tr("Self Power"))
    self.chkRemoteWakeup = QCheckBox(tr("Remote Wakeup"))
    self.maxPower = QLineEdit("100"){inputMask = "999", maxW = 80}
    self.chkSelfPower.toggled = function()
        self.maxPower.enabled = not self.chkSelfPower.checked
    end
    
    self.comboDevices = QComboBox()
    self.devMap = {}
    for i,v in ipairs(deviceList) do
        self.comboDevices:addItem(v.name)
        self.devMap[v.name] = v
    end
    self.comboDevices.currentIndexChanged = function()
        local v = self.devMap[self.comboDevices.currentText]
        if v then
            self.editMaxEP.text = tostring(v.maxEp)
            self.editMaxMem.text = tostring(v.maxMem)
        end
    end
    
    self.editMaxEP = QLineEdit("7") {maxW = 80}
    self.editMaxMem = QLineEdit("1024") {maxW = 80}
    
    self.comboDevices.currentIndexChanged()
    self.layout = QVBoxLayout{
        QHBoxLayout{
            self.editDeviceName,
            QLabel("VID"), self.editVid, QLabel("PID"), self.editPid,
            QLabel(tr("PktSize")), self.editPktSize,
        },
        QHBoxLayout{
            self.comboDevices,
            QLabel(tr("Max EP No.")), self.editMaxEP, 
            QLabel(tr("USB Memory size")), self.editMaxMem
        },
        QFormLayout{
            {tr("Vendor Str"), self.editVendor},
            {tr("Product Str"), self.editProduct},
            {tr("Serial Str"), self.editSerial},
        },
        --QHBoxLayout{QLabel(tr("Vendor Str")), self.editVendor},
        --QHBoxLayout{QLabel(tr("Product Str")), self.editProduct},
        --QHBoxLayout{QLabel(tr("Serial Str")), self.editSerial},
        QHBoxLayout{
            self.chkSelfPower, self.chkRemoteWakeup,
            QLabel(tr("Max Power")), self.maxPower, QLabel("x2mA"),
        },
        QHBoxLayout{
            self.btnViewDesc, self.btnViewEpInit
        },
    }
    self.layout.contentsMargins = QMargins(0,0,0,0)
    

end
function UsbDescView:maxEP()
    return tonumber(self.editMaxEP.text)
end
function UsbDescView:maxMem()
    return tonumber(self.editMaxMem.text)
end

local function str(edit)
        if edit.text == "" then return nil end
        return edit.text
end
function UsbDescView:makeDesc()
    local cfg = {}
    cfg.bMaxPower = tonumber(self.maxPower.text)
    cfg.SelfPower = self.chkSelfPower.checked
    cfg.RemoteWakeup = self.chkRemoteWakeup.checked
    for i,v in ipairs(self.ifs) do
        cfg[#cfg+1] = v:makeDesc()
    end
    local prefix = ""
    if self.editDeviceName.text ~= "" then
        prefix = self.editDeviceName.text
        prefix = string.gsub(prefix, "[ \t]", "_")
    end
    local desc = Device {
        strManufacture = str(self.editVendor),
        strProduct = str(self.editProduct),
        strSerial = str(self.editSerial),
        idVendor = tonumber(self.editVid.text) or 0,
        idProduct = tonumber(self.editPid.text) or 0,
        bMaxPacketSize = tonumber(self.editPktSize.currentText) or 8,
        prefix = prefix:upper(),
        Config (cfg)
    }
    return desc
end

local log = function(...)
    local r = ""
    for i,v in ipairs({...}) do r = r .. tostring(v) .. "  " end
    logEdit:append(r)
end

function logW(...)
    log("WARNING: ", ...)
end
function logI(...)
    log("INFO   : ", ...)
end
function logE(...)
    log("Error  : ", ...)
end

local devCnt = 1

class "UsbDev"(QScrollArea)
function UsbDev:__init()
    QScrollArea.__init(self)
    self.addView = nil
    self.windowTitle = "Device " .. devCnt
    devCnt = devCnt + 1
    self.back = QFrame()
    self.back.ifs = {}
    self.udv = UsbDescView(self.back)
    self.udv.editDeviceName.text = self.windowTitle
    self.udv.editDeviceName.editingFinished = function()
        self.windowTitle = self.udv.editDeviceName.text
    end
    self.btnAddIf = QPushButton(tr("Add Interface"), self.back){w = 160}
    self.btnAddIf.menu = QMenu{
        QAction(tr("CDC")) {
            triggered = function() self.addView(CDCView) end
        },
        QAction(tr("HID")){
            triggered = function() self.addView(HIDView) end
        },
        QAction(tr("Generic")){
            triggered = function() self.addView(GenericView) end
        },
    }

    self.udvSize = self.udv.sizeHint
    self.udv.x = 10
    self.udv.y = 10
    self.udv:resize(self.udvSize)
    self.btnSize = self.btnAddIf.sizeHint
    self.reset_back = function()
        local w = self.udvSize.w + self.udv.x
        local h = self.udvSize.h + self.udv.y
        local margin = 10
        local lMargin = 10
        for i,v in ipairs(self.back.ifs) do
            h = h + margin
            v.x = lMargin
            v.y = h
            h = h + v.sizeHint.h
            if v.sizeHint.w > w then w = v.sizeHint.w end
        end
        h = h + margin + 10
        self.btnAddIf.x = lMargin
        self.btnAddIf.y = h
        h = h + self.btnSize.h
        self.back:resize(  QSize(w+lMargin*2, h+margin*2) )
        --sa:ensureWidgetVisible(btnAddIf)
    end

    self.onRemove = function(view)
        local idx
        for i,v in ipairs(self.back.ifs) do
            if v == view then idx = i end
        end
        local v = table.remove(self.back.ifs, idx)
        if v then
            v:close()
            self.reset_back()
        end
        for i,v in ipairs(self.back.ifs) do
            if v.initIf then v:initIf(i, self.onRemove) end
        end
    end
    
    self.minEp = nil
    function self.freeEp(ep)
        if self.minEp and self.minEp[ep] then self.minEp[ep] = false end
    end
    function self.getMinEp()
        if not self.minEp then
            self.minEp = {}
            for i=1, 64 do self.minEp[i] = false end
        end
        for i=1,#self.minEp do
            if not self.minEp[i] then
                self.minEp[i] = true
                return i
            end
        end
    end
    self.back.getMinEp = self.getMinEp
    self.back.freeEp = self.freeEp

    self.addView = function(view)
        local v = view(self.back)
        v.freeEp = self.freeEp
        v.getMinEp = self.getMinEp
        v.visible = true
        v.reset_back = self.reset_back
        self.back.ifs[#self.back.ifs+1] = v
        if v.initIf then v:initIf(#self.back.ifs, self.onRemove) end
        self.reset_back()
    end
    self.reset_back()
    self.widget = self.back
end

--logEdit:append(""..udv.sizeHint.w ..",".. udv.sizeHint.h)
local function dev_new()
    local dev = UsbDev()
    dev:setProperty("TeenyDevice",dev)
    local tw = mdiArea:addSubWindow(dev)
    tw.w = 650
    tw.h = 500
    tw:show()
end
local function curDev(subWindow)
    subWindow = subWindow or mdiArea.currentSubWindow
    local dev = subWindow.widget:property("TeenyDevice")
    if dev.udv then return dev end
    return nil
end
local function dev_show_desc()
    local dev = curDev()
    if not dev then return end
    local t = QTextEdit()
    t.plainText = tostring(dev.udv:makeDesc())
    t.windowTitle = "Preview " .. dev.windowTitle .. " Descriptor"
    mdiArea:addSubWindow(t):showMaximized()
end
local function dev_show_ep_define()
    local dev = curDev()
    if not dev then return end
    local desc = dev.udv:makeDesc()
    local x = tostring(desc)
    local t = QTextEdit()
    local ep = EndPointInfo(desc, dev.udv:maxEP(), dev.udv:maxMem()):getInitCode()
    t.plainText = ep
    t.windowTitle = "Preview " .. dev.windowTitle .. " EP Define"
    mdiArea:addSubWindow(t):showMaximized()
end

local function gen_code(desc)
    local descs,eps = collectDescriptor(desc)
    local dir = QCommonDlg.getDir()
    if not dir or dir == "" then return end
    local s = "-- Generate from UI Tool --"
    arg = { "<Generate from UI Tool>" }
    outputCode(dir.."/teeny_usb_desc.c", s, descs)
    outputCode(dir.."/teeny_usb_init.h", s, eps)
    log("Code generated in " .. dir)
end

local function dev_gen_code()
    local dev = curDev()
    if not dev then return end
    local desc = dev.udv:makeDesc()
    desc.maxEp, desc.memSize = dev.udv:maxEP(), dev.udv:maxMem()
    gen_code(desc)
end
local function dev_gen_code_all()
    local r = mdiArea:subWindowList()
    local descs = {}
    for i,v in ipairs(r) do
        local dev = curDev(v)
        if dev and dev.udv then
            local desc = dev.udv:makeDesc()
            desc.maxEp, desc.memSize = dev.udv:maxEP(), dev.udv:maxMem()
            descs[#descs+1] = desc
        end
    end
    if #descs > 0 then
        gen_code(descs, dev)
    end
end

local function dev_show_inf()
    local dev = curDev()
    if not dev then return end
    local desc = dev.udv:makeDesc()
    desc.drivers = {}
    for i,v in ipairs(dev.back.ifs) do
        desc.drivers[i] = v.drvCombo.currentText
    end
    local t = QTextEdit()
    t.plainText = DriverCreate(desc.prefix, desc)
    t.windowTitle = "Preview " .. dev.windowTitle .. " inf files"
    mdiArea:addSubWindow(t):showMaximized()
end

local function dev_show_inf_all()
    local r = mdiArea:subWindowList()
    local descs = {}
    local n = ""
    for i,v in ipairs(r) do
        local dev = curDev(v)
        if dev and dev.udv then
            local desc = dev.udv:makeDesc()
            n = desc.prefix
            desc.drivers = {}
            for j,iv in ipairs(dev.back.ifs) do
                desc.drivers[j] = iv.drvCombo.currentText
            end
            descs[#descs+1] = desc
        end
    end
    if #descs > 0 then
        local t = QTextEdit()
        t.plainText = DriverCreate(n, descs)
        t.windowTitle = "Preview all device inf files"
        mdiArea:addSubWindow(t):showMaximized()
    end
end

local function dev_gen_inf()
    local dev = curDev()
    if not dev then return end
    local desc = dev.udv:makeDesc()
    desc.drivers = {}
    for i,v in ipairs(dev.back.ifs) do
        desc.drivers[i] = v.drvCombo.currentText
    end
    DriverCreate(desc.prefix, desc, true)
end

local function dev_gen_inf_all()
    local r = mdiArea:subWindowList()
    local descs = {}
    local n = ""
    for i,v in ipairs(r) do
        local dev = curDev(v)
        if dev and dev.udv then
            local desc = dev.udv:makeDesc()
            n =desc.prefix
            desc.drivers = {}
            for j,iv in ipairs(dev.back.ifs) do
                desc.drivers[j] = iv.drvCombo.currentText
            end
            descs[#descs+1] = desc
        end
    end
    if #descs > 0 then
        DriverCreate(n, descs, true)
    end
end

mainWindow:menuBar(){
    QMenu(tr("&USB Descriptor")){
        QAction(tr("&New Device")){
            triggered = dev_new, QKeySequence("Ctrl+N"),
        },
        QAction(tr("Preview &Descriptor")){
            triggered = dev_show_desc, QKeySequence("Ctrl+D"),
        },
        QAction(tr("Preview &EP Define")){
            triggered = dev_show_ep_define, QKeySequence("Ctrl+E"),
        },
        QAction(tr("&Generate Code")){
            triggered = dev_gen_code, QKeySequence("Ctrl+G"),
        },
        QAction(tr("Generate Code &All")){
            triggered = dev_gen_code_all, QKeySequence("Ctrl+Alt+G"),
        },

    },
    QMenu(tr("USB &Driver")){
        QAction(tr("Priview &Inf")){
            triggered = dev_show_inf, QKeySequence("Ctrl+P"),
        },
        QAction(tr("Priview &Inf All")){
            triggered = dev_show_inf_all, QKeySequence("Ctrl+Alt+P"),
        },
        QAction(tr("Generate &Inf")){
            triggered = dev_gen_inf, QKeySequence("Ctrl+I"),
        },
        QAction(tr("Generate Inf &All")){
            triggered = dev_gen_inf_all, QKeySequence("Ctrl+Alt+I"),
        },
    },
    QMenu(tr("&Window")){
        QAction(tr("&Cascade")){
            triggered = function() mdiArea:cascadeSubWindows() end, QKeySequence("Ctrl+Alt+C"),
        },
        QAction(tr("&Tile")){
            triggered = function() mdiArea:tileSubWindows() end, QKeySequence("Ctrl+Alt+T"),
        },
        QAction(""){ separator = true },
        QAction(tr("Clear &Log")){
            triggered = function() logEdit:clear() end, QKeySequence("Ctrl+Alt+L"),
        },
    }
}

dev_new()