#include "defined.h"
#include "car.h"

car::car(int x, int y, int w, int h, int ci, int dir) : object(x, y, w, h, ci, dir)
{
    // this->x = x;
    // this->y = y;
    // this->w = w;
    // this->h = h;
    // this->ci = ci;
    // this->dir = dir;
}

void car::Car_Move(void)
{
    this->x += CAR_STEP * this->dir;
    if((this->x + this->w >= X_MAX) || (this->x <= X_MIN)) 
        this->dir = -this->dir;
}