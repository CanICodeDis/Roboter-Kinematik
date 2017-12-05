#ifndef __MODEL_ROBOT_H__
#define __MODEL_ROBOT_H__

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include "../bdouble/BoundDouble.h"
#include "../defines/defines.h"
#include "../gelenk/gelenk.h"
#include "../roboter6/roboter6.h"

namespace ModelLoader {
	void loadFile(const char* MODEL_FILE, roboter6& roboter);
}

/* __MODEL_ROBOT_H__ */
#endif
