-- language.lua
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

local chs = {
["&USB Descriptor"] =  "USB描述符(&U)", 
["&New Device"] =  "新建设备(&N)",
["Preview &Descriptor"] =  "预览描述符(&D)",
["Preview &EP Define"] =  "预览端点定义(&E)",
["&Generate Code"]     =  "生成代码(&G)",
["Generate Code &All"] =  "生成全部代码(&A)",
["USB &Driver"] =  "USB驱动(&D)",
["Priview &Inf"] =  "预览INF文件(&I)",
["Priview &Inf All"] =  "预览全部INF文件(&)",
["Generate &Inf"] =  "生成INF文件(&I)",
["Generate Inf &All"] =  "生成全部INF文件(&I)",
["&Window"] =  "窗口(&W)",
["&Cascade"] =  "层叠(&C)",
["&Tile"] =  "平铺(&T)",
["Clear &Log"] =  "清除日志(&L)",
["Manufacture string or leave empty"] =  "制造商名称 或 留空",
["Product string or leave empty"] =  "产品名称 或 留空",
["Serial string or leave empty"] =  "序列号 或 留空",
["Self Power"] =  "自供电",
["Remote Wakeup"] =  "远程唤醒",
["Max EP No."] =  "最大端点号",
["USB Memory size"] =  "USB模块内存大小",
["Vendor Str"] =  "制造商名",
["Product Str"] =  "产品名",
["Serial Str"] =  "序列号",
["Max Power"] =  "最大电流",
["Add Interface"] =  "添加设备接口",
["CDC"] =  "CDC串口",
["HID"] =  "HID输入设备",
["Generic"] =  "自定义通用设备",
["Set Interface Name or leave empty"] = "接口名称 或 留空",
["Remove"] =  "删除接口",
["Control"] =  "控制端点",
["Max Pkt"] =  "最大包长",
["Interval"] =  "间隔",
["Read"] =  "读端点",
["Write"] =  "写端点",
["Edit..."] =  "编辑...",
["    Report desc"] =  "    报告描述符",
["Add EndPoint"] =  "添加端点",
["WCID WinUSB"] =  "WinUSB免驱",
["PktSize"] = "端点0最大包长",
["What is WCID ?"] = "什么是WCID免驱设备?",
["GenEp"] = "通用端点",
["Trans/Frame"] = "次每帧",
["HID Report Tool"] = "报告描述符工具",
__name__ = "CHS",
__context__ = {},
}

local curLang = chs

local logI = function(...)
    local r = ""
    for i,v in pairs({...}) do
        r = r .. tostring(v) .. " "
    end
    logEdit:append(r)
end


local s1,s2 = QDate.currentDate():toString("dddd"):find("星期")
if s1 then
    curLang = chs
else
    curLang = nil
end

_G.tr = function(text, context)
    local trans = curLang
    if not trans then return text end
    if trans and context and trans.__context__[context] then
        trans = trans.__context__[context]
    end
    if trans[text] then return trans[text] end
    trans[text] = text
    logI('["'..text..'"] = ', "not translate in language", curLang.__name__,"with context: ", context)
    return text
end