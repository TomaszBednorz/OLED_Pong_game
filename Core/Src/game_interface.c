#include "game_interface.h"


GAME_TypeDef last_results[3] = {0};  // Here are storage last 3 results of games
GAME_TypeDef game = {0};  // Here is storage actual game
BALL_TypeDef ball = {0};  // Here is storage ball position

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

	// Initial configuration in main menu - START
	game.max_points = 3;
	game_default_config();
	players_default_position();
	ball_default_position();
	last_results_default_config();
	// Initial configuration in main menu - STOP

	start_game_screen();

	while(button_read(BUTTON_ACCEPT) != BUTTON_PUSHED);  // Click accept button to go to main menu
	HAL_Delay(100);  // debouncing protection and break after click

	main_menu_screen(game_state);

	while(!end_main_menu_loop)  // While exit isn't clicked
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

			// Back to main menu after exit from last state
			if(game_state != GAME_STATE_EXIT)
			{
				main_menu_screen(game_state);
				HAL_Delay(100);  // debouncing protection and break after click
			}

		}

		// Update underline after state update (after clicking button UP or DOWN)
		if(game_state != last_game_state)
		{
			last_game_state = game_state;
			main_menu_screen(game_state);
			HAL_Delay(100);  // debouncing protection and break after click
		}

	}
}

/*
 * Setting default parameters of game structure (names and points od 2 players)
 */
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

/*
 * Setting default parameters of last results structure (0 points and nicknames are AAA)
 */
static void last_results_default_config(void)
{
	for(int i = 0; i < 3; i++)
	{
		last_results[i] = game;
	}
}

/*
 *  Welcoming screen before main menu
 */
static void start_game_screen(void)
{
	ssd1306_display_clear();
	oled_draw_string(12, 16, "START", 4, OLED_COLOR_WHITE);
	ssd1306_draw_horizontal_line(12, 46, 105, OLED_COLOR_WHITE);
	ssd1306_display_show();
}

/*
 *  Main menu screen. Printing 4 game states and one of them (game_state) is underlined
 */
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

/*
 *  Implementation of play state (one of state in main menu)
 */
static void play_state(void)
{
	uint8_t play_state = PLAY_STATE_1_LETTER;  // param: @PLAY_STATES
	uint8_t current_letter = 0; // Letter in game.player1_name / game.player2_name
	uint8_t x_start_pos = 11;  // Reference position from x axis

	ssd1306_display_clear();

	oled_draw_arrow(x_start_pos, 0, 3, OLED_ARROW_UP, OLED_COLOR_WHITE);
	oled_draw_arrow(x_start_pos, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_WHITE);

	game.player1_name[current_letter] = 'A';  // Setting the first letter of player 1 nickname (player can configure it)
	oled_draw_char(5, 22, game.player1_name[current_letter], 3, OLED_COLOR_WHITE);

	// Draw '-' in place of future letters
	for(int i = 25; i < 128; i += 20)
	{
		oled_draw_char(i, 22, '-', 3, OLED_COLOR_WHITE);
		if(i == 45)
		{
			i += 5;
		}
	}

	// Draw vertical line in center of OLED display
	for(int i = 4; i <= 60; i += 16)
	{
		ssd1306_draw_rectangle(63, i, 2, 8, OLED_COLOR_WHITE);
	}

	ssd1306_display_show();

	while(play_state != PLAY_STATE_GAME) // While letters are written (first 6 states in param: @PLAY_STATES)
	{
		// Section of letter selection - START
		if(button_read(BUTTON_PLAYER1_UP) == BUTTON_PUSHED)
		{
			if(play_state < PLAY_STATE_4_LETTER && game.player1_name[current_letter] < 'Z')
			{
				game.player1_name[current_letter]++;
				oled_draw_char(x_start_pos - 6, 22, game.player1_name[current_letter], 3, OLED_COLOR_WHITE); // x_start_pos - 6 is good position for letters
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
		// Section of letter selection - END

		if(button_read(BUTTON_ACCEPT) == BUTTON_PUSHED)
		{
			play_state++;

			if(current_letter >= 2)
			{
				current_letter = 0;  // 1st player chose his/her letters, it's time for 2nd player
			}
			else
			{
				current_letter++;  // Increment position of buffer to nickname writing
			}

			switch (play_state) {
				case PLAY_STATE_2_LETTER:
					// Delete arrows from last position
					oled_draw_arrow(x_start_pos, 0, 3, OLED_ARROW_UP, OLED_COLOR_BLACK);
					oled_draw_arrow(x_start_pos, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_BLACK);

					// Update position and write to game.player1_name[current_letter] default 'A' letter and draw it in buffer
					x_start_pos += 20;
					game.player1_name[current_letter] = 'A';
					oled_draw_char(x_start_pos - 6, 22, game.player1_name[current_letter], 3, OLED_COLOR_WHITE);

					// Draw arrows in new position
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
				case PLAY_STATE_GAME: // Start Pong game
					pong_game();
				default:
					break;
			}
			ssd1306_display_show();
			HAL_Delay(200);  // debouncing protection and break after click
		}

	}

}

/*
 *  Implementation of setting state (one of state in main menu). User can set points between 3 to 9
 */
static void settings_state(void)
{
	bool end_setting_state_loop = false;
	bool needed_display_actualization = true;  // Variable which informs about display actualization

	ssd1306_display_clear();
	oled_draw_string(60, 5, "ROUNDS", 2, OLED_COLOR_WHITE);
	oled_draw_string(60, 40, "EXIT", 3, OLED_COLOR_WHITE);
	ssd1306_draw_horizontal_line(60, 62, 64, OLED_COLOR_WHITE); // Underline "EXIT" string

	while(!end_setting_state_loop)  // While exit isn't clicked
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
			if(game.max_points == 3)  // In this situation only up arrow is on display
			{
				oled_draw_arrow(30, 0, 3, OLED_ARROW_UP, OLED_COLOR_WHITE);
				oled_draw_arrow(30, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_BLACK);
				oled_draw_char(24, 21, int_to_char[game.max_points], 3, OLED_COLOR_WHITE);
			}
			else if(game.max_points == 9)  // In this situation only down arrow is on display
			{
				oled_draw_arrow(30, 60, 3, OLED_ARROW_DOWN, OLED_COLOR_WHITE);
				oled_draw_arrow(30, 0, 3, OLED_ARROW_UP, OLED_COLOR_BLACK);
				oled_draw_char(24, 21, int_to_char[game.max_points], 3, OLED_COLOR_WHITE);

			}
			else  // In this situation up and down arrows are on display
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

/*
 *  Implementation of results state (one of state in main menu)
 */
static void results_state(void)
{
	uint8_t results_y = 0;  // Reference position from y axis

	ssd1306_display_clear();

	// Draw in 3 lines last results: XYZ P:P XYZ (XYZ-nickname, P-points)
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
	ssd1306_draw_horizontal_line(42, 63, 44, OLED_COLOR_WHITE);  // Underline "EXIT" string
	ssd1306_display_show();

	while(button_read(BUTTON_ACCEPT) != BUTTON_PUSHED);  // Click accept button to go to main menu
}

/*
 *  Implementation of Pong game in play state
 */
static void pong_game(void)
{
	uint32_t racket_time, ball_speed_time;
	bool next_round = true;  // This variable return field and settings after end of round
	int random_number;  // Here will be storage random number to create some randomness in game
	uint8_t current_movement_x, current_movement_y, current_angle;
	uint8_t game_ball_speed, player_pos_up, player_pos_down;

	default_field_screen();
	ssd1306_display_show();

	while(game.player1_points != game.max_points && game.player2_points != game.max_points)  // While one of player won't achieve max amount of points
	{
		if(next_round)
		{
			// Set field in default configuration before next round
			ball_default_position();
			players_default_position();
			default_field_screen();
			game_ball_speed = GAME_BALL_SPEED_2; // GAME_BALL_SPEED_2 is default speed of ball
			racket_time = HAL_GetTick();  // This variable storage actual time. Player can move racket every around (20ms + execution time)
			ball_speed_time = HAL_GetTick();  // This variable storage actual time. After a few seconds ball will be faster
			random_number = rand() % 4;  // Draw random number from 0 to 3

			if(random_number == 0)  // Ball will move in right, up direction
			{
				current_movement_x = GAME_RIGHT;
				current_movement_y = GAME_UP;
			}
			else if(random_number == 1)  // Ball will move in right, down direction
			{
				current_movement_x = GAME_RIGHT;
				current_movement_y = GAME_DOWN;
			}
			else if(random_number == 2)  // Ball will move in left, up direction
			{
				current_movement_x = GAME_LEFT;
				current_movement_y = GAME_UP;
			}
			else if(random_number == 3)  // Ball will move in left, down direction
			{
				current_movement_x = GAME_LEFT;
				current_movement_y = GAME_DOWN;
			}

			current_angle = rand() % 2;  // Draw random number from 0 to 1: 0 - ball will move at an angle 90 degree, 1 - ball will move at an angle 45 degree

			next_round = false;  // Block entry to this 'if' statement condition  (to next round)
		}

		// Racket service
		if(racket_time + 20 < HAL_GetTick()) // Max racket speed: 1px/(20ms + execution time)
		{
			if(button_read(BUTTON_PLAYER1_UP) == BUTTON_PUSHED)
			{
				if(game.player1_position > GAME_FIELD_MIN_Y)
				{
					// Update racket position on OLED display
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


			racket_time = HAL_GetTick();  // Update time in racket_time variable
		}

		// Ball service
		if(ball.x_cord >= GAME_PLAYER2_X_POS - GAME_BALL_SIZE)  // Limit ball movement from right side of field
		{
			current_movement_x = GAME_LEFT;  // Change ball direction
			current_angle = rand() % 2;  // Draw angle of movement

			player_pos_up = game.player2_position;
			player_pos_down = game.player2_position + GAME_RACKET_HEIGHT;

			if(ball.y_cord < player_pos_up || ball.y_cord > player_pos_down) // This round win player 1 (Ball don't see a racket)
			{
				game.player1_points++;
				round_win_screen(GAME_RACKET_1);
				next_round = true;  // Unblock entry to 'if' statement condition, which service next round
			}

		}

		if(ball.x_cord <= GAME_PLAYER1_X_POS + GAME_BALL_SIZE)  // Limit ball movement from left side of field
		{
			current_movement_x = GAME_RIGHT;
			current_angle = rand() % 2;

			player_pos_up = game.player1_position;
			player_pos_down = game.player1_position + GAME_RACKET_HEIGHT;

			if(ball.y_cord < player_pos_up || ball.y_cord > player_pos_down) // This round win player 2 (Ball don't see a racket)
			{
				game.player2_points++;
				round_win_screen(GAME_RACKET_2);
				next_round = true;
			}
		}

		if(ball.y_cord >= GAME_FIELD_MAX_Y - GAME_BALL_SIZE)  // Limit ball movement from down side of field
		{
			current_movement_y = GAME_UP;
			current_angle = rand() % 2;
		}

		if(ball.y_cord <= GAME_FIELD_MIN_Y + 1)  // Limit ball movement from up side of field
		{
			current_movement_y = GAME_DOWN;
			current_angle = rand() % 2;
		}


		ssd1306_draw_rectangle(ball.x_cord, ball.y_cord, GAME_BALL_SIZE, GAME_BALL_SIZE, OLED_COLOR_BLACK); // Delete last ball position

		if(current_movement_x == GAME_RIGHT)  // Update ball position in ball structure
		{
			if(current_angle == 0)
			{
				ball.x_cord += game_ball_speed;
			}
			else if(current_angle == 1)
			{
				ball.x_cord += game_ball_speed / 2;
			}
		}
		else if(current_movement_x == GAME_LEFT)
		{
			if(current_angle == 0)
			{
				ball.x_cord -= game_ball_speed;
			}
			else if(current_angle == 1)
			{
				ball.x_cord -= game_ball_speed / 2;
			}

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

		if(ball_speed_time + 10000 < HAL_GetTick())  // Ball will be faster after 10s and 20s from beginning of round
		{
			game_ball_speed = GAME_BALL_SPEED_3;
		}
		else if(ball_speed_time + 20000 < HAL_GetTick())
		{
			game_ball_speed = GAME_BALL_SPEED_4;
		}

		ssd1306_draw_rectangle(ball.x_cord, ball.y_cord, GAME_BALL_SIZE, GAME_BALL_SIZE, OLED_COLOR_WHITE);   // Update last ball position
		default_field_screen();
		ssd1306_display_show();

	}

	// Routine after match
	end_match_screen();
	add_result_to_table();
	game_default_config();  // Reset game structure

	while(button_read(BUTTON_ACCEPT) != BUTTON_PUSHED);
}


/*
 * Setting default parameters of ball structure (center of the field)
 */
static void ball_default_position(void)
{
	ball.x_cord = GAME_FIELD_CENTRE_X;
	ball.y_cord = GAME_FIELD_CENTRE_Y;
}

/*
 * Setting default parameters of game structure
 */
static void players_default_position(void)
{
	game.player1_position = GAME_PLAYER_DEFAULT_POSITION;
	game.player2_position = GAME_PLAYER_DEFAULT_POSITION;
}

/*
 *  Clean racket/s from current position
 */
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

/*
 *  Setting field in default position
 */
static void default_field_screen(void)
{
	ssd1306_display_clear();

	ssd1306_draw_horizontal_line(2, 3, 124, OLED_COLOR_WHITE);  // Horizontal line in top of field
	ssd1306_draw_horizontal_line(2, 61, 124, OLED_COLOR_WHITE);  // Horizontal line in bottom of field


	oled_draw_string(44, 4, game.player1_name, 1, OLED_COLOR_WHITE);  // Player 1 nickname on field
	oled_draw_string(66, 4, game.player2_name, 1, OLED_COLOR_WHITE);  // Player 2 nickname on field

	oled_draw_char(50, 12, int_to_char[game.player1_points], 1, OLED_COLOR_WHITE);  // Player 1 points on field
	oled_draw_char(72, 12, int_to_char[game.player2_points], 1, OLED_COLOR_WHITE);  // Player 2 points on field

	// Draw vertical line in center of OLED display
	for(int i = 4; i <= 60; i += 16)
	{
		ssd1306_draw_rectangle(63, i, 2, 8, OLED_COLOR_WHITE);
	}

	// Set rackets in actual position
	ssd1306_draw_rectangle(GAME_PLAYER1_X_POS, game.player1_position, GAME_RACKET_WIDTH, GAME_RACKET_HEIGHT, OLED_COLOR_WHITE);
	ssd1306_draw_rectangle(GAME_PLAYER2_X_POS, game.player2_position, GAME_RACKET_WIDTH, GAME_RACKET_HEIGHT, OLED_COLOR_WHITE);

	// Set ball in actual position
	ssd1306_draw_rectangle(ball.x_cord, ball.y_cord, GAME_BALL_SIZE, GAME_BALL_SIZE, OLED_COLOR_WHITE);
}

/*
 *  Show on display winner of the round
 */
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

/*
 *  Show on display result of match
 */
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

/*
 *  Add result of last game to last_results structure
 */
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
