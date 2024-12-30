#include "device_driver.h"
#include "frog.h"
#include "car.h"
#include "defined.h"

static int score;
static unsigned short color[] = {RED, YELLOW, GREEN, BLUE, WHITE, BLACK};

static int Check_Collision(frog player, car car1)
{
	int col = 0;

	if((car1.getX() >= player.getX()) && ((player.getX() + FROG_STEP) >= car1.getX())) 
		col |= 1<<0;
	else if((car1.getX() < player.getX()) && ((car1.getX() + CAR_STEP) >= player.getX())) 
		col |= 1<<0;
	
	if((car1.getY() >= player.getY()) && ((player.getY() + FROG_STEP) >= car1.getY())) 
		col |= 1<<1;
	else if((car1.getY() < player.getY()) && ((car1.getY() + CAR_STEP) >= player.getY())) 
		col |= 1<<1;

	if(col == 3)
	{
		Uart_Printf("SCORE = %d\n", score);	
		return GAME_OVER;
	}

	if((player.getDir() == SCHOOL) && (player.getY() == Y_MIN)) 
	{
		Uart_Printf("SCHOOL\n");
		player.setDir(HOME);
	}

	if((player.getDir() == HOME) && (player.getY() == LCDH - player.getH()))
	{
		player.setDir(SCHOOL);
		score++;
		Uart_Printf("HOME, %d\n", score);
	}

	return 0;
}

static void Game_Init(frog player, car car1)
{
	score = 0;
	Lcd_Clr_Screen();
	Lcd_Draw_Box(player.getX(), player.getY(), player.getW(), player.getH(), color[player.getCi()]);
	Lcd_Draw_Box(car1.getX(), car1.getY(), car1.getW(), car1.getH(), color[car1.getCi()]);
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
		frog player(150, 220, FROG_SIZE_X, FROG_SIZE_Y, FROG_COLOR, SCHOOL);
		car car1(0, 110, CAR_SIZE_X, CAR_SIZE_Y, CAR_COLOR, RIGHT);
		Game_Init(player, car1);
		TIM4_Repeat_Interrupt_Enable(1, TIMER_PERIOD*10);

		for(;;)
		{
			int game_over = 0;
			
			if(Jog_key_in) 
			{
				Uart_Printf("KEY\n");
				player.setCi(BACK_COLOR);
				player.Draw_Object();

				player.Frog_Move(Jog_key);
				game_over = Check_Collision(player, car1);

				player.setCi(FROG_COLOR);
				player.Draw_Object();
				Jog_key_in = 0;				
			}

			if(TIM4_expired) 
			{
				car1.setCi(BACK_COLOR);
				car1.Draw_Object();

				car1.Car_Move();
				game_over = Check_Collision(player, car1);

				car1.setCi(CAR_COLOR);
				car1.Draw_Object();
				TIM4_expired = 0;
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
		}
	}
}
