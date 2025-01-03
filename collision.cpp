#include "collision.h"

int Check_Collision(frog *player, car cars[5], cherry &cherry_, int &score, int &foodCount, int &mapNum)
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
		for (int i = 0; i < 3; i++)
		{
			cherry_.drawCherry(i, mapNum);
			cherry_.cherryExistance[i] = true;
		}
		foodCount = FOODNUM;
		Uart1_Printf("foodCount is:%d\n", foodCount);
		Uart_Printf("HOME, %d\n", score);

	}

	return 0;
}


int Check_Collision(frog *player, car cars[5], int &score, int &foodCount, int &mapNum)
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

int Check_Collision_Cherry(frog *player, cherry &cherry_, int &foodCount, int &mapNum)
{
	for (int i = 0; i < 3; i++)
	{
		if ((player->getX() == (cherry_.x[mapNum][i] * 10)) && (player->getY() == (cherry_.y[mapNum][i] * 10)))
		{
			if (cherry_.cherryExistance[i] == true)
			{
				Uart_Printf("CHERRY GOT EATEN\n");
				foodCount--;
				Uart1_Printf("foodCount is:%d\n", foodCount);
				cherry_.cherryExistance[i] = false;
				return (i + 1);
			}
		}
	}
	return 0;
}