#ifndef INC_GAME_INTERFACE_H_
#define INC_GAME_INTERFACE_H_

#include "main.h"


typedef struct
{
	uint8_t max_points;
	char player1_name[4];
	char player2_name[4];
	uint8_t player1_points;
	uint8_t player2_points;
	uint8_t player1_position;
	uint8_t player2_position;
}GAME_TypeDef;



/*
 *  States in main menu param: @GAME_STATES
 */
#define GAME_STATE_PLAY			1
#define GAME_STATE_SETTINGS		2
#define GAME_STATE_RESULTS		3
#define GAME_STATE_EXIT			4


void main_menu(void);

#endif /* INC_GAME_INTERFACE_H_ */
