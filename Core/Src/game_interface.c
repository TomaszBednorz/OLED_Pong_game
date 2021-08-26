#include "game_interface.h"


GAME_TypeDef last_results[3] = {0};  // Here are storage last 3 results of games
GAME_TypeDef game = {0};  // Here is storage actual game
BALL_TypeDef ball = {0};

// This table converts integers to chars
char int_to_char[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};


/*
 *  Static functions prototypes
 */

static void game_default_config(void);
static void last_results_default_config(void);
static void start_game_screen(void);
static void main_menu_screen(uint8_t game_state);
static void play_state(void);
static void settings_state(void);
static void results_state(void);
static void pong_game(void);
static void ball_default_position(void);
static void players_default_position(void);
static void reset_racket_places(uint8_t racket);
static void default_field_screen(void);
static void round_win_screen(uint8_t racket);
static void end_match_screen();
static void add_result_to_table(void);
/*
 * @fn      		  - main_menu
 *
 * @return            - None
 *
 * @Note              - In this function is main menu implementation
 */
void main_menu(void)
{
	bool end_main_menu_loop = false;
	uint8_t game_state = GAME_STATE_PLAY; // param: @GAME_STATES
	uint8_t last_game_state = 0;

	game.max_points = 3;
	game_default_config();
	players_default_position();
	ball_default_position();
	last_results_default_config();



	start_game_screen();

	while(button_read(BUTTON_ACCEPT) != BUTTON_PUSHED);  // Click accept button to go to main menu
	HAL_Delay(100);  // debouncing protection and break after click

	main_menu_screen(game_state);

	while(!end_main_menu_loop)
	{
		if(( button_read(BUTTON_PLAYER1_UP) == BUTTON_PUSHED || button_read(BUTTON_PLAYER2_UP) == BUTTON_PUSHED ) &&
			 game_state > GAME_STATE_PLAY)
		{
			last_game_state = game_state;
			game_state--;
		}

		if(( button_read(BUTTON_PLAYER1_DOWN) == BUTTON_PUSHED || button_read(BUTTON_PLAYER2_DOWN) == BUTTON_PUSHED ) &&
			 game_state < GAME_STATE_EXIT)
		{
			last_game_state = game_state;
			game_state++;
		}

		if(button_read(BUTTON_ACCEPT) == BUTTON_PUSHED)
		{
			HAL_Delay(100);  // debouncing protection and break after click

			switch (game_state) {
				case GAME_STATE_PLAY:
					play_state();
					break;
				case GAME_STATE_SETTINGS:
					settings_state();
					break;
				case GAME_STATE_RESULTS:
					results_state();
					break;
				case GAME_STATE_EXIT:
					end_main_menu_loop = true;
					break;
				default:
					break;
			}

			if(game_state != GAME_STATE_EXIT)
			{
				main_menu_screen(game_state);
				HAL_Delay(100);  // debouncing protection and break after click
			}

		}

		if(game_state != last_game_state)
		{
			last_game_state = game_state;
			main_menu_screen(game_state);
			HAL_Delay(100);  // debouncing protection and break after click
		}

	}
}

static void game_default_config(void)
{
	game.player1_name[0] = 'A';
	game.player1_name[1] = 'A';
	game.player1_name[2] = 'A';
	game.player1_name[3] = '\0';
	game.player2_name[0] = 'A';
	game.player2_name[1] = 'A';
	game.player2_name[2] = 'A';
	game.player2_name[3] = '\0';
	game.player1_points = 0;
	game.player2_points = 0;
}

static void last_results_default_config(void)
{
	for(int i = 0; i < 3; i++)
	{
		last_results[i] = game;
	}
}

static void start_game_screen(void)
{
	ssd1306_display_clear();
	oled_draw_string(12, 16, "START", 4, OLED_COLOR_WHITE);
	ssd1306_draw_horizontal_line(12, 46, 105, OLED_COLOR_WHITE);
	ssd1306_display_show();
}

static void main_menu_screen(uint8_t game_state)
{
	ssd1306_display_clear();
	oled_draw_string(42, 0, "PLAY", 2, OLED_COLOR_WHITE);
	oled_draw_string(20, 16, "SETTINGS", 2, OLED_COLOR_WHITE);
	oled_draw_string(25, 32, "RESULTS", 2, OLED_COLOR_WHITE);
	oled_draw_string(42, 48, "EXIT", 2, OLED_COLOR_WHITE);

	switch (game_state) {
		case GAME_STATE_PLAY:
			ssd1306_draw_horizontal_line(42, 15, 44, OLED_COLOR_WHITE);
			ssd1306_draw_horizontal_line(20, 31, 88, OLED_COLOR_BLACK);
			break;
		case GAME_STATE_SETTINGS:
			ssd1306_draw_horizontal_line(20, 31, 88, OLED_COLOR_WHITE);
			ssd1306_draw_horizontal_line(42, 15, 44, OLED_COLOR_BLACK);
			ssd1306_draw_horizontal_line(25, 47, 77, OLED_COLOR_BLACK);
			break;
		case GAME_STATE_RESULTS:
			ssd1306_draw_horizontal_line(25, 47, 77, OLED_COLOR_WHITE);
			ssd1306_draw_horizontal_line(20, 31, 88, OLED_COLOR_BLACK);
			ssd1306_draw_horizontal_line(42, 63, 44, OLED_COLOR_BLACK);
			break;
		case GAME_STATE_EXIT:
			ssd1306_draw_horizontal_line(42, 63, 44, OLED_COLOR_WHITE);
			ssd1306_draw_horizontal_line(25, 47, 77, OLED_COLOR_BLACK);
			break;
		default:
			break;
	}
	ssd1306_display_show();

}

static void play_state(void)
{
	uint8_t play_state = PLAY_STATE_1_LETTER;
	uint8_t current_letter = 0; // Letter in game.player1_name / game.player2_name
	uint8_t x_start_pos = 11;

	ssd1306_display_clear();

	oled_draw_arrow(x_start_pos, 0, 3, OLED_ARROW_UP, OLED_COLOR_WHITE);
	oled_draw_arrow(x_start_pos, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_WHITE);

	game.player1_name[current_letter] = 'A';
	oled_draw_char(5, 22, game.player1_name[current_letter], 3, OLED_COLOR_WHITE);


	for(int i = 25; i < 128; i += 20)
	{
		oled_draw_char(i, 22, '-', 3, OLED_COLOR_WHITE);
		if(i == 45)
		{
			i += 5;
		}
	}

	for(int i = 4; i <= 60; i += 16)
	{
		ssd1306_draw_rectangle(63, i, 2, 8, OLED_COLOR_WHITE);
	}


	ssd1306_display_show();

	while(play_state != PLAY_STATE_GAME)
	{
		if(button_read(BUTTON_PLAYER1_UP) == BUTTON_PUSHED)
		{
			if(play_state < 3 && game.player1_name[current_letter] < 'Z')
			{
				game.player1_name[current_letter]++;
				oled_draw_char(x_start_pos - 6, 22, game.player1_name[current_letter], 3, OLED_COLOR_WHITE);
				ssd1306_display_show();
				HAL_Delay(100);  // debouncing protection and break after click
			}
		}

		if(button_read(BUTTON_PLAYER1_DOWN) == BUTTON_PUSHED)
		{
			if(play_state < 3 && game.player1_name[current_letter] > 'A')
			{
				game.player1_name[current_letter]--;
				oled_draw_char(x_start_pos - 6, 22, game.player1_name[current_letter], 3, OLED_COLOR_WHITE);
				ssd1306_display_show();
				HAL_Delay(100);  // debouncing protection and break after click
			}
		}

		if(button_read(BUTTON_PLAYER2_UP) == BUTTON_PUSHED)
		{
			if(play_state >= 3 && game.player2_name[current_letter] < 'Z')
			{
				game.player2_name[current_letter]++;
				oled_draw_char(x_start_pos - 6, 22, game.player2_name[current_letter], 3, OLED_COLOR_WHITE);
				ssd1306_display_show();
				HAL_Delay(100);  // debouncing protection and break after click
			}
		}

		if(button_read(BUTTON_PLAYER2_DOWN) == BUTTON_PUSHED)
		{
			if(play_state >= 3 && game.player2_name[current_letter] > 'A')
			{
				game.player2_name[current_letter]--;
				oled_draw_char(x_start_pos - 6, 22, game.player2_name[current_letter], 3, OLED_COLOR_WHITE);
				ssd1306_display_show();
				HAL_Delay(100);  // debouncing protection and break after click
			}
		}

		if(button_read(BUTTON_ACCEPT) == BUTTON_PUSHED)
		{
			play_state++;

			if(current_letter >= 2)
			{
				current_letter = 0;
			}
			else
			{
				current_letter++;
			}

			switch (play_state) {
				case PLAY_STATE_2_LETTER:
					oled_draw_arrow(x_start_pos, 0, 3, OLED_ARROW_UP, OLED_COLOR_BLACK);
					oled_draw_arrow(x_start_pos, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_BLACK);
					x_start_pos += 20;
					game.player1_name[current_letter] = 'A';
					oled_draw_char(x_start_pos - 6, 22, game.player1_name[current_letter], 3, OLED_COLOR_WHITE);
					oled_draw_arrow(x_start_pos, 0, 3, OLED_ARROW_UP, OLED_COLOR_WHITE);
					oled_draw_arrow(x_start_pos, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_WHITE);
					break;
				case PLAY_STATE_3_LETTER:
					oled_draw_arrow(x_start_pos, 0, 3, OLED_ARROW_UP, OLED_COLOR_BLACK);
					oled_draw_arrow(x_start_pos, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_BLACK);
					x_start_pos += 20;
					game.player1_name[current_letter] = 'A';
					oled_draw_char(x_start_pos - 6, 22, game.player1_name[current_letter], 3, OLED_COLOR_WHITE);
					oled_draw_arrow(x_start_pos, 0, 3, OLED_ARROW_UP, OLED_COLOR_WHITE);
					oled_draw_arrow(x_start_pos, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_WHITE);
					break;
				case PLAY_STATE_4_LETTER:
					oled_draw_arrow(x_start_pos, 0, 3, OLED_ARROW_UP, OLED_COLOR_BLACK);
					oled_draw_arrow(x_start_pos, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_BLACK);
					x_start_pos += 25;
					game.player2_name[current_letter] = 'A';
					oled_draw_char(x_start_pos - 6, 22, game.player2_name[current_letter], 3, OLED_COLOR_WHITE);
					oled_draw_arrow(x_start_pos, 0, 3, OLED_ARROW_UP, OLED_COLOR_WHITE);
					oled_draw_arrow(x_start_pos, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_WHITE);
					break;
				case PLAY_STATE_5_LETTER:
					oled_draw_arrow(x_start_pos, 0, 3, OLED_ARROW_UP, OLED_COLOR_BLACK);
					oled_draw_arrow(x_start_pos, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_BLACK);
					x_start_pos += 20;
					game.player2_name[current_letter] = 'A';
					oled_draw_char(x_start_pos - 6, 22, game.player2_name[current_letter], 3, OLED_COLOR_WHITE);
					oled_draw_arrow(x_start_pos, 0, 3, OLED_ARROW_UP, OLED_COLOR_WHITE);
					oled_draw_arrow(x_start_pos, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_WHITE);
					break;
				case PLAY_STATE_6_LETTER:
					oled_draw_arrow(x_start_pos, 0, 3, OLED_ARROW_UP, OLED_COLOR_BLACK);
					oled_draw_arrow(x_start_pos, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_BLACK);
					x_start_pos += 20;
					game.player2_name[current_letter] = 'A';
					oled_draw_char(x_start_pos - 6, 22, game.player2_name[current_letter], 3, OLED_COLOR_WHITE);
					oled_draw_arrow(x_start_pos, 0, 3, OLED_ARROW_UP, OLED_COLOR_WHITE);
					oled_draw_arrow(x_start_pos, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_WHITE);
					break;
				case PLAY_STATE_GAME:
					pong_game();
				default:
					break;
			}
			ssd1306_display_show();
			HAL_Delay(200);  // debouncing protection and break after click
		}

	}

}

static void settings_state(void)
{
	bool end_setting_state_loop = false;
	bool needed_display_actualization = true;

	ssd1306_display_clear();
	oled_draw_string(60, 5, "ROUNDS", 2, OLED_COLOR_WHITE);
	oled_draw_string(60, 40, "EXIT", 3, OLED_COLOR_WHITE);
	ssd1306_draw_horizontal_line(60, 62, 64, OLED_COLOR_WHITE);

	while(!end_setting_state_loop)
	{
		if(( button_read(BUTTON_PLAYER1_UP) == BUTTON_PUSHED || button_read(BUTTON_PLAYER2_UP) == BUTTON_PUSHED ) &&
			 game.max_points < 9)
		{
			game.max_points++;
			needed_display_actualization = true;
		}

		if(( button_read(BUTTON_PLAYER1_DOWN) == BUTTON_PUSHED || button_read(BUTTON_PLAYER2_DOWN) == BUTTON_PUSHED ) &&
			 game.max_points > 3)
		{
			game.max_points--;
			needed_display_actualization = true;
		}

		if(button_read(BUTTON_ACCEPT) == BUTTON_PUSHED)  // Click accept button to go to main menu
		{
			end_setting_state_loop = true;
		}


		if(needed_display_actualization)
		{
			if(game.max_points == 3)
			{
				oled_draw_arrow(30, 0, 3, OLED_ARROW_UP, OLED_COLOR_WHITE);
				oled_draw_arrow(30, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_BLACK);
				oled_draw_char(24, 21, int_to_char[game.max_points], 3, OLED_COLOR_WHITE);
			}
			else if(game.max_points == 9)
			{
				oled_draw_arrow(30, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_WHITE);
				oled_draw_arrow(30, 0, 3, OLED_ARROW_UP, OLED_COLOR_BLACK);
				oled_draw_char(24, 21, int_to_char[game.max_points], 3, OLED_COLOR_WHITE);

			}
			else
			{
				oled_draw_arrow(30, 0, 3, OLED_ARROW_UP, OLED_COLOR_WHITE);
				oled_draw_arrow(30, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_WHITE);
				oled_draw_char(24, 21, int_to_char[game.max_points], 3, OLED_COLOR_WHITE);
			}
			ssd1306_display_show();
			needed_display_actualization = false;
			HAL_Delay(100);  // debouncing protection and break after click
		}
	}
}

static void results_state(void)
{
	uint8_t results_y = 0;

	ssd1306_display_clear();

	for(int i = 0; i < 3; i++)
	{
		oled_draw_string(4, results_y, last_results[i].player1_name, 2, OLED_COLOR_WHITE);
		oled_draw_char(48, results_y, int_to_char[last_results[i].player1_points], 2, OLED_COLOR_WHITE);
		oled_draw_char(59, results_y, ':', 2, OLED_COLOR_WHITE);
		oled_draw_char(70, results_y, int_to_char[last_results[i].player2_points], 2, OLED_COLOR_WHITE);
		oled_draw_string(92, results_y, last_results[i].player2_name, 2, OLED_COLOR_WHITE);
		results_y += 15;
	}

	oled_draw_string(42, 48, "EXIT", 2, OLED_COLOR_WHITE);
	ssd1306_draw_horizontal_line(42, 63, 44, OLED_COLOR_WHITE);
	ssd1306_display_show();

	while(button_read(BUTTON_ACCEPT) != BUTTON_PUSHED);  // Click accept button to go to main menu
}

static void pong_game(void)
{
	uint32_t racket_time, ball_speed_time;
	bool next_round = true;
	int random_number;
	uint8_t current_movement_x, current_movement_y, current_angle;
	uint8_t game_ball_speed, player_pos_up, player_pos_down;

	default_field_screen();
	ssd1306_display_show();



	while(game.player1_points != game.max_points && game.player2_points != game.max_points)
	{
		if(next_round)
		{
			ball_default_position();
			players_default_position();
			default_field_screen();
			game_ball_speed = GAME_BALL_SIZE;
			racket_time = HAL_GetTick();
			ball_speed_time = HAL_GetTick();
			random_number = rand() % 4;

			if(random_number == 0)
			{
				current_movement_x = GAME_RIGHT;
				current_movement_y = GAME_UP;
			}
			else if(random_number == 1)
			{
				current_movement_x = GAME_RIGHT;
				current_movement_y = GAME_DOWN;
			}
			else if(random_number == 2)
			{
				current_movement_x = GAME_LEFT;
				current_movement_y = GAME_UP;
			}
			else if(random_number == 3)
			{
				current_movement_x = GAME_LEFT;
				current_movement_y = GAME_DOWN;
			}

			current_angle = rand() % 2;

			next_round = false;
		}

		// Racket service
		if(racket_time + 25 < HAL_GetTick()) // Max racket speed: 1px/25ms
		{
			if(button_read(BUTTON_PLAYER1_UP) == BUTTON_PUSHED)
			{
				if(game.player1_position > GAME_FIELD_MIN_Y)
				{
					reset_racket_places(GAME_RACKET_1);
					game.player1_position--;
					ssd1306_draw_rectangle(GAME_PLAYER1_X_POS, game.player1_position, GAME_RACKET_WIDTH, GAME_RACKET_HEIGHT, OLED_COLOR_WHITE);
				}
			}

			if(button_read(BUTTON_PLAYER1_DOWN) == BUTTON_PUSHED)
			{
				if(game.player1_position <= GAME_FIELD_MAX_Y - GAME_RACKET_HEIGHT)
				{
					reset_racket_places(GAME_RACKET_1);
					game.player1_position++;
					ssd1306_draw_rectangle(GAME_PLAYER1_X_POS, game.player1_position, GAME_RACKET_WIDTH, GAME_RACKET_HEIGHT, OLED_COLOR_WHITE);
				}
			}

			if(button_read(BUTTON_PLAYER2_UP) == BUTTON_PUSHED)
			{
				if(game.player2_position > GAME_FIELD_MIN_Y)
				{
					reset_racket_places(GAME_RACKET_2);
					game.player2_position--;
					ssd1306_draw_rectangle(GAME_PLAYER2_X_POS, game.player2_position, GAME_RACKET_WIDTH, GAME_RACKET_HEIGHT, OLED_COLOR_WHITE);
				}
			}

			if(button_read(BUTTON_PLAYER2_DOWN) == BUTTON_PUSHED)
			{
				if(game.player2_position <= GAME_FIELD_MAX_Y - GAME_RACKET_HEIGHT)
				{
					reset_racket_places(GAME_RACKET_2);
					game.player2_position++;
					ssd1306_draw_rectangle(GAME_PLAYER2_X_POS, game.player2_position, GAME_RACKET_WIDTH, GAME_RACKET_HEIGHT, OLED_COLOR_WHITE);
				}
			}


			racket_time = HAL_GetTick();
		}

		// Ball service

		if(ball.x_cord >= GAME_PLAYER2_X_POS - GAME_BALL_SIZE)
		{
			current_movement_x = GAME_LEFT;
			current_angle = rand() % 2;

			player_pos_up = game.player2_position;
			player_pos_down = game.player2_position + GAME_RACKET_HEIGHT;

			if(ball.y_cord < player_pos_up || ball.y_cord > player_pos_down) // This round win player 1
			{
				game.player1_points++;
				round_win_screen(GAME_RACKET_1);
				next_round = true;
			}

		}

		if(ball.x_cord <= GAME_PLAYER1_X_POS + GAME_BALL_SIZE)
		{
			current_movement_x = GAME_RIGHT;
			current_angle = rand() % 2;

			player_pos_up = game.player1_position;
			player_pos_down = game.player1_position + GAME_RACKET_HEIGHT;

			if(ball.y_cord < player_pos_up || ball.y_cord > player_pos_down) // This round win player 2
			{
				game.player2_points++;
				round_win_screen(GAME_RACKET_2);
				next_round = true;
			}
		}

		if(ball.y_cord >= GAME_FIELD_MAX_Y - GAME_BALL_SIZE)
		{
			current_movement_y = GAME_UP;
			current_angle = rand() % 2;
		}

		if(ball.y_cord <= GAME_FIELD_MIN_Y + 1)
		{
			current_movement_y = GAME_DOWN;
			current_angle = rand() % 2;
		}


		ssd1306_draw_rectangle(ball.x_cord, ball.y_cord, GAME_BALL_SIZE, GAME_BALL_SIZE, OLED_COLOR_BLACK); // Delete last ball position

		if(current_movement_x == GAME_RIGHT)
		{
			ball.x_cord += game_ball_speed;
		}
		else if(current_movement_x == GAME_LEFT)
		{
			ball.x_cord -= game_ball_speed;
		}

		if(current_movement_y == GAME_UP)
		{
			if(current_angle == 0)
			{
				ball.y_cord -= game_ball_speed;
			}
			else if(current_angle == 1)
			{
				ball.y_cord -= game_ball_speed / 2;
			}

		}
		else if(current_movement_y == GAME_DOWN)
		{
			if(current_angle == 0)
			{
				ball.y_cord += game_ball_speed;
			}
			else if(current_angle == 1)
			{
				ball.y_cord += game_ball_speed / 2;
			}
		}

		if(ball_speed_time + 15000 < HAL_GetTick())
		{
			game_ball_speed = GAME_BALL_SIZE * 2;
		}

		ssd1306_draw_rectangle(ball.x_cord, ball.y_cord, GAME_BALL_SIZE, GAME_BALL_SIZE, OLED_COLOR_WHITE);  // Update last ball position
		default_field_screen();
		ssd1306_display_show();

	}

	// Routine after match
	end_match_screen();
	add_result_to_table();
	game_default_config();

	while(button_read(BUTTON_ACCEPT) != BUTTON_PUSHED);
}

static void ball_default_position(void)
{
	ball.x_cord = 63;
	ball.y_cord = 31;
}

static void players_default_position(void)
{
	game.player1_position = 25;
	game.player2_position = 25;
}

static void reset_racket_places(uint8_t racket)
{
	if(racket & GAME_RACKET_1)
	{
		ssd1306_draw_rectangle(GAME_PLAYER1_X_POS, game.player1_position, GAME_RACKET_WIDTH, GAME_RACKET_HEIGHT, OLED_COLOR_BLACK);
	}

	if(racket & GAME_RACKET_2)
	{
		ssd1306_draw_rectangle(GAME_PLAYER2_X_POS, game.player2_position, GAME_RACKET_WIDTH, GAME_RACKET_HEIGHT, OLED_COLOR_BLACK);
	}
}

static void default_field_screen(void)
{
	ssd1306_display_clear();

	ssd1306_draw_horizontal_line(2, 3, 124, OLED_COLOR_WHITE);
	ssd1306_draw_horizontal_line(2, 61, 124, OLED_COLOR_WHITE);


	oled_draw_string(44, 4, game.player1_name, 1, OLED_COLOR_WHITE);
	oled_draw_string(66, 4, game.player2_name, 1, OLED_COLOR_WHITE);

	oled_draw_char(50, 12, int_to_char[game.player1_points], 1, OLED_COLOR_WHITE);
	oled_draw_char(72, 12, int_to_char[game.player2_points], 1, OLED_COLOR_WHITE);

	for(int i = 4; i <= 60; i += 16)
	{
		ssd1306_draw_rectangle(63, i, 2, 8, OLED_COLOR_WHITE);
	}

	ssd1306_draw_rectangle(GAME_PLAYER1_X_POS, game.player1_position, GAME_RACKET_WIDTH, GAME_RACKET_HEIGHT, OLED_COLOR_WHITE);
	ssd1306_draw_rectangle(GAME_PLAYER2_X_POS, game.player2_position, GAME_RACKET_WIDTH, GAME_RACKET_HEIGHT, OLED_COLOR_WHITE);

	ssd1306_draw_rectangle(ball.x_cord, ball.y_cord, GAME_BALL_SIZE, GAME_BALL_SIZE, OLED_COLOR_WHITE);
}

static void round_win_screen(uint8_t racket)
{
	if(racket == GAME_RACKET_1)
	{
		oled_draw_string(10, 20, game.player1_name, 3, OLED_COLOR_WHITE);
		oled_draw_string(75, 25, "GOAL", 2, OLED_COLOR_WHITE);
	}
	else if(racket == GAME_RACKET_2)
	{
		oled_draw_string(70, 20, game.player2_name, 3, OLED_COLOR_WHITE);
		oled_draw_string(10, 25, "GOAL", 2, OLED_COLOR_WHITE);
	}
	ssd1306_display_show();
	HAL_Delay(1000);
}

static void end_match_screen()
{
	ssd1306_display_clear();

	oled_draw_string(15, 0, game.player1_name, 2, OLED_COLOR_WHITE);
	oled_draw_char(24, 18, int_to_char[game.player1_points], 3, OLED_COLOR_WHITE);

	oled_draw_string(79, 0, game.player2_name, 2, OLED_COLOR_WHITE);
	oled_draw_char(87, 18, int_to_char[game.player2_points], 3, OLED_COLOR_WHITE);

	oled_draw_string(41, 45, "EXIT", 2, OLED_COLOR_WHITE);
	ssd1306_draw_horizontal_line(41, 61, 44, OLED_COLOR_WHITE);

	ssd1306_display_show();
}

static void add_result_to_table(void)
{
	// Last results table has 3 slots, so result from 3rd place go out, result from 2nd place
	// will replace 3rd place, result from 1nd place will replace 2nd place and last result
	// will as 1st in table

	for(int i = 2; i > 0; i--)
	{
		last_results[i] = last_results[i-1];
	}

	for(int i = 0; i < 3; i++)
	{
		last_results[0].player1_name[i] = game.player1_name[i];
		last_results[0].player2_name[i] = game.player2_name[i];
	}

	last_results[0].player1_points = game.player1_points;
	last_results[0].player2_points = game.player2_points;

}
