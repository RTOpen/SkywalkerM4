/********************************** (C) COPYRIGHT *******************************
 * File Name          : LED.c
 * Author             : WCH
 * Version            : V1.2
 * Date               : 2022/01/18
 * Description        :
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include "HAL.h"



/*********************************************************************
 * @fn      HAL_LedInit
 *
 * @brief   Initialize LED Service
 *
 * @return  none
 */
void led_hw_init(void)
{
    /* Initialize all LEDs to OFF */
    LED1_DDR;
    LED2_DDR;
    led_off(LED_ALL);
}

/**
 * @brief   Turn ON LED.
 */
void led_on(uint8_t leds)
{
  if(leds & LED_1)
  {
      LED1_OUT &= ~LED1_BV;
  }
  if(leds & LED_2)
  {
      LED2_OUT &= ~LED2_BV;
  }
}

/**
 * @brief   Turn OFF LED.
 */
void led_off(uint8_t leds)
{
    if(leds & LED_1)
    {
        LED1_OUT |= LED1_BV;
    }
    if(leds & LED_2)
    {
        LED2_OUT |= LED2_BV;
    }
}

/**
 * @brief   Turn TOGGLE LED.
 */
void led_toggle(uint8_t leds)
{
    if(leds & LED_1)
    {
        LED1_OUT ^= LED1_BV;
    }
    if(leds & LED_2)
    {
        LED2_OUT ^= LED2_BV;
    }
}
