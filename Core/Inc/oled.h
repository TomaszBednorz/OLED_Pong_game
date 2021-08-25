#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "main.h"

/*
 * Defines helpful as function parameters
 */

// Color of pixel, param: @OLED_COLOR
#define OLED_COLOR_BLACK		SSD_1306_COLOR_BLACK
#define OLED_COLOR_WHITE		SSD_1306_COLOR_WHITE

/*
 *  Function prototypes
 */
void oled_draw_char(uint8_t x, uint8_t y, char ch, uint8_t scale, uint8_t color);
void oled_draw_string(uint8_t x, uint8_t y, char *str, uint8_t scale, uint8_t color);


#endif /* INC_OLED_H_ */
