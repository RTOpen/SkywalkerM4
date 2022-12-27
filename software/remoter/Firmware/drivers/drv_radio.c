
/* 头文件包含 */
#include "CONFIG.h"
#include "board.h"
#include "drv_radio.h"
#include "HAL.h"

/*********************************************************************
 * GLOBAL TYPEDEFS
 */
uint8_t taskID;
uint8_t TX_DATA[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16};

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
                uint8_t i;

                rt_kprintf("tx recv,rssi:%d\n", (int8_t)rxBuf[0]);
                rt_kprintf("len:%d-", rxBuf[1]);

                for (i = 0; i < rxBuf[1]; i++) {
                    rt_kprintf("%x ", rxBuf[i + 2]);
                }
                rt_kprintf("\n");
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
                uint8_t i;

                rt_kprintf("rx recv, rssi: %d\n", (int8_t)rxBuf[0]);
                rt_kprintf("len:%d-", rxBuf[1]);
                
                for (i = 0; i < rxBuf[1]; i++) {
                    rt_kprintf("%x ", rxBuf[i + 2]);
                }
                rt_kprintf("\n");
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
        RF_Tx(TX_DATA, 16, 0xFF, 0xFF);
        tmos_start_task(taskID, SBP_RF_PERIODIC_EVT, 20);
        return events ^ SBP_RF_PERIODIC_EVT;
    }
    if(events & SBP_RF_RF_RX_EVT)
    {
        uint8_t state;
        RF_Shut();
        TX_DATA[0]++;
        RF_Rx(TX_DATA, 16, 0xFF, 0xFF);
        return events ^ SBP_RF_RF_RX_EVT;
    }
    return 0;
}


/*********************************************************************
 * @fn      RF_Init
 *
 * @brief   RF 初始化
 *
 * @return  none
 */
void radio_hw_init(void)
{
    uint8_t    state;
    rfConfig_t rfConfig;

    tmos_memset(&rfConfig, 0, sizeof(rfConfig_t));
    taskID = TMOS_ProcessEventRegister(RF_ProcessEvent);
    rfConfig.accessAddress = 0x71764129; // 禁止使用0x55555555以及0xAAAAAAAA ( 建议不超过24次位反转，且不超过连续的6个0或1 )
    rfConfig.CRCInit = 0x555555;
    rfConfig.Channel = 10;
    //rfConfig.Frequency = 2480000;
    rfConfig.LLEMode = LLE_MODE_AUTO | LLE_MODE_PHY_CODED_S2; // 使能 LLE_MODE_EX_CHANNEL 表示 选择 rfConfig.Frequency 作为通信频点
    rfConfig.rfStatusCB = RF_2G4StatusCallBack;
    rfConfig.RxMaxlen = 251;
    state = RF_Config(&rfConfig);
//    rt_kprintf("rf 2.4g init: %x\n", state);
//    { // RX mode
//        state = RF_Rx(TX_DATA, 10, 0xFF, 0xFF);
//        rt_kprintf("RX mode.state = %x\n", state);
//    }

      { // TX mode
          tmos_set_event( taskID , SBP_RF_PERIODIC_EVT );
      }
}
