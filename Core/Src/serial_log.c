/**
  ******************************************************************************
  * @file    serial_log.c
  * @brief   Compact USART output for FFT diagnostics.
  *
  * 主控不开浮点 printf：Keil 下半浮点格式化会把固件拉大十几 KB，而且
  * 200 ms 打一行、每行二十来个数字，格式化开销比测量本身还大。
  * 所以这里全部用整数毫单位（毫赫兹、毫伏、毫度）自己拼字符串，
  * 定点小数位手工补零。
  *
  * 输出格式见 README「4.5 串口输出」一节；改格式要同步改那里的示例，
  * 否则别人照着示例解析会对不上。
  ******************************************************************************
  */

#include "serial_log.h"

#define SERIAL_LOG_LINE_SIZE 320U

static UART_HandleTypeDef *log_uart = NULL;
static char status_line[SERIAL_LOG_LINE_SIZE];

static uint16_t SerialLog_AppendChar(char *line,
                                     uint16_t length,
                                     uint16_t position,
                                     char value)
{
  if (position < length)
    line[position++] = value;
  return position;
}

static uint16_t SerialLog_AppendText(char *line,
                                     uint16_t length,
                                     uint16_t position,
                                     const char *text)
{
  while (*text != '\0' && position < length)
    line[position++] = *text++;
  return position;
}

static uint16_t SerialLog_AppendU32(char *line,
                                    uint16_t length,
                                    uint16_t position,
                                    uint32_t value)
{
  char digits[10];
  uint8_t count = 0U;

  do
  {
    digits[count++] = (char)('0' + (value % 10U));
    value /= 10U;
  } while (value != 0U && count < sizeof(digits));

  while (count != 0U && position < length)
    line[position++] = digits[--count];
  return position;
}

static uint16_t SerialLog_AppendFixed3(char *line,
                                       uint16_t length,
                                       uint16_t position,
                                       uint32_t milli_value)
{
  uint32_t fraction = milli_value % 1000U;

  position = SerialLog_AppendU32(line, length, position,
                                 milli_value / 1000U);
  position = SerialLog_AppendChar(line, length, position, '.');
  position = SerialLog_AppendChar(line, length, position,
                                  (char)('0' + fraction / 100U));
  position = SerialLog_AppendChar(line, length, position,
                                  (char)('0' + (fraction / 10U) % 10U));
  return SerialLog_AppendChar(line, length, position,
                              (char)('0' + fraction % 10U));
}

static uint16_t SerialLog_AppendSignedFixed3(char *line,
                                             uint16_t length,
                                             uint16_t position,
                                             int32_t milli_value)
{
  uint32_t magnitude;

  if (milli_value < 0)
  {
    position = SerialLog_AppendChar(line, length, position, '-');
    magnitude = (uint32_t)(-milli_value);
  }
  else
  {
    position = SerialLog_AppendChar(line, length, position, '+');
    magnitude = (uint32_t)milli_value;
  }

  return SerialLog_AppendFixed3(line, length, position, magnitude);
}

uint8_t SerialLog_Init(UART_HandleTypeDef *uart)
{
  if (uart == NULL)
    return 0U;

  log_uart = uart;
  return 1U;
}

void SerialLog_DeInit(void)
{
  if (log_uart != NULL)
    (void)HAL_UART_AbortTransmit(log_uart);
  log_uart = NULL;
}

static uint16_t SerialLog_AppendMeasurement(char *line,
                                            uint16_t length,
                                            uint16_t position,
                                            const char *name,
                                            const PeakInfo_t *peak,
                                            uint32_t vpp_mv)
{
  uint32_t frequency_millihz = 0U;
  int32_t phase_mdeg = 0;

  if (peak != NULL && peak->valid != 0U)
  {
    frequency_millihz =
        (uint32_t)(peak->frequency * 1000.0f + 0.5f);
    phase_mdeg = (int32_t)(peak->phase_radians * 180000.0f / PI +
                           ((peak->phase_radians >= 0.0f) ? 0.5f : -0.5f));
  }

  position = SerialLog_AppendText(line, length, position, name);
  position = SerialLog_AppendText(line, length, position, " F=");
  position = SerialLog_AppendFixed3(line, length, position,
                                    frequency_millihz);
  position = SerialLog_AppendText(line, length, position, "Hz Vpp=");
  position = SerialLog_AppendU32(line, length, position, vpp_mv);
  position = SerialLog_AppendText(line, length, position, "mV P=");
  position = SerialLog_AppendSignedFixed3(line, length, position,
                                          phase_mdeg);
  return SerialLog_AppendText(line, length, position, "deg");
}

void SerialLog_PrintStatus(const PeakInfo_t *input_peak,
                           uint32_t input_vpp_mv,
                           const PeakInfo_t *feedback_peak,
                           uint32_t feedback_vpp_mv,
                           const DdsPllState *pll,
                           uint32_t nominal_frequency_millihz)
{
  uint16_t position = 0U;

  if (log_uart == NULL || pll == NULL ||
      HAL_UART_GetState(log_uart) != HAL_UART_STATE_READY)
  {
    return;
  }

  position = SerialLog_AppendMeasurement(status_line, sizeof(status_line),
                                         position, "PA1", input_peak,
                                         input_vpp_mv);
  position = SerialLog_AppendText(status_line, sizeof(status_line),
                                  position, " | ");
  position = SerialLog_AppendMeasurement(status_line, sizeof(status_line),
                                         position, "DDS_FB", feedback_peak,
                                         feedback_vpp_mv);
  position = SerialLog_AppendText(status_line, sizeof(status_line),
                                  position, "\r\nPLL E=");
  position = SerialLog_AppendSignedFixed3(status_line, sizeof(status_line),
                                          position,
                                          pll->phase_error_mdeg);
  position = SerialLog_AppendText(status_line, sizeof(status_line),
                                  position, "deg TARGET=");
  position = SerialLog_AppendSignedFixed3(status_line, sizeof(status_line),
                                          position,
                                          pll->target_phase_mdeg);
  position = SerialLog_AppendText(status_line, sizeof(status_line),
                                  position, "deg CMD=");
  position = SerialLog_AppendFixed3(status_line, sizeof(status_line), position,
                                    pll->commanded_frequency_millihz);
  position = SerialLog_AppendText(status_line, sizeof(status_line),
                                  position, "Hz NOM=");
  position = SerialLog_AppendFixed3(status_line, sizeof(status_line), position,
                                    nominal_frequency_millihz);
  position = SerialLog_AppendText(status_line, sizeof(status_line),
                                  position, "Hz DF=");
  position = SerialLog_AppendSignedFixed3(
      status_line, sizeof(status_line), position,
      pll->frequency_error_millihz);
  position = SerialLog_AppendText(status_line, sizeof(status_line),
                                  position, "Hz TRIM=");
  position = SerialLog_AppendSignedFixed3(
      status_line, sizeof(status_line), position,
      pll->frequency_trim_millihz);
  position = SerialLog_AppendText(status_line, sizeof(status_line),
                                  position, "Hz PCORR=");
  position = SerialLog_AppendSignedFixed3(
      status_line, sizeof(status_line), position,
      pll->phase_frequency_correction_millihz);
  position = SerialLog_AppendText(status_line, sizeof(status_line),
                                  position, "Hz VALID=");
  position = SerialLog_AppendU32(status_line, sizeof(status_line), position,
                                 pll->measurement_valid);
  position = SerialLog_AppendText(status_line, sizeof(status_line),
                                  position, " LOCK=");
  position = SerialLog_AppendU32(status_line, sizeof(status_line), position,
                                 pll->locked);
  position = SerialLog_AppendText(status_line, sizeof(status_line),
                                  position, "\r\n");

  (void)HAL_UART_Transmit_IT(log_uart,
                            (uint8_t *)status_line,
                            position);
}
