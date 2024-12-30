#include "device_driver.h"

#define LCDW			(320)
#define LCDH			(240)
#define X_MIN	 		(0)
#define X_MAX	 		(LCDW - 1)
#define Y_MIN	 		(0)
#define Y_MAX	 		(LCDH - 1)

#define TIMER_PERIOD	(10)
#define RIGHT			(1)
#define LEFT			(-1)
#define HOME			(0)
#define SCHOOL			(1)

#define CAR_STEP		(10)
#define CAR_SIZE_X		(20)
#define CAR_SIZE_Y		(20)
#define FROG_STEP		(10)
#define FROG_SIZE_X		(20)
#define FROG_SIZE_Y		(20)

#define BACK_COLOR		(5)
#define CAR_COLOR		(0)
#define FROG_COLOR		(1)

#define GAME_OVER		(1)

static int score;
static unsigned short color[] = {RED, YELLOW, GREEN, BLUE, WHITE, BLACK};

class frog
{
	public:
		int x,y;
		int w,h;
		int ci;
		int dir;

		frog(int x, int y, int w, int h, int ci, int dir)
		{
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
			this->ci = ci;
			this->dir = dir;
		}

		void Draw_Object()
		{
			Lcd_Draw_Box(this->x, this->y, this->w, this->h, color[this->ci]);
		}

		void Frog_Move(int k)
		{
			if (k == 0 && this->y > Y_MIN)
				this->y -= FROG_STEP;
			else if (k == 1 && (this->y + this->h < Y_MAX))
				this->y += FROG_STEP;
			else if (k == 2 && (this->x > X_MIN))
				this->x -= FROG_STEP;
			else if (k == 3 && (this->x + this->w < X_MAX))	
				this->x += FROG_STEP;
		}
};

class car
{
	public:
		int x,y;
		int w,h;
		int ci;
		int dir;

		car(int x, int y, int w, int h, int ci, int dir)
		{
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
			this->ci = ci;
			this->dir = dir;
		}

		void Draw_Object()
		{
			Lcd_Draw_Box(this->x, this->y, this->w, this->h, color[this->ci]);
		}

		
};

// typedef struct
// {
// 	int x,y;
// 	int w,h;
// 	int ci;
// 	int dir;
// }QUERY_DRAW;

// static QUERY_DRAW car;
// static QUERY_DRAW frog;

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

// static int Car_Move(void)
// {
// 	car.x += CAR_STEP * car.dir;
// 	if((car.x + car.w >= X_MAX) || (car.x <= X_MIN)) car.dir = -car.dir;
// 	return Check_Collision();
// }

// static void k0(void)
// {
// 	if(frog.y > Y_MIN) frog.y -= FROG_STEP;
// }

// static void k1(void)
// {
// 	if(frog.y + frog.h < Y_MAX) frog.y += FROG_STEP;
// }

// static void k2(void)
// {
// 	if(frog.x > X_MIN) frog.x -= FROG_STEP;
// }

// static void k3(void)
// {
// 	if(frog.x + frog.w < X_MAX) frog.x += FROG_STEP;
// }

// static int Frog_Move(int k)
// {
// 	// UP(0), DOWN(1), LEFT(2), RIGHT(3)
// 	static void (*key_func[])(void) = {k0, k1, k2, k3};
// 	if(k <= 3) key_func[k]();
// 	return Check_Collision();
// }

static void Game_Init(frog player, car car1)
{
	score = 0;
	Lcd_Clr_Screen();
	// frog.x = 150; frog.y = 220; frog.w = FROG_SIZE_X; frog.h = FROG_SIZE_Y; frog.ci = FROG_COLOR; frog.dir = SCHOOL;
	// car.x = 0; car.y = 110; car.w = CAR_SIZE_X; car.h = CAR_SIZE_Y; car.ci = CAR_COLOR; car.dir = RIGHT;
	Lcd_Draw_Box(player.x, player.y, player.w, player.h, color[player.ci]);
	Lcd_Draw_Box(car1.x, car1.y, car1.w, car1.h, color[car1.ci]);
}

// static void Draw_Object(QUERY_DRAW * obj)
// {
// 	Lcd_Draw_Box(obj->x, obj->y, obj->w, obj->h, color[obj->ci]);
// }

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

			// if(TIM4_expired) 
			// {
			// 	car.ci = BACK_COLOR;
			// 	Draw_Object(&car);
			// 	game_over = Car_Move();
			// 	car.ci = CAR_COLOR;
			// 	Draw_Object(&car);
			// 	TIM4_expired = 0;
			// }

			// if(game_over)
			// {
			// 	TIM4_Repeat_Interrupt_Enable(0, 0);
			// 	Uart_Printf("Game Over, Please press any key to continue.\n");
			// 	Jog_Wait_Key_Pressed();
			// 	Jog_Wait_Key_Released();
			// 	Uart_Printf("Game Start\n");
			// 	break;
			// }
		}
	}
}
