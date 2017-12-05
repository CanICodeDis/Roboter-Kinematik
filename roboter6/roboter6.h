#ifndef _ROBOTER6_H
#define _ROBOTER6_H

#include"../Transformation/Transformation.h"
#include"../gelenk/gelenk.h"
#include<vector>
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

	public:

	roboter6 (const symtype target);
	roboter6();
	~roboter6()=default;
	
	void buildRobot();
	void BasKin1 ();
	void BasKin2 ();
	void BasKin3 ();
	void BasKin4 ();
};

#endif
