#include "defined.h"
#include "frog.h"

frog::frog(int x, int y, int w, int h, int ci, int dir) : object(x, y, w, h, ci, dir)
{
    // this->x = x;
    // this->y = y;
    // this->w = w;
    // this->h = h;
    // this->ci = ci;
    // this->dir = dir;
}

void frog::Frog_Move(int k)
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