/*
 * eeprom.h
 *
 *  Created on: Nov 13, 2023
 *      Author: felipe
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdint.h>
#include <lpc17xx_i2c.h>

#define AT24C_ADDR (0x20080020)
#define TX_DATA_ADDR (0x2007C400)
#define RX_DATA_ADDR (0x2007C800)

typedef struct {

    void (*write_byte) (uint8_t data, uint16_t addr);
    void (*write_half_word) (uint16_t data, uint16_t addr);
    void (*write_word) (uint32_t data, uint16_t addr);
    void (*write_data) (uint8_t *data, uint32_t size, uint16_t addr);
    void (*read_byte) (uint8_t *dest, uint16_t addr);
    void (*read_half_word) (uint8_t *dest, uint16_t addr);
    void (*read_word) (uint8_t *dest, uint16_t addr);
    void (*read_data) (uint8_t *dest, uint32_t size, uint16_t addr);

} AT24C_t;


void init_eeprom(AT24C_t *eeprom, uint8_t dev_Addr, LPC_I2C_TypeDef *I2Cx);

#endif /* EEPROM_H_ */
