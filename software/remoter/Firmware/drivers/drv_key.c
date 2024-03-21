
/* 头文件包含 */
#include "HAL.h"

/**************************************************************************************************
 *                                        GLOBAL VARIABLES
 **************************************************************************************************/


/**************************************************************************************************
 * @fn      key_hw_init
 *
 * @brief   Initilize Key Service
 *
 * @param   none
 *
 * @return  None
 **************************************************************************************************/
void key_hw_init(void)
{
    KEY_HOME_DIR;
    KEY_HOME_PU;
    KEY_LEFT_DIR;
    KEY_LEFT_PU;
    KEY_RIGHT_DIR;
    KEY_RIGHT_PU;
    KEY_POWER_DIR;
    KEY_POWER_PU;
}

/**************************************************************************************************
 * @fn      key_scan
 *
 * @brief   Read the current value of a key
 *
 * @param   None
 *
 * @return  keys - current keys status
 **************************************************************************************************/
uint8_t key_scan(void)
{
    uint8_t keys = 0;

    if(KEY_HOME_IN)
    {
        keys |= KEY_SW_HOME;
    }
    if(KEY_LEFT_IN)
    {
        keys |= KEY_SW_LEFT;
    }
    if(KEY_RIGHT_IN)
    {
        keys |= KEY_SW_RIGHT;
    }
    if(KEY_POWER_IN)
    {
        keys |= KEY_SW_POWER;
    }
    return keys;
}
