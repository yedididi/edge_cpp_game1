#include "device_driver.h"
#include "frog.h"
#include "car.h"
#include "defined.h"
#include "map.h"
#include "graphics.h"
#include "cherry.h"

#define FOODNUM 3

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
int cherryX[2][3] = {

};
int cherryY[2][3] = {

};

extern "C" void __cxa_pure_virtual() { while (1); }

extern "C" void __cxa_deleted_virtual() { while (1); }

namespace __gnu_cxx {
void _verbose_terminate_handler() {
  Uart1_Printf("Unexpected error\n");
  while (1);
}
}  

static int Check_Collision(frog *player, car cars[5])
{
	int col = 0;

	for (int i = 0; i < score + 1; i++)
	{
		col = 0;
		if ((cars[i].getX() >= player->getX()) && ((player->getX() + FROG_SIZE_X) > cars[i].getX())) 
			col |= 1<<0; //col = col | 1
		else if ((cars[i].getX() < player->getX()) && ((cars[i].getX() + CAR_SIZE_X) > player->getX())) 
			col |= 1<<0; //col = col | 1
		
		if ((cars[i].getY() >= player->getY()) && ((player->getY() + FROG_SIZE_Y) > cars[i].getY())) 
			col |= 1<<1; //col = col | 10
		else if ((cars[i].getY() < player->getY()) && ((cars[i].getY() + CAR_SIZE_Y) > player->getY())) 
			col |= 1<<1; //col = col | 10
		
		if (col == 3)
			break;
	}

	if(col == 3)
	{
		Uart_Printf("SCORE = %d\n", score);	
		return GAME_OVER;
	}

	if((player->getDir() == SCHOOL) && (player->getY() == Y_MIN)) 
	{
		Uart_Printf("SCHOOL\n");
		player->setDir(HOME);
	}

	if((player->getDir() == HOME) && (player->getY() == LCDH - player->getH()) && !foodCount)
	{
		player->setDir(SCHOOL);
		score++;
		Uart_Printf("HOME, %d\n", score);
	}

	return 0;
}

static int Check_Collision_Cherry(frog *player, cherry cherry_)
{
	for (int i = 0; i < 3; i++)
	{
		Uart_Printf("cherry count:%d\n", i);

		if (player->getX() == (cherry_.x[mapNum][i] * 10) && player->getY() == (cherry_.y[mapNum][i] * 10))
		{
			Uart_Printf("CHERRY GOT EATEN\n");
			foodCount--;
			return (i + 1);
		}
	}
	return 0;
}

void Lcd_Draw_Cherry(int x, int y)
{
	int cherry[10][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 3, 0, 0, 0, 0, 0},
        {0, 0, 0, 3, 0, 3, 0, 0, 0, 0},
        {0, 0, 3, 0, 0, 0, 3, 0, 0, 0},
        {0, 0, 3, 0, 0, 0, 0, 3, 0, 0},
        {0, 2, 2, 2, 0, 0, 2, 2, 2, 0},
        {0, 2, 2, 2, 0, 0, 2, 2, 2, 0},
        {0, 2, 2, 2, 0, 0, 2, 2, 2, 0},
        {0, 2, 2, 2, 0, 0, 2, 2, 2, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };


	// Uart1_Printf("cherry.x:%d, cherry.y:%d\n", x, y);
	for (int i = 0; i < FROG_SIZE_Y; i++) 
    {
        for (int j = 0; j < FROG_SIZE_X; j++) 
        {
            if (cherry[i][j] == 2)
                Lcd_Put_Pixel(x + j, y + i, RED);
            else if (cherry[i][j] == 3)
                Lcd_Put_Pixel(x + j, y + i, GREEN);
        }
    }
}

static void Game_Init(frog player, car car1)
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
}

extern volatile int TIM4_expired;
extern volatile int USART1_rx_ready;
extern volatile int USART1_rx_data;
extern volatile int Jog_key_in;
extern volatile int Jog_key;

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
		frog player(150, 230, FROG_SIZE_X, FROG_SIZE_Y, PACMAN_COLOR, SCHOOL);
		car car_array[5] = {
			car(enemyX[mapNum][0], enemyY[mapNum][0], CAR_SIZE_X, CAR_SIZE_Y, CAR_COLOR, RIGHT),
			car(enemyX[mapNum][1], enemyY[mapNum][1], CAR_SIZE_X, CAR_SIZE_Y, BACK_COLOR, RIGHT),
			car(enemyX[mapNum][2], enemyY[mapNum][2], CAR_SIZE_X, CAR_SIZE_Y, BACK_COLOR, RIGHT),
			car(enemyX[mapNum][3], enemyY[mapNum][3], CAR_SIZE_X, CAR_SIZE_Y, BACK_COLOR, RIGHT),
			car(enemyX[mapNum][4], enemyY[mapNum][4], CAR_SIZE_X, CAR_SIZE_Y, BACK_COLOR, RIGHT)
		};
		Game_Init(player, car_array[0]);
		TIM4_Repeat_Interrupt_Enable(1, TIMER_PERIOD*10);

		for(;;)
		{
			cherry cherry_;
			for (int i = 0; i < 3; i++)
				cherry_.drawCherry(i, mapNum);
			int game_over = 0;
			
			if(Jog_key_in)
			{
				Uart_Printf("KEY\n");
				player.setCi(BACK_COLOR);
				player.Draw_Object();

				player.Frog_Move(Jog_key, map[mapNum]);
				game_over = Check_Collision(&player, car_array);
				if (foodCount)
					cherry_.eraseCherry(Check_Collision_Cherry(&player, cherry_), mapNum);

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
				game_over = Check_Collision(&player, car_array);
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
				for (int i = 0; i < score + 1; i++)
				{
					car_array[i].setCi(BACK_COLOR);
					car_array[i].Draw_Object();
				}
				player.setCi(BACK_COLOR);
				player.Draw_Object();
				Uart_Printf("Game Clear, Please press any key to continue.\n");
				Jog_Wait_Key_Pressed();
				Jog_Wait_Key_Released();
				Uart_Printf("Game Start\n");
				break;
			}
		}
	}
}
