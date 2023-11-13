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


static void write_half_word(uint16_t data, uint16_t addr)
{

}


static void write_word(uint32_t data, uint16_t addr)
{

}


static void write_data(uint8_t *data, uint32_t size, uint16_t addr)
{

}


static void read_byte(uint8_t *dest, uint16_t addr)
{

}


static void read_half_word(uint8_t *dest, uint16_t addr)
{

}


static void read_word(uint8_t *dest, uint16_t addr)
{

}


static void read_data(uint8_t *dest, uint32_t size, uint16_t addr)
{

}


void init_eeprom(AT24C_t *eeprom, uint8_t dev_Addr, LPC_I2C_TypeDef *I2Cx)
{
    i2c_master_cfg.sl_addr7bit = dev_Addr;
    i2c_master_cfg.retransmissions_max = 0;

    eeprom->write_byte = write_byte;
    eeprom->write_half_word = write_half_word;
    eeprom->write_word = write_word;
    eeprom->write_data = write_data;
    eeprom->read_byte = read_byte;
    eeprom->read_half_word = read_half_word;
    eeprom->read_word = read_word;
    eeprom->read_data = read_data;
}
