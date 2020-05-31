#pragma once

#include "resource.h"

#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3
#define DIR_UNDEF 4

int xP, yP, xE, yE;
const int size = 20;
int score;
int timecount = 60;
int direction = DIR_UNDEF;
unsigned long rawTimerCounter = 0;