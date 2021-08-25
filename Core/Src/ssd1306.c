#include "ssd1306.h"


// Buffer contains display content
uint8_t buffer[SSD1306_COLUMNS * SSD1306_PAGES];

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
	ssd1306_write_command(0x02);

	// Set Contrast Color
	ssd1306_write_command(SSD1306_SET_CONTRAST_COLOR);
	ssd1306_write_command(0x7F);

	// Disable Entire Display On
	ssd1306_write_command(SSD1306_RAM_CONTENT_DISPLAY);

	// Set Normal Display
	ssd1306_write_command(SSD1306_SET_NORMAL_DISPLAY);

	// Enable charge pump regulator
	ssd1306_write_command(SSD1306_CHARGE_PUMP_SETTING);
	ssd1306_write_command(SSD1306_ENABLE_CHARGE_PUMP);

	// Display On
	ssd1306_write_command(SSD1306_SET_DISPLAY_ON);
}


void ssd1306_display_clear(void)
{
	memset(buffer, 0, sizeof(buffer));
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
