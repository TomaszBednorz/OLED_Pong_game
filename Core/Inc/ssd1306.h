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

/*
 *  Commands from SSD1306 driver datasheet
 */

// Fundamental command
#define SSD1306_SET_CONTRAST_COLOR		0x81

#define SSD1306_RAM_CONTENT_DISPLAY		0xA4
#define SSD1306_ENTIRE_DISPLAY_ON		0x05

#define SSD1306_SET_NORMAL_DISPLAY		0xA6
#define SSD1306_SET_INVERSE_DISPLAY		0xA7

#define SSD1306_SET_DISPLAY_OFF			0xAE
#define SSD1306_SET_DISPLAY_ON			0xAF

// Scrolling command






/*
 *  Function prototypes
 */
bool ssd1306_init(void);




#endif /* INC_SSD1306_H_ */
