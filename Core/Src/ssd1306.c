#include "ssd1306.h"


// Buffer contains display content
uint8_t buffer[SSD1306_COLUMNS * SSD1306_PAGES];

/*	    OLED DISPLAY (x,y)
 *   ---------- ... ----------
 *  |(0,0)			   (127,0)|
 * 	|						  |
 * 	.						  .
 * 	.						  .
 * 	.						  .
 *  |						  |
 *  |(0,63)			  (127,63)|
 *   ---------- ... ----------
 */



/*
 *  Static functions prototypes
 */

void ssd1306_write_command(uint8_t command);
void ssd1306_write_data(uint8_t *data, uint16_t length);

/*
 *  Function definitions
 */

/*
 * @fn      		  - ssd1306_init
 *
 * @return            - None
 *
 * @Note              - This is SSD1306 initialization function
 */
void ssd1306_init(void)
{
	ssd1306_display_clear();

	// Display Off
	ssd1306_write_command(SSD1306_SET_DISPLAY_OFF);

	// Set Display Clock Div
	ssd1306_write_command(SSD1306_SET_DISPLAY_CLOCK_DIV);
	ssd1306_write_command(0xE0);

	// Set MUX Ratio
	ssd1306_write_command(SSD1306_SET_MULTIPLEX_RATIO);
	ssd1306_write_command(SSD1306_ROWS - 1);

	// Set Display Offset
	ssd1306_write_command(SSD1306_SET_DISPLAY_OFFSET);
	ssd1306_write_command(0x00);

	// Set Display Start Line
	ssd1306_write_command(SSD1306_SET_DISPLAY_START_LINE);

	// Set Segment re-map
	ssd1306_write_command(SSD1306_COL0_MAPPED_SEG0);

	// Set COM Output Scan Direction
	ssd1306_write_command(SSD1306_NORMAL_MODE);

	// Set COM Pins hardware configuration
	ssd1306_write_command(SSD1306_SET_COM_PINS);
	ssd1306_write_command(0x12);

	// Set Contrast Color
	ssd1306_write_command(SSD1306_SET_CONTRAST_COLOR);
	ssd1306_write_command(0x7F);

	// Disable Entire Display On
	ssd1306_write_command(SSD1306_RAM_CONTENT_DISPLAY);

	// Set Normal Display
	ssd1306_write_command(SSD1306_SET_NORMAL_DISPLAY);

	// Set Memory Addressing Mode
	ssd1306_write_command(SSD1306_SET_MEMORY_MODE);
	ssd1306_write_command(0x00);

	// Enable charge pump regulator
	ssd1306_write_command(SSD1306_CHARGE_PUMP_SETTING);
	ssd1306_write_command(SSD1306_ENABLE_CHARGE_PUMP);

	// Display On
	ssd1306_write_command(SSD1306_SET_DISPLAY_ON);

}

/*
 * @fn      		  - ssd1306_display_clear
 *
 * @return            - None
 *
 * @Note              - This function clears buffer of display
 */
void ssd1306_display_clear(void)
{
	memset(buffer, 0, sizeof(buffer));
}

/*
 * @fn      		  - ssd1306_display_show
 *
 * @return            - None
 *
 * @Note              - This function actualizes  content of display
 */
void ssd1306_display_show(void)
{
	ssd1306_write_command(SSD1306_SET_COLUMN_ADDRESS);
	ssd1306_write_command(0x00);
	ssd1306_write_command(SSD1306_COLUMNS - 1);

	ssd1306_write_command(SSD1306_SET_PAGE_ADDRESS);
	ssd1306_write_command(0x00);
	ssd1306_write_command(SSD1306_PAGES - 1);

	ssd1306_write_data(buffer, sizeof(buffer));
}

/*
 * @fn      		  - ssd1306_draw_pixel
 *
 * @param[in]         - x - place in display from 0 to 127
 * @param[in]         - y - place in display from 0 to 63
 * @param[in]         - color - param: @SSD1306_COLOR
 *
 * @return            - None
 *
 * @Note              - This function draw pixels in buffer of display.
 */
void ssd1306_draw_pixel(uint8_t x, uint8_t y, uint8_t color)
{
	uint8_t page, cell;

	if(x < SSD1306_COLUMNS && y < SSD1306_ROWS)
	{
		page = y / 8;
		cell = buffer[SSD1306_COLUMNS * page + x];

		if(color == SSD_1306_COLOR_BLACK)
		{
			cell &= ~(1 << (y % 8));
		}
		else if(color == SSD_1306_COLOR_WHITE)
		{
			cell |= (1 << (y % 8));
		}

		buffer[SSD1306_COLUMNS * page + x] = cell;
	}
}

/*
 * @fn      		  - ssd1306_horizontal_line
 *
 * @param[in]         - x - start x axis position from 0 to 127
 * @param[in]         - y - start y axis position from 0 to 63
 * @param[in]         - length - length of horizontal line
 * @param[in]         - color - param: @SSD1306_COLOR
 *
 * @return            - None
 *
 * @Note              - This function draw horizontal line in buffer of display. x + length <= 128
 */
void ssd1306_horizontal_line(uint8_t x, uint8_t y, uint8_t length, uint8_t color)
{
	uint8_t page, cell;
	uint16_t cell_idx;

	if(x + length <= SSD1306_COLUMNS && y < SSD1306_ROWS && x < SSD1306_COLUMNS)
	{
		page = y / 8;
		cell_idx = SSD1306_COLUMNS * page + x;

		for(int i = 0; i < length; i++)
		{
			cell = buffer[cell_idx];

			if(color == SSD_1306_COLOR_BLACK)
			{
				cell &= ~(1 << (y % 8));
			}
			else if(color == SSD_1306_COLOR_WHITE)
			{
				cell |= (1 << (y % 8));
			}

			buffer[cell_idx] = cell;
			cell_idx++;
		}
	}
}

/*
 * @fn      		  - ssd1306_vertical_line
 *
 * @param[in]         - x - start x axis position from 0 to 127
 * @param[in]         - y - start y axis position from 0 to 63
 * @param[in]         - length - length of vertical line
 * @param[in]         - color - param: @SSD1306_COLOR
 *
 * @return            - None
 *
 * @Note              - This function draw vertical line in buffer of display. y + length <= 64
 */
void ssd1306_vertical_line(uint8_t x, uint8_t y, uint8_t length, uint8_t color)
{
	uint8_t page, cell, cell_pos;
	uint16_t cell_idx;

	if(y + length <= SSD1306_ROWS && y < SSD1306_ROWS && x < SSD1306_COLUMNS)
	{
		page = y / 8;
		cell_idx = SSD1306_COLUMNS * page + x;
		cell_pos = y % 8;

		for(int i = 0; i < length; i++)
		{
			cell = buffer[cell_idx];

			if(color == SSD_1306_COLOR_BLACK)
			{
				cell &= ~(1 << (cell_pos));
			}
			else if(color == SSD_1306_COLOR_WHITE)
			{
				cell |= (1 << (cell_pos));
			}

			buffer[cell_idx] = cell;

			cell_pos++;
			if(cell_pos >= 8)
			{
				cell_pos = 0;
				cell_idx += 128;
			}
		}
	}
}

/*
 * @fn      		  - ssd1306_draw_rectangle
 *
 * @param[in]         - x - start x axis position from 0 to 127
 * @param[in]         - y - start y axis position from 0 to 63
 * @param[in]         - width - width of rectangle
 * @param[in]         - height - height of rectangle
 * @param[in]         - color - param: @SSD1306_COLOR
 *
 * @return            - None
 *
 * @Note              - This function draw rectangle in buffer of display. y + height <= 64, x + width <= 128
 */
void ssd1306_draw_rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
	if(x + width <= SSD1306_COLUMNS && y + height <= SSD1306_ROWS && x < SSD1306_COLUMNS && y < SSD1306_ROWS)
	{
		for(int i = y; i < y + height; i++)
		{
			if(color == SSD_1306_COLOR_BLACK)
			{
				ssd1306_horizontal_line(x, i, width, SSD_1306_COLOR_BLACK);
			}
			else if(color == SSD_1306_COLOR_WHITE)
			{
				ssd1306_horizontal_line(x, i, width, SSD_1306_COLOR_WHITE);
			}
		}

	}
}

/*
 * @fn      		  - draw_byte_vertical
 *
 * @param[in]         - x - start x axis position from 0 to 127
 * @param[in]         - y - start y axis position from 0 to 63
 * @param[in]         - byte - byte to draw (8pixels)
 * @param[in]         - scale - e. g. in scale 2 pixel is 2x2
 * @param[in]         - color - param: @SSD1306_COLOR
 *
 * @return            - None
 *
 * @Note              - This function draw string in buffer of display.
 */
void draw_byte_vertical(uint8_t x, uint8_t y, uint8_t byte, uint8_t scale, uint8_t color)
{
	uint8_t page, cell, cell_pos;
	uint16_t cell_idx;

	page = y / 8;
	cell_idx = SSD1306_COLUMNS * page + x;
	cell_pos = y % 8;

	uint8_t mask = 0x01;

	if(scale == 1)
	{
		while(mask != 0x00)
		{
			cell = buffer[cell_idx];

			if(byte & mask)
			{
				if(color == SSD_1306_COLOR_BLACK)
				{
					cell &= ~(1 << (cell_pos));
				}
				else if(color == SSD_1306_COLOR_WHITE)
				{
					cell |= (1 << (cell_pos));
				}
			}
			else
			{
				if(color == SSD_1306_COLOR_BLACK)
				{
					cell |= (1 << (cell_pos));
				}
				else if(color == SSD_1306_COLOR_WHITE)
				{
					cell &= ~(1 << (cell_pos));
				}
			}

			buffer[cell_idx] = cell;

			cell_pos++;

			if(cell_pos >= 8)
			{
				cell_pos = 0;
				cell_idx += 128;
			}

			mask = mask << 1;

		}
	}
	else
	{
		while(mask != 0x00)
		{
			if(byte & mask)
			{
				if(color == SSD_1306_COLOR_BLACK)
				{
					ssd1306_draw_rectangle(x, y, scale, scale, SSD_1306_COLOR_BLACK);
				}
				else if(color == SSD_1306_COLOR_WHITE)
				{
					ssd1306_draw_rectangle(x, y, scale, scale, SSD_1306_COLOR_WHITE);
				}
			}
			else
			{
				if(color == SSD_1306_COLOR_BLACK)
				{
					ssd1306_draw_rectangle(x, y, scale, scale, SSD_1306_COLOR_WHITE);
				}
				else if(color == SSD_1306_COLOR_WHITE)
				{
					ssd1306_draw_rectangle(x, y, scale, scale, SSD_1306_COLOR_BLACK);
				}
			}
			y += scale;
			mask = mask << 1;
		}
	}

}

/*
 * @fn      		  - ssd1306_get_buffer
 *
 * @return            - buffer of display
 *
 * @Note              - This function return buffer od display in current state
 */
uint8_t* ssd1306_get_buffer(void)
{
	return buffer;
}

/*
 *  Static functions
 */

void ssd1306_write_command(uint8_t command)
{
	if(HAL_I2C_Mem_Write(&SSD1306_I2C_HANDLE, SSD1306_I2C_ADDRESS, SSD1306_CTRL_BYTE_COMM,
			SSD1306_MEM_ADDR_SIZE, &command, SSD1306_COMM_DATA_SIZE, SSD1306_I2C_MAX_DELAY) != HAL_OK)
	{
		error_handler();
	}
}

void ssd1306_write_data(uint8_t *data, uint16_t length)
{
	if(HAL_I2C_Mem_Write(&SSD1306_I2C_HANDLE, SSD1306_I2C_ADDRESS, SSD1306_CTRL_BYTE_DATA,
						 SSD1306_MEM_ADDR_SIZE, data, length, SSD1306_I2C_MAX_DELAY) != HAL_OK)
	{
		error_handler();
	}
}
