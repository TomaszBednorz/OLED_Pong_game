#ifndef INC_GAME_INTERFACE_H_
#define INC_GAME_INTERFACE_H_

#include "main.h"

/*
 *  Defines which set different positions and dimensions in game
 */
#define GAME_PLAYER1_X_POS				2
#define GAME_PLAYER2_X_POS				124

#define GAME_RACKET_WIDTH				2
#define GAME_RACKET_HEIGHT				14

#define GAME_BALL_SIZE					2

#define GAME_FIELD_MIN_Y				4
#define GAME_FIELD_MAX_Y				60

#define GAME_FIELD_CENTRE_X				63
#define GAME_FIELD_CENTRE_Y				31

#define GAME_PLAYER_DEFAULT_POSITION	25

/*
 *  Defines which help in ball moving
 */
#define GAME_RIGHT				0
#define GAME_LEFT				1

#define GAME_UP					0
#define GAME_DOWN				1

#define GAME_ANGLE_90			0
#define GAME_ANGLE_45			1

#define GAME_RACKET_1			1
#define GAME_RACKET_2			2

#define GAME_BALL_SPEED_2		2
#define GAME_BALL_SPEED_3		3
#define GAME_BALL_SPEED_4		4

/*
 *  Structure helpful in game storage
 */
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
 *  Structure which help storage the ball position
 */
typedef struct
{
	uint8_t x_cord;
	uint8_t y_cord;
}BALL_TypeDef;


/*
 *  States in main menu, param: @GAME_STATES
 */
#define GAME_STATE_PLAY			1
#define GAME_STATE_SETTINGS		2
#define GAME_STATE_RESULTS		3
#define GAME_STATE_EXIT			4

/*
 *  States in play state, param: @PLAY_STATES
 */
#define PLAY_STATE_1_LETTER		0
#define PLAY_STATE_2_LETTER		1
#define PLAY_STATE_3_LETTER		2
#define PLAY_STATE_4_LETTER		3
#define PLAY_STATE_5_LETTER		4
#define PLAY_STATE_6_LETTER		5
#define PLAY_STATE_GAME			6


/*
 *  Function prototypes
 */
void main_menu(void);

#endif /* INC_GAME_INTERFACE_H_ */
