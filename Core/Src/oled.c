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


