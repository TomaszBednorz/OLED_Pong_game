#ifndef INC_SSD1306_H_
#define INC_SSD1306_H_

#include "main.h"

/*
 *  Extern variables needed in SSD1306 driver
 */
extern I2C_HandleTypeDef hi2c1;

/*
 *  Configurable items section
 */
#define SSD1306_I2C_ADDRESS			(0x3C << 1)
#define SSD1306_I2C_HANDLE			hi2c1
#define SSD1306_I2C_MAX_DELAY		100

/*
 *  Defines helpful in write mode for I2C in SSD1306 (refer to SSD1306 datasheet)
 */
#define SSD1306_CTRL_BYTE_COMM		0x00
#define SSD1306_CTRL_BYTE_DATA		0x40
#define SSD1306_MEM_ADDR_SIZE		1
#define SSD1306_COMM_DATA_SIZE		1

/*
 *  Helpful defines connected with display
 */

#define SSD1306_COLUMNS				128
#define SSD1306_ROWS				64
#define SSD1306_PAGES				8

/*
 *  Commands from SSD1306 driver datasheet
 */

// Fundamental commands
#define SSD1306_SET_CONTRAST_COLOR			0x81

#define SSD1306_RAM_CONTENT_DISPLAY			0xA4
#define SSD1306_ENTIRE_DISPLAY_ON			0x05

#define SSD1306_SET_NORMAL_DISPLAY			0xA6
#define SSD1306_SET_INVERSE_DISPLAY			0xA7

#define SSD1306_SET_DISPLAY_OFF				0xAE
#define SSD1306_SET_DISPLAY_ON				0xAF

// Scrolling commands
#define SSD1306_RIGHT_HORIZONTAL_SCROLL		0x26
#define SSD1306_LEFT_HOTIZONTAL_SCROLL		0x27

#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL	0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL		0x2A

#define SSD1306_DEACTIVE_SCROLL				0x2E
#define SSD1306_ACTIVE_SCROLL				0x2F

#define SSD1306_SET_VERTICAL_SCROLL_AREA	0xA3

// Addressing setting commands
#define SSD1306_SET_LOWER_COLUMN			0x00
#define SSD1306_SET_HIGHER_COLUMN			0x10
#define SSD1306_SET_MEMORY_MODE				0x20
#define SSD1306_SET_COLUMN_ADDRESS			0x21
#define SSD1306_SET_PAGE_ADDRESS			0x22

// Hardware configuration commands
#define SSD1306_SET_DISPLAY_START_LINE		0x40
#define SSD1306_COL0_MAPPED_SEG0			0xA0
#define SSD1306_COL0_MAPPED_SEG127			0xA1
#define SSD1306_SET_MULTIPLEX_RATIO			0xA8
#define SSD1306_NORMAL_MODE					0xC0
#define SSD1306_REMAPPED_MODE				0xC3
#define SSD1306_SET_DISPLAY_OFFSET			0xD3
#define SSD1306_SET_COM_PINS				0xDA

// Timing & Driving scheme setting commands
#define SSD1306_SET_DISPLAY_CLOCK_DIV		0xD5
#define SSD1306_SET_PRE_CHARGE_PERIOD		0xD9
#define SSD1306_SET_V_COMH_DESELECT_LEVEL	0xDB
#define SSD1306_NO_OPERATION				0xE3

// Charge Bump Setting commands
#define SSD1306_CHARGE_PUMP_SETTING			0x8D
#define SSD1306_ENABLE_CHARGE_PUMP			0x14


/*
 * Defines helpful as function parameters
 */

// Color of pixel, param: @SSD1306_COLOR
#define SSD_1306_COLOR_BLACK	0
#define SSD_1306_COLOR_WHITE	1


/*
 *  Function prototypes
 */
void ssd1306_init(void);
void ssd1306_display_clear(void);
void ssd1306_display_show(void);
void ssd1306_draw_pixel(uint8_t x, uint8_t y, uint8_t color);
void ssd1306_horizontal_line(uint8_t x, uint8_t y, uint8_t length, uint8_t color);
void ssd1306_vertical_line(uint8_t x, uint8_t y, uint8_t length, uint8_t color);
void ssd1306_draw_rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
void draw_byte_vertical(uint8_t x, uint8_t y, uint8_t byte, uint8_t scale, uint8_t color);
uint8_t* ssd1306_get_buffer(void);
#endif /* INC_SSD1306_H_ */
