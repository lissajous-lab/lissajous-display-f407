/**
  ******************************************************************************
  * @file    vision_slave.h
  * @brief   Requirement-5 K230 UART slave for the AD9834 output.
  *
  * 视觉模式下本模块接管 DDS：K230 通过 USART3 下发 $SET / $FREQ / $PHASE /
  * $RAMP，本模块执行并把结果回成 $APPLIED 或 $ERROR。协议是 ASCII 行、
  * 带 seq/probe 配对与重发机制，完整说明见 docs/vision-protocol.md。
  *
  * 正常模式（前四题）下本模块不参与，USART3 也不初始化。
  ******************************************************************************
  */

#ifndef __VISION_SLAVE_H
#define __VISION_SLAVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

uint8_t VisionSlave_Init(UART_HandleTypeDef *uart);
void VisionSlave_DeInit(void);
uint8_t VisionSlave_Process(void);

#ifdef __cplusplus
}
#endif

#endif /* __VISION_SLAVE_H */

