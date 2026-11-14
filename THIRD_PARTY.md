# 第三方组件与许可

本仓库根目录的 `LICENSE`（MIT）**只覆盖作者原创部分**。下列组件版权归各自所有者，
按其原始许可分发，原始许可全文随仓库一同保留。

| 路径 | 组件 | 版权 | 许可 | 许可全文 |
|---|---|---|---|---|
| `Drivers/STM32F4xx_HAL_Driver/` | STM32F4 HAL 驱动 | STMicroelectronics | BSD-3-Clause | `Drivers/STM32F4xx_HAL_Driver/LICENSE.txt` |
| `Drivers/CMSIS/` | ARM CMSIS-Core 与器件头文件 | ARM / ST | Apache-2.0 | `Drivers/CMSIS/LICENSE.txt` |
| `Drivers/CMSIS/Device/ST/STM32F4xx/` | STM32F4 器件支持 | STMicroelectronics | Apache-2.0 | 同目录 `LICENSE.txt` |
| `Middlewares/Third_Party/ARM/DSP/` | ARM CMSIS-DSP | ARM | Apache-2.0 | `Middlewares/Third_Party/ARM/DSP/LICENSE.txt` |
| `Core/Src/system_stm32f4xx.c` | 系统时钟树（官方模板） | STMicroelectronics | BSD-3-Clause | 同上 |

## 作者原创部分

以下文件为本项目原创，适用根目录 MIT 许可：

- `Core/Src/main.c`、`signal_app.c`、`signal_acquisition.c`、`FFT.c`、`dds_pll.c`、
  `serial_log.c`、`tft_display.c`、`vision_slave.c`、`ad9834_gpio_f407.c`
- `Core/Inc/` 下对应的同名头文件与 `app_config.h`
- `K230/main.py`
- `Core/Src/adc.c`、`dma.c`、`tim.c`、`usart.c`、`gpio.c`、`stm32f4xx_it.c`、
  `stm32f4xx_hal_msp.c` 中的 `USER CODE` 区块
- 根目录 `README.md`、本文件、`CONTRIBUTING.md`

## 说明

- CubeMX 生成的骨架代码（`.ioc` 对应的初始化函数主体）属 ST 模板，正文版权归 ST；
  本项目对其的修改集中在 `USER CODE BEGIN/END` 区块内，这些修改部分适用 MIT。
- 若需将本项目用于商业产品，请自行复核上述第三方许可的合规要求
  （Apache-2.0 需保留 NOTICE 与许可声明，BSD-3-Clause 需保留版权声明与免责条款）。
