#include "game_interface.h"


GAME_TypeDef last_results[3] = {0};

/*
 *  Static functions prototypes
 */

static void start_game_screen(void);
static void mian_menu_screen(void);
static void play_state(void);
static void settings_state(void);
static void results_state(void);

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

	start_game_screen();

	while(button_read(BUTTON_ACCEPT) != BUTTON_PUSHED);  // Click accept button to go to main menu
	HAL_Delay(100);  // debouncing protection and break after click

	mian_menu_screen();

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
			HAL_Delay(100);  // debouncing protection and break after click
		}

		if(game_state != last_game_state)
		{
			last_game_state = game_state;

			if(game_state == GAME_STATE_PLAY)
			{
				ssd1306_draw_horizontal_line(42, 15, 44, OLED_COLOR_WHITE);
				ssd1306_draw_horizontal_line(20, 31, 88, OLED_COLOR_BLACK);
				ssd1306_display_show();
			}
			else if(game_state == GAME_STATE_SETTINGS)
			{
				ssd1306_draw_horizontal_line(20, 31, 88, OLED_COLOR_WHITE);
				ssd1306_draw_horizontal_line(42, 15, 44, OLED_COLOR_BLACK);
				ssd1306_draw_horizontal_line(25, 47, 77, OLED_COLOR_BLACK);
				ssd1306_display_show();
			}
			else if(game_state == GAME_STATE_RESULTS)
			{
				ssd1306_draw_horizontal_line(25, 47, 77, OLED_COLOR_WHITE);
				ssd1306_draw_horizontal_line(20, 31, 88, OLED_COLOR_BLACK);
				ssd1306_draw_horizontal_line(42, 63, 44, OLED_COLOR_BLACK);
				ssd1306_display_show();
			}
			else if(game_state == GAME_STATE_EXIT)
			{
				ssd1306_draw_horizontal_line(42, 63, 44, OLED_COLOR_WHITE);
				ssd1306_draw_horizontal_line(25, 47, 77, OLED_COLOR_BLACK);
				ssd1306_display_show();
			}
			HAL_Delay(100);  // debouncing protection and break after click
		}

	}
}


static void start_game_screen(void)
{
	ssd1306_display_clear();
	oled_draw_string(12, 16, "START", 4, OLED_COLOR_WHITE);
	ssd1306_draw_horizontal_line(12, 46, 105, OLED_COLOR_WHITE);
	ssd1306_display_show();
}

static void mian_menu_screen(void)
{
	ssd1306_display_clear();
	oled_draw_string(42, 0, "PLAY", 2, OLED_COLOR_WHITE);
	oled_draw_string(20, 16, "SETTINGS", 2, OLED_COLOR_WHITE);
	oled_draw_string(25, 32, "RESULTS", 2, OLED_COLOR_WHITE);
	oled_draw_string(42, 48, "EXIT", 2, OLED_COLOR_WHITE);
	ssd1306_display_show();
}

static void play_state(void)
{

}

static void settings_state(void)
{

}

static void results_state(void)
{

}
