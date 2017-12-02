#include<armadillo>
#include<iostream>
#include<math.h>
#include"gelenk/gelenk.h"
#include"defines/defines.h"
#include"Transformation/Transformation.h"
using namespace std;
//using namespace arma;
int main()
{
	gelenk s1(1);
	gelenk s2(2);
	s2.minThetaIs(0.0);
	s2.maxThetaIs(90.0);
	s2.minAlphaIs(0.0);
	s2.maxAlphaIs(90.0);
	s2.alphaIs(0.0);
	s2.minRis(0.0);
	s2.maxRis(500.0);
	s2.minDis(0.0);
	s2.maxDis(500.0);
	s2.rIs(34.0);
	s2.dIs(65.0);
	s2.thetaIs(45.0);
	trmat T12 (s2);
	cout<<T12.transform(s2)<<endl;
}
