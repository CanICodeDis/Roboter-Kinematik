#ifndef _ROBOTER6_H
#define _ROBOTER6_H

#include"../Transformation/Transformation.h"
#include"../gelenk/gelenk.h"
#include<vector>
#include<unordered_map>
#include<cmath>
using namespace std;

enum simtype {value,target };

class roboter6
{
	private:

	vector<gelenk> gelenke;
	Col<double> posHW;
	vector<Col<double>> posS1;
	unordered_map<string,trmat> systemTransform;
	trmat Endeffektor;
	simtype target;

	double theta1BasKin1 [2];
	double theta2BasKin2 [4];
	double theta3BasKin2 [4];
	public:

	roboter6 (const simtype target);
	roboter6();
	~roboter6()=default;
	
	void buildRobot();
	void BasKin1 ();
	void BasKin2 ();
	void BasKin3 ();
	void BasKin4 ();

	gelenk& getGelenk (const int aNummer);
	

};

#endif
