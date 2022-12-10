
/* 头文件包含 */
#include "rtthread.h"
#include "board.h"
#include "HAL.h"

/*
   NOTE:
   - Quadrature encoder makes two waveforms that are 90° out of phase:
                           _______         _______         __
                  PinA ___|       |_______|       |_______|   PinA
          CCW <--              _______         _______
                  PinB _______|       |_______|       |______ PinB
                               _______         _______
                  PinA _______|       |_______|       |______ PinA
          CW  -->          _______         _______         __
                  PinB ___|       |_______|       |_______|   PinB
          The half of the pulses from top to bottom create full state array:
          prev.A+B   cur.A+B   (prev.AB+cur.AB)  Array   Encoder State
          -------   ---------   --------------   -----   -------------
            00         00            0000          0     stop/idle
            00         01            0001          1     CW,  0x01
            00         10            0010         -1     CCW, 0x02
            00         11            0011          0     invalid state
            01         00            0100         -1     CCW, 0x04
            01         01            0101          0     stop/idle
            01         10            0110          0     invalid state
            01         11            0111          1     CW, 0x07
            10         00            1000          1     CW, 0x08
            10         01            1001          0     invalid state
            10         10            1010          0     stop/idle
            10         11            1011         -1     CCW, 0x0B
            11         00            1100          0     invalid state
            11         01            1101         -1     CCW, 0x0D
            11         10            1110          1     CW,  0x0E
            11         11            1111          0     stop/idle
          - CW  states 0b0001, 0b0111, 0b1000, 0b1110
          - CCW states 0b0010, 0b0100, 0b1011, 0b1101
*/

/**************************************************************************************************
 *                                        GLOBAL VARIABLES
 **************************************************************************************************/
typedef struct
{
 uint16_t pos;
 uint8_t state;
}encoder_data_t;

ALIGN(RT_ALIGN_SIZE)
static encoder_data_t encoder_data[ROTARY_ENCODER_NUM];

void state_change(encoder_data_t *data,uint16_t astate, uint16_t bstate)
{
    data->state = data->state << 2;
    if(astate)
    {
        data->state |=0x02;
    }
    if(bstate)
    {
        data->state |=0x01;
    }


  switch (data->state&0x0F)
  {

    case 0x01:
    case 0x07:
    case 0x08:
    case 0x0E:                                      //CW states, 1 count  per click
        data->pos++;
      break;
    case 0x02:
    case 0x04:
    case 0x0B:
    case 0x0D:                                   //CCW states, 1 count  per click
        data->pos--;
      break;
  }

}

/**************************************************************************************************
 * @fn      encoder_hw_init
 *
 * @brief   Initialize the Rotary Encoder
 *
 * @param   none
 *
 * @return  None
 **************************************************************************************************/
void encoder_hw_init(void)
{
    ROTARY_ENCODER_A_A_DIR;
    ROTARY_ENCODER_A_A_PU;
    ROTARY_ENCODER_A_B_DIR;
    ROTARY_ENCODER_A_B_PU;
    ROTARY_ENCODER_B_A_DIR;
    ROTARY_ENCODER_B_A_PU;
    ROTARY_ENCODER_B_B_DIR;
    ROTARY_ENCODER_B_B_PU;
    GPIOA_ITModeCfg(ROTARY_ENCODER_A_A_BV,GPIO_ITMode_FallEdge);
    GPIOB_ITModeCfg(ROTARY_ENCODER_B_A_BV,GPIO_ITMode_FallEdge);
    PFIC_EnableIRQ(GPIO_A_IRQn);
    PFIC_EnableIRQ(GPIO_B_IRQn);
}

/**************************************************************************************************
 * @fn      encoder_read_pos
 *
 * @brief   Read the position of Rotary Encoder
 *
 * @param   id
 *
 * @return  pos - current keys status
 **************************************************************************************************/
uint16_t encoder_read_pos(uint8_t id)
{
    if(id >= ROTARY_ENCODER_NUM)
        return 0;

    return encoder_data[id].pos;
}

/*********************************************************************
 * @fn      GPIOA_IRQHandler
 *
 * @brief   GPIOA中断函数
 *
 * @return  none
 */
__HIGH_CODE
void GPIOA_IRQHandler(void)
{
    if(GPIOA_ReadITFlagBit(ROTARY_ENCODER_A_A_BV))
    {
    uint16_t astat = (R32_PA_PIN & ROTARY_ENCODER_A_A_BV);
    uint16_t bstat = (R32_PA_PIN & ROTARY_ENCODER_A_B_BV);
    GPIOA_ClearITFlagBit(ROTARY_ENCODER_A_A_BV);
    state_change(&encoder_data[0],astat,bstat);
    if(astat)
    {
        GPIOA_ITModeCfg(ROTARY_ENCODER_A_A_BV,GPIO_ITMode_FallEdge);
    }else {
        GPIOA_ITModeCfg(ROTARY_ENCODER_A_A_BV,GPIO_ITMode_RiseEdge);
    }
    }
}

/*********************************************************************
 * @fn      GPIOB_IRQHandler
 *
 * @brief   GPIOB中断函数
 *
 * @return  none
 */
__HIGH_CODE
void GPIOB_IRQHandler(void)
{
    if(GPIOB_ReadITFlagBit(ROTARY_ENCODER_B_A_BV))
    {
    uint16_t astat = (R32_PB_PIN & ROTARY_ENCODER_B_A_BV);
    uint16_t bstat = (R32_PB_PIN & ROTARY_ENCODER_B_B_BV);
    GPIOB_ClearITFlagBit(ROTARY_ENCODER_B_A_BV);
    state_change(&encoder_data[1],astat,bstat);
    if(astat)
    {
        GPIOB_ITModeCfg(ROTARY_ENCODER_B_A_BV,GPIO_ITMode_FallEdge);
    }else {
        GPIOB_ITModeCfg(ROTARY_ENCODER_B_A_BV,GPIO_ITMode_RiseEdge);
    }
    }
}
