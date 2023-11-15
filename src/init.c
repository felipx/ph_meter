/*
 * init.c
 *
 *  Created on: Nov 14, 2023
 *      Author: felipe
 */

#include <stdio.h>
#include <lpc17xx_i2c.h>
#include <lpc17xx_timer.h>

#include "init.h"
#include "calibration.h"
#include "eeprom.h"
#include "fsm.h"


void init_init(void)
{
    I2C_Cmd(LPC_I2C1, ENABLE);
}


void exit_init(void)
{
    I2C_Cmd(LPC_I2C1, DISABLE);
}


void init(void)
{
    AT24C_t *at24c08 = (AT24C_t *) AT24C08_ADDR;

    at24c08->read_word(at24c08, (uint8_t *) &slope, 0x00);
    at24c08->read_word(at24c08, (uint8_t *) &offset, 0x04);

    event = EV_INIT_COMPLETE;
}
