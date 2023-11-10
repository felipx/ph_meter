/*
 * lcd5110.h
 *
 *  Created on: Oct 22, 2023
 *      Author: felipe
 */

#ifndef LCD5110_H_
#define LCD5110_H_

#include <lpc17xx_ssp.h>

#define DC (1 << 2)
#define RST (1 << 3)


typedef struct {

	void (*set_cursor) (unsigned char row, unsigned char col);
	void (*clear) (void);
	void (*send_cmd) (char cmd);
	void (*print_str) (const char str[]);
	void (*print_big_str) (const char str[]);
	void (*set_inverse) (uint8_t inv);
	void (*reset) (void);

} LCD5110_t;

void init_lcd5110(LCD5110_t *lcd5110, LPC_SSP_TypeDef* SSPx);


#endif /* LCD5110_H_ */
