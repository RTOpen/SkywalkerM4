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

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 4096
static uint8_t rt_heap[RT_HEAP_SIZE];     // heap default size: 4K(1024 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE-1;
}
#endif

/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{
    /* Set system clock */
    SetSysClock(CLK_SOURCE_PLL_80MHz);
    /* System Tick Configuration */
    _SysTick_Config(GetSysClock() / RT_TICK_PER_SECOND);

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

#ifdef  __RTTHREAD__
__HIGH_CODE
void SysTick_Handler(void)
{
    rt_tick_increase();
    SysTick->SR=0;
}
#endif

#ifdef __FREERTOS__
TaskHandle_t MainTask_Handler;
extern int main();
void main_task()
{
   main();
}
int entry(void)
{
    rt_hw_board_init();
#ifdef DEBUG
    GPIOA_SetBits(bTXD1);
    GPIOA_ModeCfg(bTXD1, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
#endif
    PRINT("start.\n");

    /* create three task */
    xTaskCreate((TaskFunction_t)main_task,
                (const char *)"main",
                (uint16_t)256,
                (void *)NULL,
                (UBaseType_t)10,
                (TaskHandle_t *)&MainTask_Handler);

    vTaskStartScheduler();
    while (1)
    {
        PRINT("shouldn't run at here!!\n");
    }
    return 0;
}
#endif

