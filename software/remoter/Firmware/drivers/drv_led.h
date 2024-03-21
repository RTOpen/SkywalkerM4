/********************************** (C) COPYRIGHT *******************************
 * File Name          : LED.h
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2016/04/12
 * Description        :
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/******************************************************************************/
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * CONSTANTS
 */

/* LEDS - The LED number is the same as the bit position */
#define LED_1                      0x01
#define LED_2                      0x02
#define LED_ALL                    (LED_1 | LED_2)


/*********************************************************************
 * TYPEDEFS
 */

/* 连接一个LED用于监控演示程序的进度,低电平LED亮 */

/* 1 - LED */
#define LED1_BV                 BV(8)
#define LED2_BV                 BV(16)

#define LED1_OUT                (R32_PB_OUT)
#define LED2_OUT                (R32_PB_OUT)

#define LED1_DDR                (R32_PB_DIR |= LED1_BV)
#define LED2_DDR                (R32_PB_DIR |= LED2_BV)



/*********************************************************************
 * GLOBAL VARIABLES
 */

/**
 * @brief   Initialize LED .
 */
void led_hw_init(void);

/**
 * @brief   Turn ON LED.
 */
void led_on(uint8_t leds);

/**
 * @brief   Turn OFF LED.
 */
void led_off(uint8_t leds);

/**
 * @brief   Turn TOGGLE LED.
 */
void led_toggle(uint8_t leds);



#ifdef __cplusplus
}
#endif

#endif
