#include "gelenk.h"

gelenk::gelenk(): origin(3), eMatrix(3,3)
{
	n = 0;
	isTransformValid = false;
	origin = { 0, 0, 0 };
	eMatrix.eye();
}

gelenk::gelenk(const int an): origin(3), eMatrix(3,3)
{
	n = an;
	isTransformValid=false;

}

//--------------THETA------------------------------
//-------------------------------------------------
void gelenk::minThetaIs (const double aMinTheta)
	{
	theta.setMin(aMinTheta);
	}
void gelenk::thetaIs(const double atheta)
{
	isTransformValid=false;
	theta=atheta;
}
void gelenk::maxThetaIs(const double aMaxTheta)
	{
	theta.setMax(aMaxTheta);
	}

//------------ALPHA--------------------------------
//-------------------------------------------------
void gelenk::minAlphaIs(const double aMinAlpha)
	{ 
	alpha.setMin(aMinAlpha);
	}

void gelenk::alphaIs(const double aalpha)
{
	isTransformValid = false;
	alpha=aalpha;
}

void gelenk::maxAlphaIs(const double aMaxAlpha)
	{
	alpha.setMax(aMaxAlpha);
	}

//---------D---------------------------------------
//-------------------------------------------------
void gelenk::minDis (const double aMinD)
	{
	d.setMin(aMinD);
}	

void gelenk::dIs (const double ad)
{
	isTransformValid=false;
	d=ad;
}

void gelenk::maxDis (const double aMaxD)
	{
	d.setMax(aMaxD);
	}

//---------R---------------------------------------
//-------------------------------------------------

void gelenk::minRis (const double aMinR)
	{
	r.setMin(aMinR);
	}

void gelenk::rIs (const double ar)
{
	isTransformValid=false;
	r=ar;
}

void gelenk::maxRis(const double aMaxR)
	{
	r.setMax(aMaxR);
	}

Col<double> gelenk::posInWorld(Mat<double> aTran)
	{
	return origin;
	}

Mat<double> gelenk::baseInWorld()
	{
	return eMatrix;
	}


