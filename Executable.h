#ifndef __EXECUTABLE_H__
#define __EXECUTABLE_H__

#include <armadillo>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>

//The main loop will not exit until this variable is set to false:
extern SDL_sem* running;

#include "camera/visualization.h"
#include "model/robot.h"

/* __EXECUTABLE_H__ */
#endif
