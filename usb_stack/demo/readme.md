# Demo目录结构

Demo内容见下表:

| Demo Folder      |      Demo Type       |Supported Board |
|------------------|----------------------|----------------|
| custom_bulk      | 自定义 Bulk 设备     | All            |
| cdc              | 虚拟串口 CDC         | All            |
| msc              | 虚拟U盘 Mass Storage | All            |
| custom_hid       | 自定义 HID 设备      | All            |
| cdc5             | 5路 CDC              | All            |

## 支持的开发板 Supported board

开发板相关的代码在boards目录下，目前支持的开发板如下：

Below is supported develop boards:

| Board Folder     |      Board Type      |      Chip     |HSE Freq | USB Core      |
|------------------|----------------------|---------------|---------|---------------|
| stm32f0          | Custom board         | STM32F072C8T6 | No HSE  | USB FS        |
| stm32f1          | Custom board         | STM32F103RET6 | 8MHz    | USB FS        |
| stm32f723e_disco | [stm32f723e discovery][723] | STM32F723IEK6 | 25MHz   | OTG_FS/OTG_HS |
| stm32767zi_nucleo| [stm32f767zi nucleo][767]   | STM32F767ZIT6 | 8HMz    | OTG_FS        |

[767]: https://www.st.com/en/evaluation-tools/nucleo-f767zi.html
[723]: https://www.st.com/en/evaluation-tools/32f723ediscovery.html
