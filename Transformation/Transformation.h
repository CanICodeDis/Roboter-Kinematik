#ifndef _TRANSFORMATION_H
#define _TRANSFORMATION_H
#include<armadillo>
#include<math.h>
#include"../defines/defines.h"
#include"../bdouble/BoundDouble.h"

using namespace arma;
class trmat
{
	private:
		Mat<double> Transform;
		int eingangssystem, ausgangssystem;
	public:
		trmat (const Mat<double>, const int ausgang, const int eingang);
		trmat(int nummer);
		trmat();
		trmat (const trmat&);
		trmat(trmat& aTrmatNach, trmat& aTrmatVor);
		~trmat()=default;

		Mat<double> rotation();
		double validateRotation();
		Col<double> translation();
		
		void operator = (trmat);
		trmat operator -();
		Col<double> operator * (Col<double>&);
		trmat operator * (trmat& aVor);

		Mat<double> transform (double aTheta, double aL, double aAlpha, double aR);
		Mat<double> giveTransform();
		//void setTransformationInWorld(Mat<double> aTransW,int nummer);
};



#endif
