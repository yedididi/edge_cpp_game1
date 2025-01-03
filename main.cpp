#include "device_driver.h"
#include "frog.h"
#include "car.h"
#include "defined.h"
#include "map.h"
#include "graphics.h"
#include "cherry.h"
#include "collision.h"
#include "arrow.h"

static int score;
static unsigned short color[] = {RED, YELLOW, GREEN, BLUE, WHITE, BLACK};
int foodCount = FOODNUM;
int mapNum = 0;
int enemyX[2][5] = {
	{10, 110, 70, 150, 10},
	{10, 110, 160, 110, 300}
};
int enemyY[2][5] = {
	{110, 160, 220, 70, 20},
	{100, 60, 160, 220, 30}
};

extern "C" void __cxa_pure_virtual() { while (1); }

extern "C" void __cxa_deleted_virtual() { while (1); }

namespace __gnu_cxx 
{
	void _verbose_terminate_handler() 
	{
		Uart1_Printf("Unexpected error\n");
		while (1) ;
	}
}  

static void Game_Init(frog player, car car1, cherry cherry_)
{
	score = 0;
	Lcd_Clr_Screen();

	player.Draw_Object();
	car1.Draw_Object();

	for (int i = 0; i < 24; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			if (map[mapNum][i][j] == '1')
				Lcd_Draw_Box(j * MAP_UNIT, i * MAP_UNIT, FROG_SIZE_X, FROG_SIZE_Y, color[WALL_COLOR]);
		}
	}

	for (int i = 0; i < 3; i++)
		cherry_.drawCherry(i, mapNum);
}

extern volatile int TIM4_expired;
extern volatile int USART1_rx_ready;
extern volatile int USART1_rx_data;
extern volatile int Jog_key_in;
extern volatile int Jog_key;

static void Game_First_Screen(void)
{
	Lcd_Printf(85, 50, WHITE, BLACK, 2, 2, "PACK BOY", 100);
	Lcd_Printf(64, 140, WHITE, BLACK, 2, 2, "MAP1", 100);
	Lcd_Printf(212, 140, WHITE, BLACK, 2, 2, "MAP2", 100);
	for (;;)
	{
		if (Jog_key == 2)
		{
			Lcd_Printf(64, 140, BLACK, WHITE, 2, 2, "MAP1", 100);
			Lcd_Printf(212, 140, WHITE, BLACK, 2, 2, "MAP2", 100);
			Lcd_Draw_Arrow(21, 139);
			Lcd_Draw_Box(168, 139, 36, 36, color[5]);
			mapNum = 0;
		}
		if (Jog_key == 3)
		{
			Lcd_Printf(64, 140, WHITE, BLACK, 2, 2, "MAP1", 100);
			Lcd_Printf(212, 140, BLACK, WHITE, 2, 2, "MAP2", 100);
			Lcd_Draw_Arrow(168, 139);
			Lcd_Draw_Box(21, 139, 36, 36, color[5]);
			mapNum = 1;
		}
		if (Jog_key == 4)
			break;
   	}
}

extern "C" void abort(void)
{
  while (1);
}

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);

	SCB->VTOR = 0x08003000;
	SCB->SHCSR = 7<<16;	
}

extern "C" void Main()
{
	Sys_Init();
	Uart_Printf("Game Example\n");

	Lcd_Init();
	Jog_Poll_Init();
	Jog_ISR_Enable(1);
	Uart1_RX_Interrupt_Enable(1);

	for(;;)
	{
		Lcd_Clr_Screen();
		Game_First_Screen();
		frog player(150, 230, FROG_SIZE_X, FROG_SIZE_Y, PACMAN_COLOR, SCHOOL);
		car car_array[5] = {
			car(enemyX[mapNum][0], enemyY[mapNum][0], CAR_SIZE_X, CAR_SIZE_Y, CAR_COLOR, RIGHT),
			car(enemyX[mapNum][1], enemyY[mapNum][1], CAR_SIZE_X, CAR_SIZE_Y, BACK_COLOR, RIGHT),
			car(enemyX[mapNum][2], enemyY[mapNum][2], CAR_SIZE_X, CAR_SIZE_Y, BACK_COLOR, RIGHT),
			car(enemyX[mapNum][3], enemyY[mapNum][3], CAR_SIZE_X, CAR_SIZE_Y, BACK_COLOR, RIGHT),
			car(enemyX[mapNum][4], enemyY[mapNum][4], CAR_SIZE_X, CAR_SIZE_Y, BACK_COLOR, RIGHT)
		};
		cherry cherry_;
			
		Game_Init(player, car_array[0], cherry_);
		TIM4_Repeat_Interrupt_Enable(1, TIMER_PERIOD*10);
		foodCount = FOODNUM;

		for(;;)
		{
			int game_over = 0;
			
			if(Jog_key_in)
			{
				Uart_Printf("KEY\n");
				player.setCi(BACK_COLOR);
				player.Draw_Object();

				player.Frog_Move(Jog_key, map[mapNum]);
				game_over = Check_Collision(&player, car_array, cherry_, score, foodCount, mapNum);

				if (foodCount)
					cherry_.eraseCherry(Check_Collision_Cherry(&player, cherry_, foodCount, mapNum), mapNum);

				player.setCi(FROG_COLOR);
				player.Draw_Object();
				Jog_key_in = 0;				
			}

			if(TIM4_expired) 
			{
				for (int i = 0; i < score + 1; i++)
				{
					car_array[i].setCi(BACK_COLOR);
					car_array[i].Draw_Object();

					car_array[i].Car_Move(map[mapNum]);

					car_array[i].setCi(CAR_COLOR);
					car_array[i].Draw_Object();
					TIM4_expired = 0;
				}
				game_over = Check_Collision(&player, car_array, score, foodCount, mapNum);
			}

			if(game_over)
			{
				TIM4_Repeat_Interrupt_Enable(0, 0);
				Uart_Printf("Game Over, Please press any key to continue.\n");
				Jog_Wait_Key_Pressed();
				Jog_Wait_Key_Released();
				Uart_Printf("Game Start\n");
				break;
			}

			if (score == TOTAL_LEVEL)
			{
				Lcd_Clr_Screen();
				Game_First_Screen();
				break;
			}
		}
	}
}
