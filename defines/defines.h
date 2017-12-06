#ifndef _DEFINES_H
#define _DEFINE_H

#include <math.h>
#include <string>

// THIS SECTION IS THE USER CONFIGURATION
#define TFONT_NAME "assets/fonts/DejaVuSansMono.ttf"
#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 480

//DO NOT CHANGE ANYTHING BELOW

#define VERSION "BETA 1.0"
#define _DEG2RAD(a) ((a) * M_PI / 180.0)
#define _RAD2DEG(a) ((a) * (180.0/M_PI))
#define rethrow catch(...) { throw; }
#endif
