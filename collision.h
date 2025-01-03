#ifndef COLLISION_H
#define COLLISION_H

#include "frog.h"
#include "car.h"
#include "cherry.h"
#include "defined.h"
#include "device_driver.h"

int Check_Collision(frog *player, car cars[5], cherry &cherry_, int &score, int &foodCount, int &mapNum);
int Check_Collision(frog *player, car cars[5], int &score, int &foodCount, int &mapNum);
int Check_Collision_Cherry(frog *player, cherry &cherry_, int &foodCount, int &mapNum);

#endif