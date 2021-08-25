#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "main.h"

/*
 * Defines helpful as function parameters
 */

// Color of pixel, param: @OLED_COLOR
#define OLED_COLOR_BLACK		SSD_1306_COLOR_BLACK
#define OLED_COLOR_WHITE		SSD_1306_COLOR_WHITE

// Direction of arrow, param: @OLED_ARROW_DIRECTION
#define OLED_ARROW_UP			1
#define OLED_ARROW_DOWN			2
#define OLED_ARROW_LEFT			3
#define OLED_ARROW_RIGHT		4

/*
 *  Function prototypes
 */
void oled_draw_char(uint8_t x, uint8_t y, char ch, uint8_t scale, uint8_t color);
void oled_draw_string(uint8_t x, uint8_t y, char *str, uint8_t scale, uint8_t color);
void oled_draw_arrow(uint8_t x, uint8_t y, uint8_t scale, uint8_t direction, uint8_t color);


#endif /* INC_OLED_H_ */
