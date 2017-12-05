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

class Robot {
public:
	std::vector<gelenk> joints;
//	std::vector<Vertex> vertices;
//	std::vector<Polygon> polygons;
	Robot(const char* MODEL_FILE);
	inline ~Robot() {}
};

/* __MODEL_ROBOT_H__ */
#endif
