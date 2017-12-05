#include "gelenk.h"

gelenk::gelenk():Transform(0)
{
	n = 0;
}

gelenk::gelenk(const int an):Transform(an)
{
	n = an;
}

void gelenk::makeTargetTransformMatrix()
	{
	Mat<double> T12 (4,4);
	T12=Transform.transform(theta.getTarget(), alpha, h.getTarget(), r);
	std::cout<<T12<<endl;
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
