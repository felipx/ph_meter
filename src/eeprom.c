/*
 * eeprom.c
 *
 *  Created on: Nov 13, 2023
 *      Author: felipe
 */

#include "eeprom.h"


static I2C_M_SETUP_Type i2c_master_cfg;


static void write_byte(uint8_t data, uint16_t addr)
{

}


static void write_data(uint8_t *data, uint32_t size, uint16_t addr)
{

}


static void write_float(float data, uint16_t addr)
{

}


static void read_byte(uint8_t *data, uint16_t addr)
{

}


static read_data(uint8_t *data, uint32_t size, uint16_t addr)
{

}


static read_float(float *data, uint16_t addr)
{

}


void init_eeprom(AT24C_t *eeprom, uint8_t dev_Addr, LPC_I2C_TypeDef *I2Cx)
{
    i2c_master_cfg.sl_addr7bit = dev_Addr;
    i2c_master_cfg.retransmissions_max = 0;

    eeprom->write_byte = write_byte;
    eeprom->write_data = write_data;
    eeprom->write_float = write_float;
    eeprom->read_byte = read_byte;
    eeprom->read_data = read_data;
    eeprom->read_float = read_float;
}
