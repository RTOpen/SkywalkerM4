
#ifndef __DRV_KEY_H
#define __DRV_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *                                              MACROS
 **************************************************************************************************/

/* Switches (keys) */
#define KEY_SW_HOME             0x01  // key home
#define KEY_SW_LEFT             0x02  // key left size
#define KEY_SW_RIGHT            0x04  // key right size
#define KEY_SW_POWER            0x08  // key power

/* °´¼ü¶¨Òå */

/* 1 - KEY */
#define KEY_HOME_BV                  BV(22)
#define KEY_LEFT_BV                  BV(5)
#define KEY_RIGHT_BV                 BV(2)
#define KEY_POWER_BV                 BV(6)

#define KEY_HOME_PU                  (R32_PB_PU |= KEY_HOME_BV)
#define KEY_LEFT_PU                  (R32_PA_PU |= KEY_LEFT_BV)
#define KEY_RIGHT_PU                 (R32_PB_PU |= KEY_RIGHT_BV)
#define KEY_POWER_PU                 (R32_PB_PU |= KEY_POWER_BV)

#define KEY_HOME_DIR                 (R32_PB_DIR &= ~KEY_HOME_BV)
#define KEY_LEFT_DIR                 (R32_PA_DIR &= ~KEY_LEFT_BV)
#define KEY_RIGHT_DIR                (R32_PB_DIR &= ~KEY_RIGHT_BV)
#define KEY_POWER_DIR                (R32_PB_DIR &= ~KEY_POWER_BV)

#define KEY_HOME_IN                  (ACTIVE_LOW(R32_PB_PIN & KEY_HOME_BV))
#define KEY_LEFT_IN                  (ACTIVE_LOW(R32_PA_PIN & KEY_LEFT_BV))
#define KEY_RIGHT_IN                 (ACTIVE_LOW(R32_PB_PIN & KEY_RIGHT_BV))
#define KEY_POWER_IN                 (ACTIVE_LOW(R32_PB_PIN & KEY_POWER_BV))



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
