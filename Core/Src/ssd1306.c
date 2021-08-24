#include "ssd1306.h"







/*
 *  Static functions prototypes
 */

void ssd1306_write_command(uint8_t *command);
void ssd1306_write_data(uint8_t *data);

/*
 *  Function definitions
 */

/*
 * @fn      		  - ssd1306_init
 *
 * @return            - true: correct initialization / false: incorrect initialization
 *
 * @Note              - This is SSD1306 initialization function
 */
bool ssd1306_init(void)
{

}







/*
 *  Static functions
 */

void ssd1306_write_command(uint8_t *command)
{
	if(HAL_I2C_Mem_Write(&SSD1306_I2C_HANDLE, SSD1306_I2C_ADDRESS, SSD1306_CTRL_BYTE_COMM,
						 SSD1306_MEM_ADDR_SIZE, command, strlen((const char *)command), SSD1306_I2C_MAX_DELAY) != HAL_OK)
	{
		error_handler();
	}
}

void ssd1306_write_data(uint8_t *data)
{
	if(HAL_I2C_Mem_Write(&SSD1306_I2C_HANDLE, SSD1306_I2C_ADDRESS, SSD1306_CTRL_BYTE_DATA,
						 SSD1306_MEM_ADDR_SIZE, data, strlen((const char *)data), SSD1306_I2C_MAX_DELAY) != HAL_OK)
	{
		error_handler();
	}
}
