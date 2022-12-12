
#include "board.h"
#include "drv_i2c.h"

#define  I2C_TIMEOUT      (50000)

int i2c_hw_init(void)
{
    GPIOB_ModeCfg(GPIO_Pin_13 | GPIO_Pin_12, GPIO_ModeIN_PU);
    I2C_Init(I2C_Mode_I2C, 400000, I2C_DutyCycle_16_9, I2C_Ack_Enable, I2C_AckAddr_7bit, TxAdderss);
    while (I2C_GetFlagStatus(I2C_FLAG_BUSY)) {
        __nop();
    }
    return 0;
}

i2c_status_t i2c_start(void)
{
    uint32_t timeout= 0;

    I2C_GenerateSTART(ENABLE);

    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) {
        if ((timeout++ > I2C_TIMEOUT)) {
            return I2C_STATUS_TIMEOUT;
        }

    }
    return I2C_STATUS_SUCCESS;
}

void i2c_stop(void)
{
    I2C_GenerateSTOP(ENABLE);
}

i2c_status_t i2c_transmit(uint8_t address, const uint8_t *data, uint16_t length)
{
    i2c_status_t status = i2c_start();

    if (status != I2C_STATUS_SUCCESS) {
        return status;
    }

    uint16_t timeout_timer = timer_read();

    I2C_Send7bitAddress(address, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    for (uint8_t i = 0; i < length; i++) {
        while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) {
            if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
                return I2C_STATUS_TIMEOUT;
            }
        }
        I2C_SendData(data[i]);
    }

    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
            return I2C_STATUS_TIMEOUT;
        }
    }
    i2c_stop();

    return I2C_STATUS_SUCCESS;
}

i2c_status_t i2c_receive(uint8_t address, uint8_t *data, uint16_t length)
{
    i2c_status_t status = i2c_start();

    if (status != I2C_STATUS_SUCCESS) {
        return status;
    }

    uint16_t timeout_timer = timer_read();

    I2C_Send7bitAddress(address, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    for (uint8_t i = 0; i < length; i++) {
        while (!I2C_GetFlagStatus(I2C_FLAG_RXNE)) {
            if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
                return I2C_STATUS_TIMEOUT;
            }
        }
        if (i == length - 1) {
            I2C_AcknowledgeConfig(DISABLE);
        }
        data[i] = I2C_ReceiveData();
    }

    i2c_stop();
    I2C_AcknowledgeConfig(ENABLE);
    return I2C_STATUS_SUCCESS;
}

i2c_status_t i2c_write_reg(uint8_t address, uint8_t regaddr, const uint8_t *data, uint16_t length)
{
    uint8_t buffer[length + 1];

    buffer[0] = regaddr;
    for (uint16_t i = 0; i < length; i++) {
        buffer[i + 1] = data[i];
    }

    return i2c_transmit(address, (const uint8_t *)buffer, length + 1);
}

i2c_status_t i2c_write_reg16(uint8_t address, uint16_t regaddr, const uint8_t *data, uint16_t length)
{
    uint8_t buffer[length + 2];

    buffer[0] = (uint8_t)((regaddr & 0xFF00) >> 8);
    buffer[1] = (uint8_t)(regaddr & 0xFF);
    for (uint16_t i = 0; i < length; i++) {
        buffer[i + 2] = data[i];
    }

    return i2c_transmit(address, (const uint8_t *)buffer, length + 2);
}

i2c_status_t i2c_read_reg(uint8_t address, uint8_t regaddr, uint8_t *data, uint16_t length)
{
    i2c_status_t status = i2c_start();

    if (status != I2C_STATUS_SUCCESS) {
        return status;
    }

    uint16_t timeout_timer = timer_read();

    I2C_Send7bitAddress(address, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) {
        if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
            return I2C_STATUS_TIMEOUT;
        }
    }
    I2C_SendData(regaddr);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    // just resend a start signal to initiate read procedure
    status = i2c_start();
    if (status != I2C_STATUS_SUCCESS) {
        return status;
    }

    I2C_Send7bitAddress(address, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    I2C_GenerateSTOP(DISABLE);

    for (uint8_t i = 0; i < length; i++) {
        while (!I2C_GetFlagStatus(I2C_FLAG_RXNE)) {
            if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
                return I2C_STATUS_TIMEOUT;
            }
        }
        if (i == length - 1) {
            I2C_AcknowledgeConfig(DISABLE);
        }
        data[i] = I2C_ReceiveData();
    }

    i2c_stop();
    I2C_AcknowledgeConfig(ENABLE);
    return I2C_STATUS_SUCCESS;
}

i2c_status_t i2c_read_reg16(uint8_t address, uint16_t regaddr, uint8_t *data, uint16_t length)
{
    i2c_status_t status = i2c_start();

    if (status != I2C_STATUS_SUCCESS) {
        return status;
    }

    uint16_t timeout_timer = timer_read();

    I2C_Send7bitAddress(address, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) {
        if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
            return I2C_STATUS_TIMEOUT;
        }
    }
    I2C_SendData((uint8_t)((regaddr & 0xFF00) >> 8));
    while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) {
        if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
            return I2C_STATUS_TIMEOUT;
        }
    }
    I2C_SendData((uint8_t)(regaddr & 0xFF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    // just resend a start signal to initiate read procedure
    status = i2c_start();
    if (status != I2C_STATUS_SUCCESS) {
        return status;
    }

    I2C_Send7bitAddress(address, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    I2C_GenerateSTOP(DISABLE);

    for (uint8_t i = 0; i < length; i++) {
        while (!I2C_GetFlagStatus(I2C_FLAG_RXNE)) {
            if ((timer_elapsed(timeout_timer) > I2C_TIMEOUT)) {
                return I2C_STATUS_TIMEOUT;
            }
        }
        if (i == length - 1) {
            I2C_AcknowledgeConfig(DISABLE);
        }
        data[i] = I2C_ReceiveData();
    }

    i2c_stop();
    I2C_AcknowledgeConfig(ENABLE);
    return I2C_STATUS_SUCCESS;
}
