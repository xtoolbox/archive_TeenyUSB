# Demo说明

## USB设备例程 Demo for device

| Demo Folder      |      Demo Type       |  Tested Board  |
|------------------|----------------------|----------------|
| custom_bulk      | 自定义 Bulk 设备     | All Below      |
| cdc              | 虚拟串口 CDC         | All Below      |
| msc              | 虚拟U盘 Mass Storage | All Below      |
| custom_hid       | 自定义 HID 设备      | All Below      |
| cdc5             | 5路 CDC              | All Below      |
|CubeMxF107        | CubeMX+TeenyUSB 自定义Bulk设备| F107 Demo Board |
|CubeMXF303        | CubeMX+TeenyUSB 自定义Bulk设备|[STM32F3 Disco][303]|
|CubeMXF723        | CubeMX+TeenyUSB 自定义Bulk设备|[stm32f723e_disco][723]|

## USB主机例程 Demo for host

| Demo Folder      |      Demo Type       |  Tested Board  |
|------------------|----------------------|----------------|
| host_raw         | 无协议栈主机 Raw Host| F723/F767       |
| host_input       | HUB+KeyBoard+Mouse   | F723/F767       |


## Demo测试用的开发板 Demo tested on boards

开发板相关的代码在boards目录下，目前支持的开发板如下：

Below is supported develop boards:

| Board Folder     |      Board Type      |      Chip     |HSE Freq | Tested USB Core |
|------------------|----------------------|---------------|---------|-----------------|
| stm32f0          | Custom board         | STM32F072C8T6 | No HSE  | USB FS          |
| stm32f1          | Custom board         | STM32F103RET6 | 8 MHz   | USB FS          |
| stm32f407_evk    | [Waveshare EVK407I][407]    | STM32F407IGT6 | 8 MHz   | OTG_FS/OTG_HS_ULPI  |
| stm32f723e_disco | [stm32f723e discovery][723] | STM32F723IEK6 | 25 MHz  | OTG_FS/OTG_HS_Embed |
| stm32767zi_nucleo| [stm32f767zi nucleo][767]   | STM32F767ZIT6 | 8 MHz   | OTG_FS              |

[767]: https://www.st.com/en/evaluation-tools/nucleo-f767zi.html
[723]: https://www.st.com/en/evaluation-tools/32f723ediscovery.html
[407]: http://www.waveshare.net/wiki/EVK407I
[303]: https://www.st.com/en/evaluation-tools/stm32f3discovery.html
