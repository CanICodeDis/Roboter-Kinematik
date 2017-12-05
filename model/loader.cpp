#include "robot.h"

Robot::Robot(const char* MODEL_FILE) {
	std::string FILE_HEADER = "ROBMDL ";
	FILE_HEADER += VERSION;
	bool properTermination=false:
	try {
		std::ifstream fin (MODEL_FILE, std::ifstream::in);
		std::string line;
		std::getline(fin, line);

		if (line != FILE_HEADER) {
			throw std::runtime_error ( string("Invalid model header for ")+MODEL_FILE );
		}

		while (fin.good()) {
			std::getline(fin, line);
			if (line == "JOINTS") {

				gelenk* gelenk = new gelenk(fin);
				//joints.push_back(gelenk(fin));
			}
		}

	} rethrow
}
