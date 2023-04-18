#include "utils.h"

#include <stdint.h>

#include "stm8s.h"
#include "main.h"


bool wait_event(bool (*condition) (void), uint32_t time)
{
    uint32_t start_time = Global_time;
    while (ABS_DIF(start_time, Global_time) < time) {
        if (condition()) {
            return TRUE;
        }
    }
    return FALSE;
}

uint32_t get_clock_freq()
{
  uint32_t clock_frequency = 0;
  uint8_t clock_source;
  uint8_t tmp = 0, presc = 0;
  const uint8_t HSI_div_factor[4] = {1, 2, 4, 8}; /*!< Holds the different HSI Divider factors */
  
  /* Get CLK source. */
  clock_source = (uint8_t)CLK->CMSR;
  
  tmp = (uint8_t)(CLK->CKDIVR & CLK_CKDIVR_HSIDIV);
  tmp = (uint8_t)(tmp >> 3);
  presc = HSI_div_factor[tmp];
  clock_frequency = HSI_VALUE / presc;

  return ((uint32_t)clock_frequency);
}