 /********************************** (C) COPYRIGHT *******************************
 * File Name          : drv_usart.h
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2022/04/26
 * Description
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/
#ifndef __DRV_USART_H__
#define __DRV_USART_H__
#include "CH58x_common.h"


int usart_init(void);

uint16_t usart_write(uint8_t *data,uint16_t len);

uint16_t usart_read(uint8_t *data,uint16_t len);

uint16_t usart_read_char(uint8_t *data);



#endif /* __BOARD_H__ */
