#include "device_driver.h"
#include "frog.h"
#include "car.h"
#include "defined.h"

static int score;
static unsigned short color[] = {RED, YELLOW, GREEN, BLUE, WHITE, BLACK};

// class frog
// {
// 	public:
// 		int x,y;
// 		int w,h;
// 		int ci;
// 		int dir;

// 		frog(int x, int y, int w, int h, int ci, int dir)
// 		{
// 			this->x = x;
// 			this->y = y;
// 			this->w = w;
// 			this->h = h;
// 			this->ci = ci;
// 			this->dir = dir;
// 		}

// 		void Draw_Object()
// 		{
// 			Lcd_Draw_Box(this->x, this->y, this->w, this->h, color[this->ci]);
// 		}

// 		void Frog_Move(int k)
// 		{
// 			if (k == 0 && this->y > Y_MIN)
// 				this->y -= FROG_STEP;
// 			else if (k == 1 && (this->y + this->h < Y_MAX))
// 				this->y += FROG_STEP;
// 			else if (k == 2 && (this->x > X_MIN))
// 				this->x -= FROG_STEP;
// 			else if (k == 3 && (this->x + this->w < X_MAX))	
// 				this->x += FROG_STEP;
// 		}
// };

// class car
// {
// 	public:
// 		int x,y;
// 		int w,h;
// 		int ci;
// 		int dir;

// 		car(int x, int y, int w, int h, int ci, int dir)
// 		{
// 			this->x = x;
// 			this->y = y;
// 			this->w = w;
// 			this->h = h;
// 			this->ci = ci;
// 			this->dir = dir;
// 		}

// 		void Draw_Object()
// 		{
// 			Lcd_Draw_Box(this->x, this->y, this->w, this->h, color[this->ci]);
// 		}

// 		void Car_Move(void)
// 		{
// 			this->x += CAR_STEP * this->dir;
// 			if((this->x + this->w >= X_MAX) || (this->x <= X_MIN)) 
// 				this->dir = -this->dir;
// 		}
// };

static int Check_Collision(frog player, car car1)
{
	int col = 0;

	if((car1.x >= player.x) && ((player.x + FROG_STEP) >= car1.x)) 
		col |= 1<<0;
	else if((car1.x < player.x) && ((car1.x + CAR_STEP) >= player.x)) 
		col |= 1<<0;
	
	if((car1.y >= player.y) && ((player.y + FROG_STEP) >= car1.y)) 
		col |= 1<<1;
	else if((car1.y < player.y) && ((car1.y + CAR_STEP) >= player.y)) 
		col |= 1<<1;

	if(col == 3)
	{
		Uart_Printf("SCORE = %d\n", score);	
		return GAME_OVER;
	}

	if((player.dir == SCHOOL) && (player.y == Y_MIN)) 
	{
		Uart_Printf("SCHOOL\n");		
		player.dir = HOME;
	}

	if((player.dir == HOME) && (player.y == LCDH - player.h))
	{
		player.dir = SCHOOL;
		score++;
		Uart_Printf("HOME, %d\n", score);
	}

	return 0;
}

static void Game_Init(frog player, car car1)
{
	score = 0;
	Lcd_Clr_Screen();
	Lcd_Draw_Box(player.x, player.y, player.w, player.h, color[player.ci]);
	Lcd_Draw_Box(car1.x, car1.y, car1.w, car1.h, color[car1.ci]);
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
				player.ci = BACK_COLOR;
				player.Draw_Object();

				player.Frog_Move(Jog_key);
				game_over = Check_Collision(player, car1);

				player.ci = FROG_COLOR;
				player.Draw_Object();
				Jog_key_in = 0;				
			}

			if(TIM4_expired) 
			{
				car1.ci = BACK_COLOR;
				car1.Draw_Object();

				car1.Car_Move();
				game_over = Check_Collision(player, car1);

				car1.ci = CAR_COLOR;
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
