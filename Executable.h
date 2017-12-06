#ifndef __EXECUTABLE_H__
#define __EXECUTABLE_H__

#include <armadillo>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h>
#include <string>

//The main loop will not exit until this variable is set to false:
extern SDL_sem* running;

#include "defines/defines.h"
#include "Transformation/Transformation.h"
#include "gelenk/gelenk.h"
#include "roboter6/roboter6.h"
#include "threadqueue/ThreadMessageQueue.h"

//the target robot we will play with
extern roboter6* roboter;

extern ThreadMessageQueue toVis;
extern ThreadMessageQueue toCli;

#include "camera/visualization.h"

/* __EXECUTABLE_H__ */
#endif
