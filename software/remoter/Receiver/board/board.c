/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 * 2018-11-12     Ernest Chen  modify copyright
 */
 
#include <stdint.h>
#include "board.h"

static uint32_t _SysTick_Config(uint32_t ticks)
{
    PFIC_SetPriority(SysTick_IRQn,0x00);
    PFIC_SetPriority(SWI_IRQn,0x00);
    PFIC_EnableIRQ(SysTick_IRQn);
    PFIC_EnableIRQ(SWI_IRQn);
    SysTick->CTLR=0;
    SysTick->SR=0;
    SysTick->CNT=0;
    SysTick->CMP=ticks-1;
    SysTick->CTLR=0xF;

    return 0;
}


/**
 * This function will initial your board.
 */
void rt_hw_board_init(void)
{
    /* Set system clock */
    SetSysClock(CLK_SOURCE_PLL_60MHz);
    /* System Tick Configuration */
    _SysTick_Config(GetSysClock() / RT_TICK_PER_SECOND);

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

}

__HIGH_CODE
void SysTick_Handler(void)
{
    rt_tick_increase();
    SysTick->SR=0;
}

