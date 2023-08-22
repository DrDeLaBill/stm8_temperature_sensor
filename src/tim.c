#include "tim.h"

#include "stm8s.h"
#include "main.h"
#include "utils.h"


#define TIM1_COUNT_DOWN_VALUE 20
#define TIM1_DEFAULT_DIVIDER  2000

#define TIM2_COUNT_UP_VALUE   ((uint16_t)0xFFFF)
#define TIM2_TARGET_TIME_S    (ADT7420_MEASURE_TIME / 1000)


void _tim1_init();
void _tim2_init();


void tim_init()
{
    _tim1_init();
    _tim2_init();
}

void _tim1_init()
{
    TIM1->CR1  = TIM1_CR1_RESET_VALUE;
    TIM1->CR2  = TIM1_CR2_RESET_VALUE;
    TIM1->SMCR = TIM1_SMCR_RESET_VALUE;
    TIM1->ETR  = TIM1_ETR_RESET_VALUE;
    TIM1->IER  = TIM1_IER_RESET_VALUE;
    TIM1->SR2  = TIM1_SR2_RESET_VALUE;
    /* Disable channels */
    TIM1->CCER1 = TIM1_CCER1_RESET_VALUE;
    TIM1->CCER2 = TIM1_CCER2_RESET_VALUE;
    /* Configure channels as inputs: it is necessary if lock level is equal to 2 or 3 */
    TIM1->CCMR1 = 0x01;
    TIM1->CCMR2 = 0x01;
    TIM1->CCMR3 = 0x01;
    TIM1->CCMR4 = 0x01;
    /* Then reset channel registers: it also works if lock level is equal to 2 or 3 */
    TIM1->CCER1 = TIM1_CCER1_RESET_VALUE;
    TIM1->CCER2 = TIM1_CCER2_RESET_VALUE;
    TIM1->CCMR1 = TIM1_CCMR1_RESET_VALUE;
    TIM1->CCMR2 = TIM1_CCMR2_RESET_VALUE;
    TIM1->CCMR3 = TIM1_CCMR3_RESET_VALUE;
    TIM1->CCMR4 = TIM1_CCMR4_RESET_VALUE;
    TIM1->CNTRH = TIM1_CNTRH_RESET_VALUE;
    TIM1->CNTRL = TIM1_CNTRL_RESET_VALUE;
    TIM1->PSCRH = TIM1_PSCRH_RESET_VALUE;
    TIM1->PSCRL = TIM1_PSCRL_RESET_VALUE;
    TIM1->ARRH  = TIM1_ARRH_RESET_VALUE;
    TIM1->ARRL  = TIM1_ARRL_RESET_VALUE;
    TIM1->CCR1H = TIM1_CCR1H_RESET_VALUE;
    TIM1->CCR1L = TIM1_CCR1L_RESET_VALUE;
    TIM1->CCR2H = TIM1_CCR2H_RESET_VALUE;
    TIM1->CCR2L = TIM1_CCR2L_RESET_VALUE;
    TIM1->CCR3H = TIM1_CCR3H_RESET_VALUE;
    TIM1->CCR3L = TIM1_CCR3L_RESET_VALUE;
    TIM1->CCR4H = TIM1_CCR4H_RESET_VALUE;
    TIM1->CCR4L = TIM1_CCR4L_RESET_VALUE;
    TIM1->OISR  = TIM1_OISR_RESET_VALUE;
    TIM1->EGR   = 0x01; /* TIM1_EGR_UG */
    TIM1->DTR   = TIM1_DTR_RESET_VALUE;
    TIM1->BKR   = TIM1_BKR_RESET_VALUE;
    TIM1->RCR   = TIM1_RCR_RESET_VALUE;
    TIM1->SR1   = TIM1_SR1_RESET_VALUE;

    
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

    TIM1->CR1 |= TIM1_CR1_CEN;
}

void _tim2_init()
{
    TIM2->PSCR = TIM2_PRESCALER;

    uint16_t arr_val = (get_clock_freq() * TIM2_TARGET_TIME_S) / u32_pow(2, TIM2_PRESCALER);
    TIM2->ARRH = (uint8_t)(arr_val >> 8);
    TIM2->ARRL = (uint8_t)(arr_val);

    TIM2->IER |= TIM2_IER_UIE;

    TIM2->CR1 |= TIM2_CR1_CEN;
}