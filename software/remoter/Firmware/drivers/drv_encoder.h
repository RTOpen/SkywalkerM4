
#ifndef __DRV_ENCODER_H
#define __DRV_ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *                                              MACROS
 **************************************************************************************************/
#define  ROTARY_ENCODER_NUM      (2)
/* Rotary Encoder */
#define ROTARY_ENCODER_A         0x00  // Rotary Encoder A
#define ROTARY_ENCODER_B         0x01  // Rotary Encoder B



#define ROTARY_ENCODER_A_A_BV        GPIO_Pin_4
#define ROTARY_ENCODER_A_B_BV        GPIO_Pin_6
#define ROTARY_ENCODER_B_A_BV        GPIO_Pin_3
#define ROTARY_ENCODER_B_B_BV        GPIO_Pin_1

#define ROTARY_ENCODER_A_A_PU                  (R32_PA_PU |= ROTARY_ENCODER_A_A_BV)
#define ROTARY_ENCODER_A_B_PU                  (R32_PA_PU |= ROTARY_ENCODER_A_B_BV)
#define ROTARY_ENCODER_B_A_PU                  (R32_PB_PU |= ROTARY_ENCODER_B_A_BV)
#define ROTARY_ENCODER_B_B_PU                  (R32_PB_PU |= ROTARY_ENCODER_B_B_BV)

#define ROTARY_ENCODER_A_A_DIR                 (R32_PA_DIR &= ~ROTARY_ENCODER_A_A_BV)
#define ROTARY_ENCODER_A_B_DIR                 (R32_PA_DIR &= ~ROTARY_ENCODER_A_B_BV)
#define ROTARY_ENCODER_B_A_DIR                 (R32_PB_DIR &= ~ROTARY_ENCODER_B_A_BV)
#define ROTARY_ENCODER_B_B_DIR                 (R32_PB_DIR &= ~ROTARY_ENCODER_B_B_BV)

#define ROTARY_ENCODER_A_A_IN                  (ACTIVE_LOW(R32_PA_PIN & ROTARY_ENCODER_A_A_BV))
#define ROTARY_ENCODER_A_B_IN                  (ACTIVE_LOW(R32_PA_PIN & ROTARY_ENCODER_A_B_BV))
#define ROTARY_ENCODER_B_A_IN                 (ACTIVE_LOW(R32_PB_PIN & ROTARY_ENCODER_B_A_BV))
#define ROTARY_ENCODER_B_B_IN                 (ACTIVE_LOW(R32_PB_PIN & ROTARY_ENCODER_B_B_BV))



/**************************************************************************************************
 *                                             GLOBAL VARIABLES
 **************************************************************************************************/

/*********************************************************************
 * FUNCTIONS
 */

/**
 * @brief   Initialize the Rotary Encoder
 */
void encoder_hw_init(void);

/**
 * @brief   Read the position of Rotary Encoder
 */
int16_t encoder_read_pos(uint8_t id);



/**************************************************************************************************
**************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
