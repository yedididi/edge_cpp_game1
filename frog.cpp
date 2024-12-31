#include "defined.h"
#include "frog.h"

frog::frog(int x, int y, int w, int h, int ci, int dir) : object(x, y, w, h, ci, dir)
{
}

// bool isWall(int x, int y, char map[24][32])
// {
//     int xNum[9] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
//     int yNum[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
    
//     for (int i = 0; i < 9; i++)
//     {
//         int newX = x + (xNum[i] * 10);
//         int newY = y + (yNum[i] * 10);
//         if (map[newY / 10][newX / 10] == '1')
//             return (true);
//     }
//     return (false);
// }

// void frog::Frog_Move(int k, char map[24][32])
// {
//     //0:up 1:down 2:left 3:right
//     if (k == 0 && this->y > Y_MIN && isWall(this->x, this->y - FROG_STEP, map))
//         this->y -= FROG_STEP;
//     else if (k == 1 && (this->y + this->h < Y_MAX) && isWall(this->x, this->y + FROG_STEP, map))
//         this->y += FROG_STEP;
//     else if (k == 2 && (this->x > X_MIN) && isWall(this->x - FROG_STEP, this->y, map))
//         this->x -= FROG_STEP;
//     else if (k == 3 && (this->x + this->w < X_MAX) && isWall(this->x + FROG_STEP, this->y, map))	
//         this->x += FROG_STEP;
// }

void frog::Frog_Move(int k, char map[24][32])
{
    //0:up 1:down 2:left 3:right
    if (k == 0 && this->y > Y_MIN && map[(this->y - FROG_STEP) / 10][this->x / 10] != '1')
        this->y -= FROG_STEP;
    else if (k == 1 && (this->y + this->h < Y_MAX) && map[(this->y + FROG_STEP) / 10][this->x / 10] != '1')
        this->y += FROG_STEP;
    else if (k == 2 && (this->x > X_MIN) && map[this->y / 10][(this->x - FROG_STEP) / 10] != '1')
        this->x -= FROG_STEP;
    else if (k == 3 && (this->x + this->w < X_MAX) && map[this->y / 10][(this->x + FROG_STEP) / 10] != '1')	
        this->x += FROG_STEP;
}