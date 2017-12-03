#include "camera.h"

//std::ostream& operator<<(std::ostream& os, const sPoint& point) {
//	os << '[' << point.X() << "; " << point.Y() << "](D " << point.Depth() << ')';
//	return os;
//}

// with aVector = (x,y,z,1)
sPoint* Camera::transform( const arma::Col<double>& aVector ) {
	arma::Col<double> wuah = matT * aVector;
	return new sPoint(
			(wuah(0)/wuah(1)*cx -0.5)*SCREEN_WIDTH, 
			(wuah(2)/wuah(1)*cy -0.5)*SCREEN_HEIGHT, 
			wuah(1)
	);
}

Camera& Camera::updateT() {
	double x,y,z,alpha,beta;
/*	double l = sin( _DEG2RAD(90.0) ) * dist;
  x = cos( _DEG2RAD(angleY) ) * l;
	y = cos( _DEG2RAD(70.0) ) * dist;
	z = sin( angleY ) * l;
	alpha = _DEG2RAD( angleY + 90.0 );
	beta = _DEG2RAD( -10.0 ); //looking 5 deg above world origin
	*/
	x=1.0;
	y=0.0;
	z=0.0;
	alpha= _DEG2RAD(-180);
	beta=0;

	double ca = cos(alpha), sa = sin(alpha), cb = cos(beta), sb = sin(beta);

	matT(0,0) = ca;
	matT(0,1) = sa;
	matT(0,2) = 0.0;
	matT(0,3) = x;
	matT(1,0) = -sa*cb;
	matT(1,1) = ca*cb;
	matT(1,2) = sb;
	matT(1,3) = y;
	matT(2,0) = sa*sb;
	matT(2,1) = -ca*sb;
	matT(2,2) = cb;
	matT(2,3) = z;

	std::cout<<matT<<std::endl;

	return *this;
}
Camera& Camera::setDist( double aDist ) {
	dist = aDist;
	if (dist < 0.0) dist = -dist;
	if (dist < 1.0) dist = 1.0;
	return *this;
}
Camera& Camera::setAngleY( double aAngle ) {
	angleY = aAngle;
	while( angleY >= 180.0 ) angleY-=360.0;
	while( angleY < -180.0) angleY+=360.0;
	return *this;
}
