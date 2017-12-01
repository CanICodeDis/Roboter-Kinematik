#include "gelenk.h"

gelenk::gelenk(): origin(3), eMatrix(3,3), Transform(4, 4)
{
	n = 0;
	isTransformValid = false;
	origin = { 0, 0, 0 };
	eMatrix.eye();
	isBaseValid= true;
	angleOverflow= false;
	angleUnderflow= false;
	isOriginValid = true;
	minTheta= -361;
	maxTheta= 361;
	minAlpha= -361;
	maxAlpha= 361;
}

gelenk::gelenk(const int an): origin(3), eMatrix(3,3), Transform(4,4)
{
	n = an;
	isTransformValid=false;
	isOriginValid =false;
	isBaseValid=false;
	angleOverflow= false;
	angleUnderflow=false;
	minTheta= -361;
        maxTheta= 361;
        minAlpha= -361;
        maxAlpha= 361;

}

void gelenk::thetaIs(const double atheta)
{
	if(atheta <= 0)
		{
		if(atheta < minTheta)
			{
			angleOverflow=true;
			}
		}
	else
	{
	if(atheta > maxTheta)
		{
		angleOverflow=true;
		}
	}
	theta= atheta;
	isTransformValid=false;
}

void gelenk::alphaIs(const double aalpha)
{
	alpha = aalpha;
	isTransformValid = false;
}

void gelenk::dIs (const double ad)
{
	d=ad;
	isTransformValid=false;
}

void gelenk::rIs (const double ar)
{
	r=ar;
	isTransformValid=false;
}
Col<double> gelenk::posInWorld()
	{
	return origin;
	}

Mat<double> gelenk::baseInWorld()
	{
	return eMatrix;
	}

Mat<double> gelenk::transform(gelenk& vorher)
	{
	if(! isTransformValid)
		{
		// Matrix wird Spalten-orientiert befüllt,
		// also zuerst A11 bis A41, dann A12 bis A42
		// usw.
		Transform[0] = cos (_DEG2RAD(theta));
		Transform[1] = sin (_DEG2RAD(theta));
		Transform[2] = 0;
		Transform[3] = 0;

		Transform[4] = - ( sin (_DEG2RAD(theta))* cos(_DEG2RAD(alpha)));
		Transform[5] = cos(_DEG2RAD(theta)) * cos (_DEG2RAD(alpha));
		Transform[6] = sin (_DEG2RAD(alpha));
		Transform[7] = 0;

		Transform[8] = sin(_DEG2RAD(theta)) * sin(_DEG2RAD(alpha));
		Transform[9] = -(cos(_DEG2RAD(theta))*sin(_DEG2RAD(alpha)));
		Transform[10]= cos(_DEG2RAD(alpha));
		Transform[11]= 0;

		Transform[12]= r * cos(_DEG2RAD(theta));
		Transform[13]= r * sin(_DEG2RAD(theta));
		Transform[14]= d;
		Transform[15]= 1;

		isTransformValid=true;
		}
	return Transform;
	}

