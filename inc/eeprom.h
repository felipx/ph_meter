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

#define AT24C08_ADDR (0x20081000)
#define TX_DATA_ADDR (0x2007D000)
#define RX_DATA_ADDR (0x2007D400)
#define N_BYTES (1024)

typedef enum {
    AT24C01,
    AT24C02,
    AT24C04,
    AT24C08,
	AT24C16
} EEPROM_t;

typedef struct at24c_eeprom AT24C_t;

typedef struct at24c_eeprom{

    LPC_I2C_TypeDef *I2Cx;
    EEPROM_t eeprom_type;
    uint8_t dev_addr;
    void (*write_byte) (AT24C_t *eeprom, uint8_t data, uint16_t addr);
    void (*write_half_word) (AT24C_t *eeprom, uint16_t data, uint16_t addr);
    void (*write_word) (AT24C_t *eeprom, uint32_t data, uint16_t addr);
    void (*write_data) (AT24C_t *eeprom, uint8_t *data, uint32_t size, uint16_t addr);
    void (*read_byte) (AT24C_t *eeprom, uint8_t *dest, uint16_t addr);
    void (*read_half_word) (AT24C_t *eeprom, uint8_t *dest, uint16_t addr);
    void (*read_word) (AT24C_t *eeprom, uint8_t *dest, uint16_t addr);
    void (*read_data) (AT24C_t *eeprom, uint8_t *dest, uint32_t size, uint16_t addr);

} AT24C_t;


void init_eeprom(AT24C_t *eeprom, EEPROM_t mem_type, uint8_t dev_Addr, LPC_I2C_TypeDef *I2Cx);

#endif /* EEPROM_H_ */
