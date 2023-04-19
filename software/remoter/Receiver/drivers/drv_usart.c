 /********************************** (C) COPYRIGHT *******************************
 * File Name          : drv_usart.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2022/04/26
 * Description
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/
#include "drv_usart.h"
#include "board.h"
#include "rtdevice.h"
#include <string.h>

#define RT_UART_RINGBUF_SIZE        256
static struct rt_ringbuffer uart_tx_ringbuf;
static struct rt_ringbuffer uart_rx_ringbuf;
ALIGN(RT_ALIGN_SIZE) uint8_t uart_tx_buffer[RT_UART_RINGBUF_SIZE];
ALIGN(RT_ALIGN_SIZE) uint8_t uart_rx_buffer[RT_UART_RINGBUF_SIZE];

int usart_init(void)
{
    rt_ringbuffer_init(&uart_tx_ringbuf, uart_tx_buffer, RT_UART_RINGBUF_SIZE);
    rt_ringbuffer_init(&uart_rx_ringbuf, uart_rx_buffer, RT_UART_RINGBUF_SIZE);

    /* ���ô���1��������IO��ģʽ�������ô��� */
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);      // RXD-������������
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA); // TXD-�������������ע������IO������ߵ�ƽ

    UART1_BaudRateCfg(115200);
    R8_UART1_FCR = (0 << 6) | RB_FCR_TX_FIFO_CLR | RB_FCR_RX_FIFO_CLR | RB_FCR_FIFO_EN;
    R8_UART1_LCR = RB_LCR_WORD_SZ;
    R8_UART1_IER = RB_IER_TXD_EN;
    R8_UART1_DIV = 1;

    UART1_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART1_IRQn);

    return 0;
}
INIT_BOARD_EXPORT(usart_init);//add to board auto initial

// output
void rt_hw_console_output(const char *str)
{
//    rt_size_t i = 0, size = 0;
//    char a = '\r';
//
//    size = rt_strlen(str);
//    for (i = 0; i < size; i++)
//    {
//        if (*(str + i) == '\n')
//        {
//
//            while(R8_UART1_TFC == UART_FIFO_SIZE);
//            R8_UART1_THR = a;   //if end of '\n', then add '\r'
//        }
//        while(R8_UART1_TFC == UART_FIFO_SIZE);
//        R8_UART1_THR = *(str + i);
//    }
}


//input
char rt_hw_console_getchar(void)
{
    /* Note: the initial value of ch must < 0 */
//    rt_uint8_t ch;
//
//    if(rt_ringbuffer_getchar(&uart_rx_ringbuf, &ch) != 0)
//    {
//        return ch;
//    }
//    else
//    {
        return -1;
//    }
//    return ch;
}

uint16_t usart_write(uint8_t *data,uint16_t len)
{
    uint16_t i = 0;
    if(len == 0)
    {
        return len;
    }
    while(i < len)
    {
    rt_ringbuffer_putchar(&uart_tx_ringbuf, data[i++]);
    }

    UART1_INTCfg(ENABLE, RB_IER_THR_EMPTY);

    return len;
}

uint16_t usart_read(uint8_t *data,uint16_t len)
{
    uint16_t i = 0;
    while(i < len)
    {
        if(rt_ringbuffer_getchar(&uart_rx_ringbuf, &data[i])!=0)
        {
            i++;
        }else
        {
            break;
        }
    }

    return len;
}

uint16_t usart_read_char(uint8_t *data)
{
    return rt_ringbuffer_getchar(&uart_rx_ringbuf, data);
}

__HIGH_CODE
void UART1_IRQHandler(void)
{
    uint8_t ch;
    switch (UART1_GetITFlag())
    {
    case UART_II_LINE_STAT:
        ch = UART1_GetLinSTA();
        break;
    case UART_II_RECV_RDY:
    case UART_II_RECV_TOUT:
    {
        while (R8_UART1_RFC)
        {
            ch = R8_UART1_RBR;
            rt_ringbuffer_putchar(&uart_rx_ringbuf, ch);
        }
        break;
    }
    case UART_II_THR_EMPTY:
        if(rt_ringbuffer_getchar(&uart_tx_ringbuf, &ch) != 0)
            {
                R8_UART1_THR = ch;
            }
            else
            {
                UART1_INTCfg(DISABLE, RB_IER_THR_EMPTY);
            }
        break;
    default:
        break;
    }
}
