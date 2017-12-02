#ifndef _TRANSFORMATION_H
#define _TRANSFORMATION_H
#include<armadillo>
#include<math.h>
#include"../defines/defines.h"
#include"../gelenk/gelenk.h"

using namespace arma;
class trmat
{
	private:
		Mat<double> Transform;
		int ausgangssystem, eingangssystem;
	public:
		trmat (gelenk& gelenk);
		trmat();
		trmat(trmat& aTrmatNach, trmat& aTrmatVor);
		~trmat()=default;

		Mat<double> rotation();
		bool checkRotation();
		Col<double> translation();

		trmat operator * (trmat& aVor);
		Mat<double> transform (gelenk& );



		

};



#endif
