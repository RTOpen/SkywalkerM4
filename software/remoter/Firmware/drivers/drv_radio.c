
/* 头文件包含 */
#include "CONFIG.h"
#include "board.h"
#include "drv_radio.h"
#include "HAL.h"
#include "usbd_cdc_acm_hid.h"

/*********************************************************************
 * GLOBAL TYPEDEFS
 */
uint8_t taskID;
static uint8_t rf_tx_buffer[32];
static rfConfig_t rf_config;
int8_t rssi = -100;

/*********************************************************************
 * @fn      RF_2G4StatusCallBack
 *
 * @brief   RF 状态回调，注意：不可在此函数中直接调用RF接收或者发送API，需要使用事件的方式调用
 *
 * @param   sta     - 状态类型
 * @param   crc     - crc校验结果
 * @param   rxBuf   - 数据buf指针
 *
 * @return  none
 */
void RF_2G4StatusCallBack(uint8_t sta, uint8_t crc, uint8_t *rxBuf)
{
    switch(sta)
    {
        case TX_MODE_TX_FINISH:
        {
            break;
        }
        case TX_MODE_TX_FAIL:
        {
            break;
        }
        case TX_MODE_RX_DATA:
        {
            if (crc == 0) {
                uint8_t len = rxBuf[1];
                rssi = rxBuf[0];
                usbd_cdc_acm_write(&rxBuf[2],len);
            } else {
                if (crc & (1<<0)) {
                    rt_kprintf("crc error\n");
                }

                if (crc & (1<<1)) {
                    rt_kprintf("match type error\n");
                }
            }
            break;
        }
        case TX_MODE_RX_TIMEOUT: // Timeout is about 200us
        {
            break;
        }
        case RX_MODE_RX_DATA:
        {
            if (crc == 0) {
                uint8_t len;

                rssi = rxBuf[0];
                len = rxBuf[1];
                
//                for (i = 0; i < rxBuf[1]; i++) {
//                    rt_kprintf("%x ", rxBuf[i + 2]);
//                }
            } else {
                if (crc & (1<<0)) {
                    rt_kprintf("crc error\n");
                }

                if (crc & (1<<1)) {
                    rt_kprintf("match type error\n");
                }
            }
            tmos_set_event(taskID, SBP_RF_RF_RX_EVT);
            break;
        }
        case RX_MODE_TX_FINISH:
        {
            tmos_set_event(taskID, SBP_RF_RF_RX_EVT);
            break;
        }
        case RX_MODE_TX_FAIL:
        {
            break;
        }
    }
}

/*********************************************************************
 * @fn      RF_ProcessEvent
 *
 * @brief   RF 事件处理
 *
 * @param   task_id - 任务ID
 * @param   events  - 事件标志
 *
 * @return  未完成事件
 */
uint16_t RF_ProcessEvent(uint8_t task_id, uint16_t events)
{
    if(events & SYS_EVENT_MSG)
    {
        uint8_t *pMsg;

        if((pMsg = tmos_msg_receive(task_id)) != NULL)
        {
            // Release the TMOS message
            tmos_msg_deallocate(pMsg);
        }
        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }
    if(events & SBP_RF_START_DEVICE_EVT)
    {
        tmos_start_task(taskID, SBP_RF_PERIODIC_EVT, 20);
        return events ^ SBP_RF_START_DEVICE_EVT;
    }
    if(events & SBP_RF_PERIODIC_EVT)
    {
        uint8_t state;
        RF_Shut();
        RF_Tx(rf_tx_buffer, 16, 0xFF, 0xFF);
        tmos_start_task(taskID, SBP_RF_PERIODIC_EVT, 20);
        return events ^ SBP_RF_PERIODIC_EVT;
    }
    if(events & SBP_RF_RF_RX_EVT)
    {
        uint8_t state;
        RF_Shut();
        //TX_DATA[0]++;
//        RF_Rx(rf_tx_buffer, 16, 0xFF, 0xFF);
        return events ^ SBP_RF_RF_RX_EVT;
    }
    return 0;
}

void radio_set_channel(uint8_t ch)
{
    if(rf_config.Channel == ch)
        return;

    rf_config.accessAddress = 0x71764129; // 禁止使用0x55555555以及0xAAAAAAAA ( 建议不超过24次位反转，且不超过连续的6个0或1 )
    rf_config.CRCInit = 0x555555;
    rf_config.Channel = ch;
    rf_config.LLEMode = LLE_MODE_AUTO | LLE_MODE_PHY_CODED_S2; // 使能 LLE_MODE_EX_CHANNEL 表示 选择 rfConfig.Frequency 作为通信频点
    rf_config.rfStatusCB = RF_2G4StatusCallBack;
    rf_config.RxMaxlen = 251;
    RF_Config(&rf_config);
}
/*********************************************************************
 * @fn      radio_hw_init
 *
 * @brief   radio 初始化
 *
 * @return  none
 */
void radio_hw_init(void)
{
    uint8_t    state;

    taskID = TMOS_ProcessEventRegister(RF_ProcessEvent);
    rf_config.accessAddress = 0x71764129; // 禁止使用0x55555555以及0xAAAAAAAA ( 建议不超过24次位反转，且不超过连续的6个0或1 )
    rf_config.CRCInit = 0x555555;
    rf_config.Channel = 10;
    rf_config.LLEMode = LLE_MODE_AUTO | LLE_MODE_PHY_CODED_S2; // 使能 LLE_MODE_EX_CHANNEL 表示 选择 rfConfig.Frequency 作为通信频点
    rf_config.rfStatusCB = RF_2G4StatusCallBack;
    rf_config.RxMaxlen = 251;
    state = RF_Config(&rf_config);
    rt_kprintf("rf 2.4g init: %x\n", state);
    // TX mode
    tmos_set_event( taskID , SBP_RF_PERIODIC_EVT );
}
