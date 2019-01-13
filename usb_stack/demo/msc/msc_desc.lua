return Device {
    strManufacture = "TeenyUSB",
    strProduct = "TeenyUSB MSC",
    strSerial = "TUSB123456",
    idVendor = 0x0483,
    idProduct = 0x0003,
    prefix = "MSC",
    Config {
        Interface{
            bInterfaceClass = 0x08,        -- MSC
            bInterfaceSubClass = 0x06,     -- SCSI
            bInterfaceProtocol = 0x50,     -- BOT
            EndPoint(IN(1),  BulkDouble, 64),
            EndPoint(OUT(1), BulkDouble, 64),
        },
   }
}