/**
  ******************************************************************************
  * @file    tft_display.h
  * @brief   3.5 寸 TFT（FSMC 并口）事件驱动显示。
  *
  * 只在事件发生时刷新，不做周期性重绘：上电/切题显示题号大字，第四题显示
  * 当前档位格数，视觉模式显示 K230 上报的频率。视觉扫描期间刻意不反复刷新，
  * 因为重绘会占用主循环时间，导致 USART3 轮询丢 K230 的命令。
  ******************************************************************************
  */

#ifndef TFT_DISPLAY_H
#define TFT_DISPLAY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void TFT_Display_Init(void);
void TFT_Display_ShowQuestion(uint8_t question_number);
void TFT_Display_ShowFourth(uint8_t vertical_divisions);
void TFT_Display_ShowVision(uint32_t frequency_millihz);
void TFT_Display_UpdateVisionFrequency(uint32_t frequency_millihz);

#ifdef __cplusplus
}
#endif

#endif /* TFT_DISPLAY_H */
