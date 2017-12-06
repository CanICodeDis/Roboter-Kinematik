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
#include "../bdouble/BoundDouble.h"
#include "../gelenk/gelenk.h"
#include "../roboter6/roboter6.h"
#include "../Executable.h"
#include "camera.h"
#include "../threadqueue/ThreadMessageQueue.h"

//The main loop will not exit until this variable is set to false:
extern SDL_sem* running;

extern int thVisual(void* data);

extern ThreadMessageQueue toVis;
extern ThreadMessageQueue toCli;

/*__VISUALIZATION_H__*/
#endif
