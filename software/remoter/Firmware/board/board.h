/*
 * File      : board.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-09-22     Bernard      add board.h to this bsp
 * 2017-10-20     ZYH          emmm...setup for HAL Libraries
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#ifdef __FREERTOS__
#include "freertos.h"
#include "task.h"
#include "semphr.h"

#define rt_err_t int
/* RT-Thread error code definitions */
#define RT_EOK                          0               /**< There is no error */
#define RT_ERROR                        1               /**< A generic error happens */
#define RT_ETIMEOUT                     2               /**< Timed out */
#define RT_EFULL                        3               /**< The resource is full */
#define RT_EEMPTY                       4               /**< The resource is empty */
#define RT_ENOMEM                       5               /**< No memory */
#define RT_ENOSYS                       6               /**< No system */
#define RT_EBUSY                        7               /**< Busy */
#define RT_EIO                          8               /**< IO error */
#define RT_EINTR                        9               /**< Interrupted system call */
#define RT_EINVAL                       10              /**< Invalid argument */

#define rt_thread_delay(x) vTaskDelay(x)
#define rt_thread_mdelay(x) vTaskDelay(x/portTICK_PERIOD_MS)
#define RT_TICK_PER_SECOND configTICK_RATE_HZ
#define rt_tick_get xTaskGetTickCount
#endif

#ifdef __RTTHREAD__
#include <rtthread.h>
#endif
#include "CH58x_common.h"
#include "drv_usart.h"
#include "drv_adc.h"
#include "drv_led.h"
#include "drv_buzzer.h"
#include "drv_lcd.h"
#include "drv_usbd.h"
#include "drv_radio.h"
#include "drv_encoder.h"

/* board configuration */
#define SRAM_SIZE  32
#define SRAM_END (0x20000000 + SRAM_SIZE * 1024)

extern int _ebss;
#define HEAP_BEGIN  ((void *)&_ebss)
#define HEAP_END    (SRAM_END-2048) //reserved for IRQ

void rt_hw_board_init(void);

#endif /* __BOARD_H__ */
