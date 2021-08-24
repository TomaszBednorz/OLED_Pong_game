#include "main.h"

void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}


void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	GPIO_InitTypeDef GPIO_i2c1 = {0};

	if(hi2c->Instance == I2C1)
	{
		/*
		 * PB6 --> I2C1_SCL
		 * PB7 --> I2C1_SDA
		 */
		__HAL_RCC_I2C1_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();

		GPIO_i2c1.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_i2c1.Pull = GPIO_NOPULL;
		GPIO_i2c1.Speed = GPIO_SPEED_FREQ_MEDIUM;
		GPIO_i2c1.Mode = GPIO_MODE_AF_OD;
		GPIO_i2c1.Alternate = GPIO_AF1_I2C1;
		HAL_GPIO_Init(GPIOB, &GPIO_i2c1);
	}

}
