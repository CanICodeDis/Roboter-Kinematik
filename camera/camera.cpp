#include "camera.h"

// with aVector = (x,y,z,1)
sPoint* Camera::transform( const arma::Col<double>& aVector ) {
	arma::Col<double> wposc = aVector;
	if (wposc.n_rows == 4) wposc = wposc.head(3);
	wposc = wposc-matT.col(3).head(3); //move point, so camera is origin - the world spins around, even it feels like your head spinning sometimes
	wposc = matT.submat(0,0,2,2) * wposc; // the actual rotation
	double sxp, syp;

		sxp = (wposc[0]/(cx*wposc[1]) +0.5);
 		syp = (wposc[2]/(cx*wposc[1]) +0.5);
	/*///
	std::cout << aVector[0]<<","<<aVector[1]<<","<<aVector[2]<<
					"->"<<wposc[0]<<","<<wposc[1]<<","<<wposc[2]<< 
					" CF"<<cx<<
					" %P:"<<sxp<<";"<<syp<<std::endl;
	//*///
	return new sPoint(
			int(sxp*SCREEN_WIDTH),
			int(syp*SCREEN_WIDTH), //use width here as well since we scaled the points to 0;1 width with cx.
			                       //everything should actually be scaled to the wider, but most screens are wide
			wposc[1]
	);
}

Camera& Camera::updateT() {
	double x,y,z,alpha,beta;
	//*///
	double raY = _DEG2RAD(angleY);
	double rld = _DEG2RAD(angleX-1.0); //angle we look down
	double rlu = _DEG2RAD(90.0+angleX); //trinagle inner angle (90-angle we look down)
	double l = sin( rlu ) * dist;
  x = cos( raY ) * l;
	y = sin( raY ) * l;
	z = -cos( rlu ) * dist;
	alpha = _DEG2RAD( angleY + 90.0 );
	beta = rld;

	double ca = cos(alpha), sa = sin(alpha), cb = cos(beta), sb = sin(beta);
	
	matT(0,0) = -ca;
	matT(0,1) = -sa;
	matT(0,2) = 0.0;
	matT(1,0) = -sa*cb;
	matT(1,1) = ca*cb;
	matT(1,2) = -sb;
	matT(2,0) = sa*sb;
	matT(2,1) = -ca*sb;
	matT(2,2) = -cb;

	matT(0,3) = x;
	matT(1,3) = y;
	matT(2,3) = z;

//	std::cout << matT << std::endl << std::endl;

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
	if (angleX < -45.0) angleX = -45.0;
	if (angleX > 45.0) angleX = 45.0;
	return *this;
}
Camera& Camera::setAngleYaw( double aAngle ) {
	angleY = aAngle;
	while( angleY >= 180.0 ) angleY-=360.0;
	while( angleY < -180.0) angleY+=360.0;
	return *this;
}
