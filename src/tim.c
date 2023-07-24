#include "tim.h"

#include "stm8s.h"
#include "main.h"
#include "utils.h"


#define TIM1_COUNT_DOWN_VALUE 20
#define TIM1_DEFAULT_DIVIDER  2000


void tim_init()
{
    /* Set the Autoreload value */
    TIM1->ARRH = (uint8_t)(TIM1_COUNT_DOWN_VALUE >> 8);
    TIM1->ARRL = (uint8_t)(TIM1_COUNT_DOWN_VALUE);
    /* Set the Prescaler value */
    uint32_t hsi_divider = get_clock_freq() / (TIM1_DEFAULT_DIVIDER / TIM1_COUNT_DOWN_VALUE);
    uint32_t presc = (get_clock_freq() / hsi_divider) - 1;
    TIM1->PSCRH = (uint8_t)(presc >> 8);
    TIM1->PSCRL = (uint8_t)(presc);
    /* Select the Counter Mode */
    TIM1->CR1 = (uint8_t)((uint8_t)(TIM1->CR1 & (uint8_t)(~(TIM1_CR1_CMS | TIM1_CR1_DIR)))
                        | (uint8_t)(TIM1_COUNTERMODE_DOWN));
    /* Set the Repetition Counter value */
    TIM1->RCR = TIM1_PSCRELOADMODE_UPDATE;
  
    /* Enable the Interrupt sources */
    TIM1->IER |= (uint8_t)TIM1_IT_UPDATE;

    /* Enable the Interrupt sources */
    TIM1->IER |= (uint8_t)TIM1_IT_CC1;

    TIM1->CR1 |= TIM1_CR1_CEN;
}