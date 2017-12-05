#ifndef __VISUALIZATION_H__
#define __VISUALIZATION_H__

#include <armadillo>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include "../defines/defines.h"
#include "camera.h"


//The main loop will not exit until this variable is set to false:
extern SDL_sem* running;

extern int thVisual(void* data);

/*__VISUALIZATION_H__*/
#endif
