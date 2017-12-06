#include "gelenk.h"

//------------Konstruktoren------------------------

gelenk::gelenk(const int an, bDouble aTheta, bDouble aH, double aR, double aAlpha)
	{
	n = an;
	theta = aTheta;
	h = aH;
	r = aR;
	alpha = aAlpha;
	makeTargetTransformMatrix();
	calcLaenge();
	}
gelenk::gelenk():Transform(0)
{
	n = 0;
}

gelenk::gelenk(const int an):Transform(an)
{
	n = an;
}

gelenk::gelenk (const gelenk& other):theta(other.theta),h(other.h)
	{
	r= other.r;
	alpha= other.r;
	Transform= other.Transform;
	n= other.n;
	}

//--------------THETA------------------------------
//-------------------------------------------------
void gelenk::minThetaIs (const double aMinTheta)
	{
	theta.setMin(aMinTheta);
	}
void gelenk::thetaIs(const double atheta)
{
	theta=atheta;
}
void gelenk::maxThetaIs(const double aMaxTheta)
	{
	theta.setMax(aMaxTheta);
	}

//------------ALPHA--------------------------------
//-------------------------------------------------

void gelenk::alphaIs(const double aalpha)
{
	alpha=aalpha;
}


//---------D---------------------------------------
//-------------------------------------------------
void gelenk::minHIs (const double aMinH)
	{
	h.setMin(aMinH);
}	

void gelenk::hIs (const double aH)
{
	h=aH;
}

void gelenk::maxHIs (const double aMaxH)
	{
	h.setMax(aMaxH);
	}

//---------R---------------------------------------
//-------------------------------------------------


void gelenk::rIs (const double aR)
{
	r=aR;
}



void gelenk::makeTargetTransformMatrix()
	{
	Transform.transform(theta.getTarget(),alpha, h.getTarget(),r);
	}
void gelenk::makeValueTransformMatrix()
	{
	Transform.transform(theta,alpha,h,r);
	}
double gelenk::validateRotation()
	{
	return Transform.validateRotation();
	}

Col<double> gelenk::translation()
	{
	return Transform.translation();
	}

Mat<double> gelenk::rotation()
	{
	return Transform.rotation();
	}

trmat gelenk::getTransformation()
	{
	return Transform;
	}
void gelenk::calcLaenge()
	{
	Col<double> v1;
	v1= this->translation();
	l=sqrt(v1[0] * v1[0] + v1[1] * v1[1] + v1[2] * v1[2]);
	}

double gelenk::laenge()
	{
	return l;
	}

gelenk& gelenk::operator = (gelenk& other)
	{
	this->n = other.n;
	this->Transform = other.Transform;
	this->theta = other.theta;
	this->h = other.h;
	this->r = other.r;
	this->alpha = other.alpha;
	this->l = other.l;
	return *this;
	}
