
#include <lpc17xx_gpio.h>
#include "lcd5110.h"

#define LCD_WIDTH 84
#define LCD_HEIGHT 48

const char ascii_table[][5] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00 }, // 0x20, space
    { 0x00, 0x00, 0x5f, 0x00, 0x00 }, // 0x21, !
    { 0x00, 0x07, 0x00, 0x07, 0x00 }, // 0x22, "
    { 0x14, 0x7f, 0x14, 0x7f, 0x14 }, // 0x23, #
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12 }, // 0x24, $
    { 0x23, 0x12, 0x08, 0x64, 0x62 }, // 0x25, %
    { 0x36, 0x49, 0x55, 0x22, 0x50 }, // 0x26, &
    { 0x00, 0x05, 0x03, 0x00, 0x00 }, // 0x27, '
    { 0x00, 0x1c, 0x22, 0x41, 0x00 }, // 0x28, (
    { 0x00, 0x41, 0x22, 0x1c, 0x00 }, // 0x29, )
    { 0x14, 0x08, 0x3E, 0x08, 0x14 }, // 0x2a, *
    { 0x08, 0x08, 0x3E, 0x08, 0x08 }, // 0x2b, +
    { 0x00, 0x50, 0x30, 0x00, 0x00 }, // 0x2c, ,
    { 0x08, 0x08, 0x08, 0x08, 0x08 }, // 0x2d, -
    { 0x00, 0x60, 0x60, 0x00, 0x00 }, // 0x2e, .
    { 0x20, 0x10, 0x08, 0x04, 0x02 }, // 0x2f, /
    { 0x3E, 0x51, 0x49, 0x45, 0x3E }, // 0x30, 0
    { 0x00, 0x42, 0x7F, 0x40, 0x00 }, // 0x31, 1
    { 0x42, 0x61, 0x51, 0x49, 0x46 }, // 0x32, 2
    { 0x21, 0x41, 0x45, 0x4B, 0x31 }, // 0x33, 3
    { 0x18, 0x14, 0x12, 0x7F, 0x10 }, // 0x34, 4
    { 0x27, 0x45, 0x45, 0x45, 0x39 }, // 0x35, 5
    { 0x3C, 0x4A, 0x49, 0x49, 0x30 }, // 0x36, 6
    { 0x01, 0x71, 0x09, 0x05, 0x03 }, // 0x37, 7
    { 0x36, 0x49, 0x49, 0x49, 0x36 }, // 0x38, 8
    { 0x06, 0x49, 0x49, 0x29, 0x1E }, // 0x39, 9
    { 0x00, 0x36, 0x36, 0x00, 0x00 }, // 0x3a, :
    { 0x00, 0x56, 0x36, 0x00, 0x00 }, // 0x3b, ;
    { 0x08, 0x14, 0x22, 0x41, 0x00 }, // 0x3c, <
    { 0x14, 0x14, 0x14, 0x14, 0x14 }, // 0x3d, =
    { 0x00, 0x41, 0x22, 0x14, 0x08 }, // 0x3e, >
    { 0x02, 0x01, 0x51, 0x09, 0x06 }, // 0x3f, ?
    { 0x32, 0x49, 0x59, 0x51, 0x3E }, // 0x40, @
    { 0x7E, 0x11, 0x11, 0x11, 0x7E }, // 0x41, A
    { 0x7F, 0x49, 0x49, 0x49, 0x36 }, // 0x42, B
    { 0x3E, 0x41, 0x41, 0x41, 0x22 }, // 0x43, C
    { 0x7F, 0x41, 0x41, 0x22, 0x1C }, // 0x44, D
    { 0x7F, 0x49, 0x49, 0x49, 0x41 }, // 0x45, E
    { 0x7F, 0x09, 0x09, 0x09, 0x01 }, // 0x46, F
    { 0x3E, 0x41, 0x49, 0x49, 0x7A }, // 0x47, G
    { 0x7F, 0x08, 0x08, 0x08, 0x7F }, // 0x48, H
    { 0x00, 0x41, 0x7F, 0x41, 0x00 }, // 0x49, I
    { 0x20, 0x40, 0x41, 0x3F, 0x01 }, // 0x4a, J
    { 0x7F, 0x08, 0x14, 0x22, 0x41 }, // 0x4b, K
    { 0x7F, 0x40, 0x40, 0x40, 0x40 }, // 0x4c, L
    { 0x7F, 0x02, 0x0C, 0x02, 0x7F }, // 0x4d, M
    { 0x7F, 0x04, 0x08, 0x10, 0x7F }, // 0x4e, N
    { 0x3E, 0x41, 0x41, 0x41, 0x3E }, // 0x4f, O
    { 0x7F, 0x09, 0x09, 0x09, 0x06 }, // 0x50, P
    { 0x3E, 0x41, 0x51, 0x21, 0x5E }, // 0x51, Q
    { 0x7F, 0x09, 0x19, 0x29, 0x46 }, // 0x52, R
    { 0x46, 0x49, 0x49, 0x49, 0x31 }, // 0x53, S
    { 0x01, 0x01, 0x7F, 0x01, 0x01 }, // 0x54, T
    { 0x3F, 0x40, 0x40, 0x40, 0x3F }, // 0x55, U
    { 0x1F, 0x20, 0x40, 0x20, 0x1F }, // 0x56, V
    { 0x3F, 0x40, 0x38, 0x40, 0x3F }, // 0x57, W
    { 0x63, 0x14, 0x08, 0x14, 0x63 }, // 0x58, X
    { 0x07, 0x08, 0x70, 0x08, 0x07 }, // 0x59, Y
    { 0x61, 0x51, 0x49, 0x45, 0x43 }, // 0x5a, Z
    { 0x00, 0x7F, 0x41, 0x41, 0x00 }, // 0x5b, [
    { 0x55, 0x2A, 0x55, 0x2A, 0x55 }, // 0x5c, back slash
    { 0x00, 0x41, 0x41, 0x7F, 0x00 }, // 0x5d, ]
    { 0x04, 0x02, 0x01, 0x02, 0x04 }, // 0x5e, ^
    { 0x40, 0x40, 0x40, 0x40, 0x40 }, // 0x5f, _
    { 0x00, 0x01, 0x02, 0x04, 0x00 }, // 0x60, `
    { 0x20, 0x54, 0x54, 0x54, 0x78 }, // 0x61, a
    { 0x7F, 0x48, 0x44, 0x44, 0x38 }, // 0x62, b
    { 0x38, 0x44, 0x44, 0x44, 0x20 }, // 0x63, c
    { 0x38, 0x44, 0x44, 0x48, 0x7F }, // 0x64, d
    { 0x38, 0x54, 0x54, 0x54, 0x18 }, // 0x65, e
    { 0x08, 0x7E, 0x09, 0x01, 0x02 }, // 0x66, f
    { 0x0C, 0x52, 0x52, 0x52, 0x3E }, // 0x67, g
    { 0x7F, 0x08, 0x04, 0x04, 0x78 }, // 0x68, h
    { 0x00, 0x44, 0x7D, 0x40, 0x00 }, // 0x69, i
    { 0x20, 0x40, 0x44, 0x3D, 0x00 }, // 0x6a, j
    { 0x7F, 0x10, 0x28, 0x44, 0x00 }, // 0x6b, k
    { 0x00, 0x41, 0x7F, 0x40, 0x00 }, // 0x6c, l
    { 0x7C, 0x04, 0x18, 0x04, 0x78 }, // 0x6d, m
    { 0x7C, 0x08, 0x04, 0x04, 0x78 }, // 0x6e, n
    { 0x38, 0x44, 0x44, 0x44, 0x38 }, // 0x6f, o
    { 0x7C, 0x14, 0x14, 0x14, 0x08 }, // 0x70, p
    { 0x08, 0x14, 0x14, 0x18, 0x7C }, // 0x71, q
    { 0x7C, 0x08, 0x04, 0x04, 0x08 }, // 0x72, r
    { 0x48, 0x54, 0x54, 0x54, 0x20 }, // 0x73, s
    { 0x04, 0x3F, 0x44, 0x40, 0x20 }, // 0x74, t
    { 0x3C, 0x40, 0x40, 0x20, 0x7C }, // 0x75, u
    { 0x1C, 0x20, 0x40, 0x20, 0x1C }, // 0x76, v
    { 0x3C, 0x40, 0x30, 0x40, 0x3C }, // 0x77, w
    { 0x44, 0x28, 0x10, 0x28, 0x44 }, // 0x78, x
    { 0x0C, 0x50, 0x50, 0x50, 0x3C }, // 0x79, y
    { 0x44, 0x64, 0x54, 0x4C, 0x44 }, // 0x7a, z
    { 0x00, 0x08, 0x36, 0x41, 0x00 }, // 0x7b, {
    { 0x00, 0x00, 0x7f, 0x00, 0x00 }, // 0x7c, |
    { 0x00, 0x41, 0x36, 0x08, 0x00 }, // 0x7d, }
    { 0x10, 0x08, 0x08, 0x10, 0x08 }, // 0x7e, ~
    { 0x78, 0x46, 0x41, 0x46, 0x78 }  // 0x7f, DEL
};


static const char ascii_table_11x16_top[][11] = {
    {0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00}, // -
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // .
    {0x00,0x00,0x00,0x00,0x00,0xC0,0xE0,0x78,0x1C,0x0F,0x03}, // /
    {0xFE,0xFF,0x03,0x03,0x03,0xC3,0x33,0x0B,0x07,0xFF,0xFE}, // 0
    {0x00,0x03,0x03,0x03,0x03,0xFF,0xFF,0x00,0x00,0x00,0x00}, // 1
    {0x03,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0xFF,0xFE}, // 2
    {0x00,0x03,0x03,0x83,0x83,0x83,0x83,0x83,0x83,0xFF,0xFE}, // 3
    {0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFC,0xFC,0x00}, // 4
    {0x00,0x00,0xFF,0xFF,0x83,0x83,0x83,0x83,0x83,0x83,0x03}, // 5
    {0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x07,0x06,0x00}, // 6
    {0x07,0x07,0x03,0x03,0x03,0x03,0x83,0x83,0xC3,0x7F,0x7F}, // 7
    {0xFE,0xFF,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0xFF,0xFE}, // 8
    {0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xFF,0xFF}  // 9
};

static const char ascii_table_11x16_bottom[][11] = {
    {0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00}, // -
    {0x00,0x00,0x00,0x00,0x0E,0x0E,0x0E,0x0E,0x00,0x00,0x00}, // .
    {0x00,0x60,0x78,0x3C,0x0F,0x03,0x01,0x00,0x00,0x00,0x00}, // /
    {0x3F,0x7F,0x70,0x6C,0x63,0x60,0x60,0x60,0x60,0x7F,0x3F}, // 0
    {0x00,0x60,0x60,0x60,0x60,0x7F,0x7F,0x60,0x60,0x60,0x60}, // 1
    {0x7F,0x7F,0x61,0x61,0x61,0x61,0x61,0x61,0x61,0x61,0x60}, // 2
    {0x00,0x60,0x60,0x61,0x61,0x61,0x61,0x61,0x61,0x7F,0x3F}, // 3
    {0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x7F,0x7F,0x03}, // 4
    {0x00,0x38,0x79,0x61,0x61,0x61,0x61,0x61,0x61,0x7F,0x3F}, // 5
    {0x7F,0x7F,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x7F,0x7F}, // 6
    {0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x01,0x00,0x00,0x00}, // 7
    {0x3F,0x7F,0x61,0x61,0x61,0x61,0x61,0x61,0x61,0x7F,0x3F}, // 8
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F}  // 9
};


typedef enum {
	CMD,
	DATA
} CMD_t;


typedef struct {
	unsigned char row; // 6 rows
	unsigned char col; // 14 cols
} coords_t;

coords_t coords;


static struct lcd5110_cfg {
	LPC_SSP_TypeDef* SSPx;
	uint8_t inverse;
} lcd5110_cfg;


//static coords_t get_cursor(void)
//{
//	return coords;
//}


static void delay(void)
{
	for (int i=0; i<200; i++);
}


static void write(CMD_t cmd, uint8_t data)
{
    if (cmd == CMD)
    	GPIO_ClearValue(0, DC);
    else if (cmd == DATA) {
        GPIO_SetValue(0, DC);
        if (lcd5110_cfg.inverse)
        	data = ~data;
    }
    else
    	return;

    SSP_SendData(lcd5110_cfg.SSPx, (uint16_t)(data & 0x00FF));
}


static void send_cmd(char cmd)
{
	write(CMD, cmd);
	delay();
}


static void set_cursor(unsigned char row, unsigned char col)
{
    if ((row < 0) | (row > LCD_HEIGHT / 8) | (col < 0) | (col > LCD_WIDTH / 6))
        return;
    send_cmd(0x40 | row);
    send_cmd(0x80 | col*6);

    coords.row = row;
    coords.col = col;
}


static void clear(void)
{
    set_cursor(0,0);
    for (int pixel = (LCD_WIDTH * LCD_HEIGHT / 8); pixel > 0; pixel--) {
    	write(DATA, 0x00);
    	delay();
    }
}


static void print_str(const char str[])
{
    unsigned int index = 0;
    while (str[index] != '\0') {
        if ((str[index] >= 0x20) & (str[index] <= 0x7F)) {
            for (int i = 0; i < 5; i++) {
            	write(DATA, ascii_table[str[index] - 0x20][i]);
                delay();
            }
            write(DATA, 0x00);
            delay();
        }
        index++;
    }
}


static void print_big_str(const char str[])
{
    unsigned int index = 0;
    while (str[index] != '\0') {
        if ((str[index] >= 0x2D) & (str[index] <= 0x39)) {
            for (int i = 0; i < 11; i++) {
            	write(DATA, ascii_table_11x16_top[str[index] - 0x2D][i]);
                delay();
            }
            write(DATA, 0x00);
            delay();
        }
        index++;
    }

    set_cursor(coords.row + 1, coords.col);

    index = 0;
    while (str[index] != '\0') {
        if ((str[index] >= 0x2D) & (str[index] <= 0x39)) {
            for (int i = 0; i < 11; i++) {
            	write(DATA, ascii_table_11x16_bottom[str[index] - 0x2D][i]);
                delay();
            }
            write(DATA, 0x00);
            delay();
        }
        index++;
    }
}


static void set_inverse(uint8_t inv)
{
	if (inv)
	    lcd5110_cfg.inverse = 1;
	else
		lcd5110_cfg.inverse = 0;
}


static void reset(void)
{
	GPIO_ClearValue(0, RST);
	for (int i=0; i<10000; i++);
	GPIO_SetValue(0, RST);
}


void init_lcd5110(LCD5110_t *lcd5110, LPC_SSP_TypeDef* SSPx)
{
	if (SSPx == LPC_SSP0 || SSPx == LPC_SSP1) {
		lcd5110_cfg.SSPx = SSPx;
	}
	else
		return;

	lcd5110->clear = clear;
	lcd5110->set_cursor = set_cursor;
	lcd5110->print_str = print_str;
	lcd5110->print_big_str = print_big_str;
	lcd5110->send_cmd = send_cmd;
	lcd5110->set_inverse = set_inverse;
	lcd5110->reset = reset;

	lcd5110->set_inverse(0);
	lcd5110->reset();
	lcd5110->send_cmd(0x21);
	lcd5110->send_cmd(0xB2);
	lcd5110->send_cmd(0x20);
	lcd5110->send_cmd(0x0C);
	lcd5110->clear();
}
