#include "oled.h"




/*
 * @fn      		  - oled_draw_char
 *
 * @param[in]         - x - start x axis position from 0 to 127
 * @param[in]         - y - start y axis position from 0 to 63
 * @param[in]         - ch - char in ASCII standard
 * @param[in]         - scale - in scale 1 single char is 5x7
 * @param[in]         - color - param: @OLED_COLOR
 *
 * @return            - None
 *
 * @Note              - This function draw string in buffer of display.
 */
void oled_draw_char(uint8_t x, uint8_t y, char ch, uint8_t scale, uint8_t color)
{
	int iterator = (int)ch * FONT_WIDTH;


	for(int i = iterator; i < iterator + 5; i++)
	{
		ssd1306_draw_byte_vertical(x, y, font5x7[i], scale, color);
		x += scale;
	}


}

/*
 * @fn      		  - oled_draw_string
 *
 * @param[in]         - x - start x axis position from 0 to 127
 * @param[in]         - y - start y axis position from 0 to 63
 * @param[in]         - str - string in ASCII standard
 * @param[in]         - scale - in scale 1 single char is 5x7
 * @param[in]         - color - param: @OLED_COLOR
 *
 * @return            - None
 *
 * @Note              - This function draw string in buffer of display.
 */
void oled_draw_string(uint8_t x, uint8_t y, char *str, uint8_t scale, uint8_t color)
{
	for(int i = 0; i < strlen(str); i++)
	{
		oled_draw_char(x, y, str[i], scale, color);
		x += scale * FONT_WIDTH + 1; // FONT_WIDTH is width of char + 1 line off
	}
}

/*
 * @fn      		  - oled_draw_string
 *
 * @param[in]         - x - start x axis position from 0 to 127
 * @param[in]         - y - start y axis position from 0 to 63
 * @param[in]         - scale - in scale 1 arrow is 5x5
 * @param[in]         - direction -  param: @OLED_ARROW_DIRECTION
 * @param[in]         - color - param: @OLED_COLOR
 *
 * @return            - None
 *
 * @Note              - This function draw arrow in buffer of display. (x, y) are a top of arrow
 */
void oled_draw_arrow(uint8_t x, uint8_t y, uint8_t scale, uint8_t direction, uint8_t color)
{
	if(direction == OLED_ARROW_UP)
	{
		ssd1306_draw_rectangle(x, y, scale, scale, color);
		ssd1306_draw_rectangle(x - scale, y + scale, scale * 3, scale, color);
		ssd1306_draw_rectangle(x - 2 * scale, y + 2 * scale, scale * 5, scale, color);
		ssd1306_draw_rectangle(x, y + 3 * scale, scale, scale * 2, color);
	}
	else if(direction == OLED_ARROW_DOWN)
	{
		ssd1306_draw_rectangle(x, y, scale, scale, color);
		ssd1306_draw_rectangle(x - scale, y - scale, scale * 3, scale, color);
		ssd1306_draw_rectangle(x - 2 * scale, y - 2 * scale, scale * 5, scale, color);
		ssd1306_draw_rectangle(x, y - 4 * scale, scale, scale * 2, color);
	}
	else if(direction == OLED_ARROW_LEFT)
	{
		ssd1306_draw_rectangle(x, y, scale, scale, color);
		ssd1306_draw_rectangle(x + scale, y - scale, scale, scale * 3, color);
		ssd1306_draw_rectangle(x + 2 * scale, y - 2 * scale, scale, scale * 5, color);
		ssd1306_draw_rectangle(x + 3 * scale, y, scale * 2, scale, color);
	}
	else if(direction == OLED_ARROW_RIGHT)
	{
		ssd1306_draw_rectangle(x, y, scale, scale, color);
		ssd1306_draw_rectangle(x - scale, y - scale, scale, scale * 3, color);
		ssd1306_draw_rectangle(x - 2 * scale, y - 2 * scale, scale, scale * 5, color);
		ssd1306_draw_rectangle(x - 4 * scale, y, scale * 2, scale, color);
	}


}







