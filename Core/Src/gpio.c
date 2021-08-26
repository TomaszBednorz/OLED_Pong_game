#include "gpio.h"


void gpio_init()
{
	GPIO_InitTypeDef GPIO_buttons = {0};
	/*
	 * PA12 --> BUTTON_PLAYER1_UP
	 * PA11 --> BUTTON_PLAYER1_DOWN
	 * PB12 --> BUTTON_PLAYER2_UP
	 * PB11 --> BUTTON_PLAYER2_DOWN
	 * PC8  --> BUTTON_ACCEPT
	 */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_buttons.Pin = BUTTON_ACCEPT_PIN;
	GPIO_buttons.Pull = GPIO_NOPULL;
	GPIO_buttons.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_buttons.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(BUTTON_ACCEPT_PORT, &GPIO_buttons);

	GPIO_buttons.Pin = BUTTON_PLAYER1_DOWN_PIN;
	HAL_GPIO_Init(BUTTON_PLAYER1_DOWN_PORT, &GPIO_buttons);

	GPIO_buttons.Pin = BUTTON_PLAYER2_UP_PIN;
	HAL_GPIO_Init(BUTTON_PLAYER2_UP_PORT, &GPIO_buttons);

	GPIO_buttons.Pin = BUTTON_PLAYER2_DOWN_PIN;
	HAL_GPIO_Init(BUTTON_PLAYER2_DOWN_PORT, &GPIO_buttons);

	GPIO_buttons.Pin = BUTTON_PLAYER1_UP_PIN;
	HAL_GPIO_Init(BUTTON_PLAYER1_UP_PORT, &GPIO_buttons);

}

/*
 * @fn      		  - button_read
 *
 * @param[in]         - button - param: @BUTTON_NUM
 *
 * @return            - param: @BUTTON_STATE
 *
 * @Note              - This function read state from GPIOs
 */
uint8_t button_read(uint8_t button)
{
	if(button == BUTTON_ACCEPT)
	{
		if(HAL_GPIO_ReadPin(BUTTON_ACCEPT_PORT, BUTTON_ACCEPT_PIN) == GPIO_PIN_RESET)
		{
			return BUTTON_PUSHED;
		}
	}
	else if(button == BUTTON_PLAYER1_UP)
	{
		if(HAL_GPIO_ReadPin(BUTTON_PLAYER1_UP_PORT, BUTTON_PLAYER1_UP_PIN) == GPIO_PIN_RESET)
		{
			return BUTTON_PUSHED;
		}
	}
	else if(button == BUTTON_PLAYER1_DOWN)
	{
		if(HAL_GPIO_ReadPin(BUTTON_PLAYER1_DOWN_PORT, BUTTON_PLAYER1_DOWN_PIN) == GPIO_PIN_RESET)
		{
			return BUTTON_PUSHED;
		}
	}
	else if(button == BUTTON_PLAYER2_UP)
	{
		if(HAL_GPIO_ReadPin(BUTTON_PLAYER2_UP_PORT, BUTTON_PLAYER2_UP_PIN) == GPIO_PIN_RESET)
		{
			return BUTTON_PUSHED;
		}
	}
	else if(button == BUTTON_PLAYER2_DOWN)
	{
		if(HAL_GPIO_ReadPin(BUTTON_PLAYER2_DOWN_PORT, BUTTON_PLAYER2_DOWN_PIN) == GPIO_PIN_RESET)
		{
			return BUTTON_PUSHED;
		}
	}

	return BUTTON_RELEASED;
}


