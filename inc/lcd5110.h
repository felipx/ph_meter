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

#define LCD5110_ADDR (0x20080000)


typedef struct {
    unsigned char row; // 6 rows
    unsigned char col; // 14 cols
} coords_t;


typedef struct display LCD5110_t;

typedef struct display {

	LPC_SSP_TypeDef* SSPx;
	uint8_t inverse;
	coords_t coords;
	void (*set_cursor) (LCD5110_t *lcd5110, unsigned char row, unsigned char col);
	void (*clear) (LCD5110_t *lcd5110);
	void (*send_cmd) (LCD5110_t *lcd5110, char cmd);
	void (*print_str) (LCD5110_t *lcd5110, const char str[]);
	void (*print_big_str) (LCD5110_t *lcd5110, const char str[]);
	void (*set_inverse) (LCD5110_t *lcd5110, uint8_t inv);
	void (*reset) (void);

} LCD5110_t;

void init_lcd5110(LCD5110_t *lcd5110, LPC_SSP_TypeDef* SSPx);


#endif /* LCD5110_H_ */
