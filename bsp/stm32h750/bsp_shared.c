#include "bsp_shared.h"
#include "stm32h7xx_hal.h"

void bsp_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

void bsp_delay_us(uint32_t us)
{
    volatile uint32_t remain = us << 1;
    if (us < 1000)
    {
        while (remain--)
        {
            __NOP();
        }
    }
    else
    {
        bsp_delay_ms(us / 1000);
        remain = (us % 1000) << 1;
        while (remain--)
        {
            __NOP();
        }
    }
}

uint32_t bsp_get_tick_count(void)
{
    return HAL_GetTick();
}