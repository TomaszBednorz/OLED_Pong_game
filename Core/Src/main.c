#include "main.h"


int main(void)
{
	HAL_Init();
	clock_configuration();

	// Basic initializations - START
	i2c1_init();
	ssd1306_init();
	gpio_init();
	// Basic initializations - END

	srand(time(NULL));


	while (1)
	{
		main_menu();
	}
}


