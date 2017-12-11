#ifndef _ROBOTER6_H
#define _ROBOTER6_H

#include"../Transformation/Transformation.h"
#include"../gelenk/gelenk.h"
#include"../defines/parse_error.h"
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
	trmat systemTransform [6][8];
	double theta [6][8];
	trmat worldTransform[6];
	
	trmat Endeffektor;//T06
	Mat<double> R36[4];
	simtype target;

	double theta1BasKin1 [2];
	double theta2BasKin2 [4];
	double theta3BasKin2 [4];

	void abn(std::ifstream& s, std::string& a, std::string& b, int& n, int& ln);
	Mat<double> InvEuler323(const int n);

	public:

	roboter6 (const simtype target);
	roboter6 (const char* MODEL_FILE);
	roboter6();
	~roboter6()=default;

	
	void BasKin1 ();
	void BasKin2 ();
	void BasKin3 ();

	gelenk& getGelenk (const int aNummer);
	inline simtype getSimType() {
		return target; 
	}	
	Col<double> giveAnkleNPositionInWorld (const int ankle);
	Mat<double> giveAnkleNRotationInWorld (const int ankle);
	void calcWorldTransformations ();
	void setEndEffektor (const trmat&);
	void giveInverseOptions (double** theta);
//	void sortPossibilities();
	void updateMatrices();

};

#endif
