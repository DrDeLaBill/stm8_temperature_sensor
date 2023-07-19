#include "iwdg.h"

#include "main.h"


void iwdg_init()
{
    IWDG->KR  = IWDG_KEY_ENABLE;
    IWDG->KR  = (uint8_t)IWDG_WriteAccess_Enable; /* Write Access */
    IWDG->PR  = (uint8_t)IWDG_Prescaler_8;
    IWDG->RLR = 0x99;
    IWDG->KR  = (uint8_t)IWDG_WriteAccess_Disable; /* Write Access */
}

void iwdg_reload()
{
    IWDG->KR = (uint8_t)IWDG_WriteAccess_Enable; /* Write Access */
    IWDG->KR = IWDG_KEY_REFRESH;
    IWDG->KR = (uint8_t)IWDG_WriteAccess_Disable; /* Write Access */
}