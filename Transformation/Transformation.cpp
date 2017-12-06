#include"Transformation.h"

trmat::trmat(const Mat<double> tran, const int ausgang, const int eingang):Transform(4,4)
{
Transform=tran;
ausgangssystem=ausgang;
eingangssystem=eingang;
}

trmat::trmat(int nummer):Transform(4,4)
{
ausgangssystem= nummer-1;
eingangssystem = nummer;
}


trmat::trmat():Transform(4,4)
{}

trmat trmat::operator * (trmat& aVor)
{
	if (this->eingangssystem != aVor.ausgangssystem)
		{
			std::cout<< "Ungültige Transformation!"<<std::endl;
			//throw
		} 
trmat produkt;
produkt.ausgangssystem=this->ausgangssystem;
produkt.eingangssystem= aVor.eingangssystem;
produkt.Transform= this->Transform * aVor.Transform;
return produkt;
}

Mat<double> trmat::transform (double aTheta, double aAlpha, double aH, double aR)
{
	double ca, ct, sa, st;
	ca = cos (_DEG2RAD(aAlpha));
	ct = cos (_DEG2RAD(aTheta));
	sa = sin (_DEG2RAD(aAlpha));
	st = sin (_DEG2RAD(aTheta));

	Transform[0]= ct;
	Transform[1]= st;
	Transform[2]= 0;
	Transform[3]= 0;

	Transform[4]= -(st*ca) ;
	Transform[5]= ct*ca;
        Transform[6]= sa;
	Transform[7]= 0;

	Transform[8] = st*sa;
	Transform[9] = -ct*sa;
	Transform[10]= ca;
	Transform[11]= 0;

	Transform[12]= aR*ct;
	Transform[13]= aR*st;
	Transform[14]= aH;
	Transform[15]= 1;
	
	return Transform;
}

Mat<double> trmat::giveTransform()
	{
	return Transform;
	}

Col<double> trmat::operator * (Col<double>& aPoint)
	{
	Col<double> result (4);
	aPoint.resize(4);
	aPoint[3]= 1;
	result = Transform * aPoint;
	return result;
	}

void trmat::operator = (trmat aTrans)
	{
	//trmat temp;
	this->ausgangssystem = aTrans.ausgangssystem;
	this->eingangssystem = aTrans.eingangssystem;
	this->Transform = aTrans.Transform;
	//return temp;
	}

trmat trmat::operator -()
	{
	trmat temp;
	temp.eingangssystem = this-> ausgangssystem;
	temp.ausgangssystem = this-> eingangssystem;
	temp.Transform = inv(this->Transform);
	return temp;
	}

Mat<double> trmat::rotation()
	{
	return Transform.submat(0,0,2,2);
	}

Col<double> trmat::translation()
	{
	return Transform.col(3);
	}

double trmat::validateRotation()
	{
	return det(Transform.submat(0,0,2,2) );
	}
