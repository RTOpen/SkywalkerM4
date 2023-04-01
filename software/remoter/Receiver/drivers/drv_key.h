
#ifndef __DRV_KEY_H
#define __DRV_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *                                              MACROS
 **************************************************************************************************/
#define KEY_SW_NUM             (1)
/* Switches (keys) */
#define KEY_SW_BIND             0x01  // key bind


/* 1 - KEY */
#define KEY_BIND_BV                  BV(22)

#define KEY_BIND_PU                  (R32_PB_PU |= KEY_BIND_BV)

#define KEY_BIND_DIR                 (R32_PB_DIR &= ~KEY_BIND_BV)

#define KEY_BIND_IN                  (ACTIVE_LOW(R32_PB_PIN & KEY_BIND_BV))




/**************************************************************************************************
 *                                             GLOBAL VARIABLES
 **************************************************************************************************/

/*********************************************************************
 * FUNCTIONS
 */

/**
 * @brief   Initialize the Key
 */
void key_hw_init(void);

/**
 * @brief   key scan
 */
uint8_t key_scan(void);



/**************************************************************************************************
**************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
