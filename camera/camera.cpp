#include "camera.h"

// with aVector = (x,y,z,1)
sPoint* Camera::transform( const arma::Col<double>& aVector ) {
	arma::Col<double> wposc = matR * (aVector - colT); //move world so camer is origin, then rot,trans with Rcam, campos
	double sxp, syp;
	//if (wuah[1]<0.000000000000001) { //spx=spy=0.5;
	//	sxp = (wuah[0]/cx +0.5);
	//	syp = (-wuah[2]/cx +0.5);
	//} else {
		sxp = (wposc[0]/(cx*wposc[1]) +0.5);
 		syp = 1-(wposc[2]/(cx*wposc[1]) +0.5);
	//}
	/*
	std::cout << aVector[0]<<","<<aVector[1]<<","<<aVector[2]<<
					"->"<<wposc[0]<<","<<wposc[1]<<","<<wposc[2]<< 
					" CF"<<cx<<
					" %P:"<<sxp<<";"<<syp<<std::endl;
	*/
	return new sPoint(
			int(sxp*SCREEN_WIDTH),
			int(syp*SCREEN_HEIGHT),
			wposc[1]
	);
}

Camera& Camera::updateT() {
	double x,y,z,alpha,beta;
	//*///
	double raY = _DEG2RAD(angleY);
	double rld = _DEG2RAD(angleX); //angle we look down
	double rlu = _DEG2RAD(90.0+angleX); //trinagle inner angle (90-angle we look down)
	double l = cos( rld ) * dist;
  x = cos( raY ) * l;
	y = sin( raY ) * l;
	z = cos( rlu ) * dist;
	alpha = _DEG2RAD( angleY + 90.0 );
	beta = rld; //looking 5 deg above world origin
	/*///
	x=0.0;
	y=2.0;
	z=2.0;
	alpha= _DEG2RAD(180.0);
	beta= _DEG2RAD(-0.0);
	//*///

	double ca = cos(alpha), sa = sin(alpha), cb = cos(beta), sb = sin(beta);

	matR(0,0) = ca;
	matR(0,1) = sa;
	matR(0,2) = 0.0;
	matR(1,0) = -sa*cb;
	matR(1,1) = ca*cb;
	matR(1,2) = sb;
	matR(2,0) = sa*sb;
	matR(2,1) = -ca*sb;
	matR(2,2) = cb;

	colT(0) = x;
	colT(1) = y;
	colT(2) = z;

	//std::cout<<matR<<colT<<std::endl;

	return *this;
}
Camera& Camera::setDist( double aDist ) {
	dist = aDist;
	if (dist < 0.0) dist = -dist;
	if (dist < 1.0) dist = 1.0;
	return *this;
}
Camera& Camera::setAnglePitch( double aAngle ) {
	angleX = aAngle;
	while( angleX >= 180.0 ) angleX-=360.0;
	while( angleX < -180.0) angleX+=360.0;
	return *this;
}
Camera& Camera::setAngleYaw( double aAngle ) {
	angleY = aAngle;
	while( angleY >= 180.0 ) angleY-=360.0;
	while( angleY < -180.0) angleY+=360.0;
	return *this;
}
