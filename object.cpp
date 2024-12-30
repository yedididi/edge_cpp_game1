#include "object.h"

static unsigned short color_o[] = {RED, YELLOW, GREEN, BLUE, WHITE, BLACK};

object::object()
{
    
}

object::object(int x, int y, int w, int h, int ci, int dir)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->ci = ci;
    this->dir = dir;
}

void object::Draw_Object()
{
    Lcd_Draw_Box(this->x, this->y, this->w, this->h, color_o[this->ci]);
}