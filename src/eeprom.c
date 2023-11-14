/*
 * eeprom.c
 *
 *  Created on: Nov 13, 2023
 *      Author: felipe
 */

#include "eeprom.h"


static uint8_t *tx_data = (uint8_t *) TX_DATA_ADDR;
static uint8_t *rx_data = (uint8_t *) RX_DATA_ADDR;


static void write_byte(AT24C_t *eeprom, uint8_t data, uint16_t addr)
{
    I2C_M_SETUP_Type i2c_transfer_cfg;
    uint8_t page_addr = 0x00;

    switch (eeprom->eeprom_type) {
    case AT24C01:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C02:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C04:
        page_addr = (addr >> 8) & 0x01;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C08:
        page_addr = (addr >> 8) & 0x03;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C16:
        page_addr = (addr >> 8) & 0x07;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    break;
    default:
        return;
    }

    *tx_data = addr & 0xFF;
    *(tx_data + 1) = data;

    i2c_transfer_cfg.tx_data = tx_data;
    i2c_transfer_cfg.tx_length = 2;
    i2c_transfer_cfg.rx_data = NULL;
    i2c_transfer_cfg.rx_length = 0;
    i2c_transfer_cfg.retransmissions_max = 0;

    I2C_MasterTransferData(eeprom->I2Cx, &i2c_transfer_cfg, I2C_TRANSFER_POLLING);
    for (int i=0; i<10000; i++);
}


static void write_half_word(AT24C_t *eeprom, uint16_t data, uint16_t addr)
{
    I2C_M_SETUP_Type i2c_transfer_cfg;
    uint8_t page_addr = 0x00;

    switch (eeprom->eeprom_type) {
    case AT24C01:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C02:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C04:
        page_addr = (addr >> 8) & 0x01;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C08:
        page_addr = (addr >> 8) & 0x03;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C16:
        page_addr = (addr >> 8) & 0x07;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    break;
    default:
        return;
    }

    *tx_data = addr & 0xFF;
    *(tx_data + 1) = *((uint8_t *) &data);
    *(tx_data + 2) = *((uint8_t *) &data + 1);

    i2c_transfer_cfg.tx_data = tx_data;
    i2c_transfer_cfg.tx_length = 3;
    i2c_transfer_cfg.rx_data = NULL;
    i2c_transfer_cfg.rx_length = 0;
    i2c_transfer_cfg.retransmissions_max = 0;

    I2C_MasterTransferData(eeprom->I2Cx, &i2c_transfer_cfg, I2C_TRANSFER_POLLING);
    for (int i=0; i<10000; i++);
}


static void write_word(AT24C_t *eeprom, uint32_t data, uint16_t addr)
{
    I2C_M_SETUP_Type i2c_transfer_cfg;
    uint8_t page_addr = 0x00;

    switch (eeprom->eeprom_type) {
    case AT24C01:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C02:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C04:
        page_addr = (addr >> 8) & 0x01;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C08:
        page_addr = (addr >> 8) & 0x03;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C16:
        page_addr = (addr >> 8) & 0x07;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    break;
    default:
        return;
    }

    *tx_data = addr & 0xFF;
    for (int i=0; i<4; i++)
        *(tx_data + 1 + i) = *((uint8_t *) &data + i);

    i2c_transfer_cfg.tx_data = tx_data;
    i2c_transfer_cfg.tx_length = 5;
    i2c_transfer_cfg.rx_data = NULL;
    i2c_transfer_cfg.rx_length = 0;
    i2c_transfer_cfg.retransmissions_max = 0;

    I2C_MasterTransferData(eeprom->I2Cx, &i2c_transfer_cfg, I2C_TRANSFER_POLLING);
    for (int i=0; i<10000; i++);
}


static void write_data(AT24C_t *eeprom, uint8_t *data, uint32_t size, uint16_t addr)
{
    I2C_M_SETUP_Type i2c_transfer_cfg;
    uint8_t page_addr = 0x00;

    switch (eeprom->eeprom_type) {
    case AT24C01:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C02:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C04:
        page_addr = (addr >> 8) & 0x01;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C08:
        page_addr = (addr >> 8) & 0x03;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C16:
        page_addr = (addr >> 8) & 0x07;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    break;
    default:
        return;
    }

    *tx_data = addr & 0xFF;
    for (uint32_t i=0; i<size; i++)
        *(tx_data + 1 + i) = *(data + i);

    i2c_transfer_cfg.tx_data = tx_data;
    i2c_transfer_cfg.tx_length = size + 1;
    i2c_transfer_cfg.rx_data = NULL;
    i2c_transfer_cfg.rx_length = 0;
    i2c_transfer_cfg.retransmissions_max = 0;

    I2C_MasterTransferData(eeprom->I2Cx, &i2c_transfer_cfg, I2C_TRANSFER_POLLING);
    for (int i=0; i<10000; i++);
}


static void read_byte(AT24C_t *eeprom, uint8_t *dest, uint16_t addr)
{
    I2C_M_SETUP_Type i2c_transfer_cfg;
    uint8_t page_addr = 0x00;

    switch (eeprom->eeprom_type) {
    case AT24C01:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C02:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C04:
        page_addr = (addr >> 8) & 0x01;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C08:
        page_addr = (addr >> 8) & 0x03;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C16:
        page_addr = (addr >> 8) & 0x07;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    break;
    default:
        return;
    }

    *tx_data = addr & 0xFF;
    i2c_transfer_cfg.tx_data = tx_data;
    i2c_transfer_cfg.tx_length = 1;
    i2c_transfer_cfg.rx_data = NULL;
    i2c_transfer_cfg.rx_length = 0;
    i2c_transfer_cfg.retransmissions_max = 0;

    I2C_MasterTransferData(eeprom->I2Cx, &i2c_transfer_cfg, I2C_TRANSFER_POLLING);
    for (int i=0; i<10000; i++);

    i2c_transfer_cfg.tx_data = NULL;
    i2c_transfer_cfg.tx_length = 0;
    i2c_transfer_cfg.rx_data = rx_data;
    i2c_transfer_cfg.rx_length = 1;
    i2c_transfer_cfg.retransmissions_max = 0;

    I2C_MasterTransferData(eeprom->I2Cx, &i2c_transfer_cfg, I2C_TRANSFER_POLLING);
    for (int i=0; i<10000; i++);

    if (dest != NULL)
        *dest = *rx_data;
}


static void read_half_word(AT24C_t *eeprom, uint8_t *dest, uint16_t addr)
{
    I2C_M_SETUP_Type i2c_transfer_cfg;
    uint8_t page_addr = 0x00;

    switch (eeprom->eeprom_type) {
    case AT24C01:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C02:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C04:
        page_addr = (addr >> 8) & 0x01;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C08:
        page_addr = (addr >> 8) & 0x03;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C16:
        page_addr = (addr >> 8) & 0x07;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    break;
    default:
        return;
    }

    *tx_data = addr & 0xFF;
    i2c_transfer_cfg.tx_data = tx_data;
    i2c_transfer_cfg.tx_length = 1;
    i2c_transfer_cfg.rx_data = NULL;
    i2c_transfer_cfg.rx_length = 0;
    i2c_transfer_cfg.retransmissions_max = 0;

    I2C_MasterTransferData(eeprom->I2Cx, &i2c_transfer_cfg, I2C_TRANSFER_POLLING);
    for (int i=0; i<10000; i++);

    i2c_transfer_cfg.tx_data = NULL;
    i2c_transfer_cfg.tx_length = 0;
    i2c_transfer_cfg.rx_data = rx_data;
    i2c_transfer_cfg.rx_length = 2;
    i2c_transfer_cfg.retransmissions_max = 0;

    I2C_MasterTransferData(eeprom->I2Cx, &i2c_transfer_cfg, I2C_TRANSFER_POLLING);
    for (int i=0; i<10000; i++);

    if (dest != NULL) {
        *dest = *rx_data;
        *(dest + 1) = *(rx_data + 1);
    }
}


static void read_word(AT24C_t *eeprom, uint8_t *dest, uint16_t addr)
{
    I2C_M_SETUP_Type i2c_transfer_cfg;
    uint8_t page_addr = 0x00;

    switch (eeprom->eeprom_type) {
    case AT24C01:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C02:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C04:
        page_addr = (addr >> 8) & 0x01;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C08:
        page_addr = (addr >> 8) & 0x03;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C16:
        page_addr = (addr >> 8) & 0x07;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    break;
    default:
        return;
    }

    *tx_data = addr & 0xFF;
    i2c_transfer_cfg.tx_data = tx_data;
    i2c_transfer_cfg.tx_length = 1;
    i2c_transfer_cfg.rx_data = NULL;
    i2c_transfer_cfg.rx_length = 0;
    i2c_transfer_cfg.retransmissions_max = 0;

    I2C_MasterTransferData(eeprom->I2Cx, &i2c_transfer_cfg, I2C_TRANSFER_POLLING);
    for (int i=0; i<10000; i++);

    i2c_transfer_cfg.tx_data = NULL;
    i2c_transfer_cfg.tx_length = 0;
    i2c_transfer_cfg.rx_data = rx_data;
    i2c_transfer_cfg.rx_length = 4;
    i2c_transfer_cfg.retransmissions_max = 0;

    I2C_MasterTransferData(eeprom->I2Cx, &i2c_transfer_cfg, I2C_TRANSFER_POLLING);
    for (int i=0; i<10000; i++);

    if (dest != NULL) {
        for (int i=0; i<4; i++)
            *(dest + i) = *(rx_data + i);
    }
}


static void read_data(AT24C_t *eeprom, uint8_t *dest, uint32_t size, uint16_t addr)
{
    I2C_M_SETUP_Type i2c_transfer_cfg;
    uint8_t page_addr = 0x00;

    switch (eeprom->eeprom_type) {
    case AT24C01:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C02:
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr;
        break;
    case AT24C04:
        page_addr = (addr >> 8) & 0x01;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C08:
        page_addr = (addr >> 8) & 0x03;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    case AT24C16:
        page_addr = (addr >> 8) & 0x07;
        i2c_transfer_cfg.sl_addr7bit = eeprom->dev_addr | page_addr;
        break;
    break;
    default:
        return;
    }

    *tx_data = addr & 0xFF;
    i2c_transfer_cfg.tx_data = tx_data;
    i2c_transfer_cfg.tx_length = 1;
    i2c_transfer_cfg.rx_data = NULL;
    i2c_transfer_cfg.rx_length = 0;
    i2c_transfer_cfg.retransmissions_max = 0;

    I2C_MasterTransferData(eeprom->I2Cx, &i2c_transfer_cfg, I2C_TRANSFER_POLLING);
    for (int i=0; i<10000; i++);

    i2c_transfer_cfg.tx_data = NULL;
    i2c_transfer_cfg.tx_length = 0;
    i2c_transfer_cfg.rx_data = rx_data;
    i2c_transfer_cfg.rx_length = size;
    i2c_transfer_cfg.retransmissions_max = 0;

    I2C_MasterTransferData(eeprom->I2Cx, &i2c_transfer_cfg, I2C_TRANSFER_POLLING);
    for (int i=0; i<10000; i++);

    if (dest != NULL) {
        for (uint32_t i=0; i<size; i++)
            *(dest + i) = *(rx_data + i);
    }
}


void init_eeprom(AT24C_t *eeprom, EEPROM_t mem_type, uint8_t dev_Addr, LPC_I2C_TypeDef *I2Cx)
{
	eeprom->I2Cx = I2Cx;
	eeprom->dev_addr = dev_Addr;
	eeprom->eeprom_type = mem_type;

    eeprom->write_byte = write_byte;
    eeprom->write_half_word = write_half_word;
    eeprom->write_word = write_word;
    eeprom->write_data = write_data;
    eeprom->read_byte = read_byte;
    eeprom->read_half_word = read_half_word;
    eeprom->read_word = read_word;
    eeprom->read_data = read_data;
}
