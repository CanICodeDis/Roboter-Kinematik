#include "robot.h"

void ModelLoader::abn(std::ifstream& s, std::string& a, std::string& b, int& n) {
	fin >> a >> b;
	if (fin.peek()==' ') fin >> n; else n = -1;
}
void ModelLoader::loadFile(const char* MODEL_FILE, robot6& rob) {
	std::string FILE_HEADER = "ROBMDL ";
	FILE_HEADER += VERSION;

	try {
		std::ifstream fin (MODEL_FILE, std::ifstream::in);
		std::string line,a,b; //block tokens
		int n; //blok param
		std::getline(fin, line);

		if (line != FILE_HEADER) {
			throw std::runtime_error ( string("Invalid model header for ")+MODEL_FILE );
		}

		while (fin.good()) {
			abn(fin, a, b, n);
			if (a == "BEGIN") {
				if (b == "ROBOT") {
					rob = roboter6( simtype(n) );
	
					abn(fin, a, b, n);
					while (a != "END" && b != "ROBOT") {
	
						if (a == "BEGIN" && b == "JOINT") {
							gelenk g = rob.getGelenk(n);
							double min,max,val,spd;
							std::string target;
							
							for (int i = 0; i < 4; i++) {
								
								fin >> target >> min;
								if (target == "Alpha") {
									g.alpha = min;
								} else if (target == "TransX") {
									g.r = min;
								} else if (target == "Theta") {
									fin >> max >> val >> spd;
									g.theta = bDouble(min, max, val, spd);
								} else if (target == "TransZ") {
									fin >> max >> val >> spd;
									g.h = bDouble(min, max, val, spd);
								} else throw std::runtime_error( "Invalid value target in JOINT block" );
							}
							abn(fin, a, b, n);
							if (a != "END" || b != "JOINT") throw std::runtime_error( "JOINT does not end after values" );
						} else throw new std::runtime_error( "Invalid block within ROBOT, begin JOINT or end ROBOT" );
	
						abn(fin, a, b, n);
					}
				} else throw std::runtime_error( "Unknown begin block" );
			} else throw std::runtime_error( "At root level a block begin is expected" );
		}
	} rethrow
}
