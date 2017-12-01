#include<armadillo>
#include<iostream>
#include<math.h>
#include"gelenk/gelenk.h"
#include"gelenk/endeffektor.h"
#include"defines/defines.h"
using namespace std;
//using namespace arma;
int main()
{
	gelenk s0;
	endeffektor greifer(6);
	double deg =90;
	cout<< _DEG2RAD (deg)<<endl;
	cout<< cos(_DEG2RAD(deg))<<endl;
}
