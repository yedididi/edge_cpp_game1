#include "defined.h"
#include "car.h"
#include "device_driver.h"

car::car(int x, int y, int w, int h, int ci, int dir) : object(x, y, w, h, ci, dir)
{
}

void car::Car_Move(char map[24][32])
{
    if (map[this->y / 10][(this->x + (CAR_STEP * this->dir)) / 10] == '1')
        this->dir = -this->dir;
    else if (((this->x + (CAR_STEP * this->dir)) % 10 != 0) && (map[this->y / 10][(this->x + (CAR_STEP * this->dir * 2)) / 10] == '1'))
        this->dir = -this->dir;
    else
        this->x += CAR_STEP * this->dir;
}