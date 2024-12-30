#ifndef OBJECT_H
#define OBJECT_H

#include "lcd.h"

class object
{
    public:
		int x,y;
		int w,h;
		int ci;
		int dir;

		object();
		object(int x, int y, int w, int h, int ci, int dir);

		void Draw_Object();
};

#endif