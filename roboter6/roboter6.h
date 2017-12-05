#ifndef _ROBOTER6_H
#define _ROBOTER6_H

#include"../Transformation/Transformation.h"
#include"../gelenk/gelenk.h"
#include<vector>
#include<stdexcept>
#include<unordered_map>
using namespace std;

enum simtype {value,target };

class roboter6
{
	private:
	vector<gelenk> gelenke;
	unordered_map<int,trmat> systemTransform;
	trmat Endeffektor;
	simtype target;
	bool isBuilt;
	void abn(std::ifstream& s, std::string& a, std::string& b, int& n);

	public:
	roboter6 (const char* MODEL_FILE);
	roboter6 (const simtype target);
	roboter6();
	~roboter6()=default;
	
	gelenk& getGelenk(int n);
	void BasKin1 ();
	void BasKin2 ();
	void BasKin3 ();
	void BasKin4 ();
};

#endif
