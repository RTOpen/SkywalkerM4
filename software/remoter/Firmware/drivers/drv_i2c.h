#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef int16_t i2c_status_t;

#define TxAdderss 0x52

#define I2C_IO_REMAPPING 1

#define I2C_STATUS_SUCCESS (0)
#define I2C_STATUS_ERROR   (-1)
#define I2C_STATUS_TIMEOUT (-2)

int i2c_hw_init(void);

i2c_status_t i2c_start(void);

void i2c_stop(void);

i2c_status_t i2c_transmit(uint8_t address, const uint8_t *data, uint16_t length);

i2c_status_t i2c_receive(uint8_t address, uint8_t *data, uint16_t length);
i2c_status_t i2c_write_reg(uint8_t address, uint8_t reg_addr, const uint8_t *data, uint16_t length);

i2c_status_t i2c_write_reg16(uint8_t address, uint16_t reg_addr, const uint8_t *data, uint16_t length);

i2c_status_t i2c_read_reg(uint8_t address, uint8_t reg_addr, uint8_t *data, uint16_t length);

i2c_status_t i2c_read_reg16(uint8_t address, uint16_t reg_addr, uint8_t *data, uint16_t length);

