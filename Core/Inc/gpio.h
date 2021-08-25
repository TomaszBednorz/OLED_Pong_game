#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "main.h"

void gpio_init();
uint8_t button_read(uint8_t button);

// Buttons defines param: @BUTTON_NUM
#define BUTTON_ACCEPT				1
#define BUTTON_PLAYER1_UP			2
#define BUTTON_PLAYER1_DOWN			3
#define BUTTON_PLAYER2_UP			4
#define BUTTON_PLAYER2_DOWN			5

// GPIOs ports and pins defines
#define BUTTON_PLAYER1_UP_PORT		GPIOA
#define BUTTON_PLAYER1_UP_PIN		GPIO_PIN_12

#define BUTTON_PLAYER1_DOWN_PORT	GPIOA
#define BUTTON_PLAYER1_DOWN_PIN		GPIO_PIN_11

#define BUTTON_PLAYER2_UP_PORT		GPIOB
#define BUTTON_PLAYER2_UP_PIN		GPIO_PIN_12

#define BUTTON_PLAYER2_DOWN_PORT	GPIOB
#define BUTTON_PLAYER2_DOWN_PIN		GPIO_PIN_11

#define BUTTON_ACCEPT_PORT			GPIOC
#define BUTTON_ACCEPT_PIN			GPIO_PIN_8

// Button states param: @BUTTON_STATE
#define BUTTON_RELEASED				0
#define BUTTON_PUSHED				1

void gpio_init();


#endif /* INC_GPIO_H_ */
