#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../defines/defines.h"
#include <map>
#include <armadillo>
#include <math.h>
#include <iostream>

//REQUIRED DEFINES SCREEN_WIDTH, SCREEN_HEIGHT

class sPoint {
	double d;
	int x,y;
public:
	inline sPoint() {
		x=0;
		y=0;
		d=0.0;
	}
	/*expects a screen position in pixel x,z. Depth is untouched axis. */
	inline sPoint(int ax, int ay, double az): x(ax),y(ay),d(az) {
	}
	/*expects a screen position in pixel x,z. Depth is untouched axis. */
	inline sPoint(const arma::Col<double>& c) {
		x=(int)c[0];
		y=(int)c[2];
		d=c[1];
	}
  inline int X() {
		return x;
	}
	inline int Y() {
		return y;
	}
	inline double Depth() {
		return d;
	}
};

class Camera {
	std::map<const arma::Col<double>*, sPoint*> screenCache;
	double FOV, dist, angleY, angleX;
	double cx; //korrecturfaktor

	//why are those separated:
	//Everything revolves around the world! not the camera.
	//So in order to rotate the point around the camera we need to put the camera
	//into world origin first. (Pw-Tc)
	//Now that the camera is origin we need to ONLY rotate and everything should
	//be honkydory
	arma::Mat<double> matT;
public:
	Camera ():dist(10.0), angleY(-135.0), angleX(-10.0) {
		matT = arma::Mat<double>(4,4,arma::fill::eye);
		try {
			setFOV(90);
		} rethrow
		updateT();
		screenCache = {};
	}	
	Camera (double aFOV, double aDist, double aAngleY, double aAngleX):dist(aDist),angleY(aAngleY),angleX(aAngleX) {
		matT = arma::Mat<double>(4,4,arma::fill::eye);
		try {
			setFOV(aFOV);
		} rethrow
		updateT();
		screenCache = {};
	}
	~Camera () {
		clear();
	}

	Camera& setDist( double aDist );
	inline double getDist() { return dist; }
	Camera& setAngleYaw( double aAngle );
	inline double getAngleYaw() { return angleY; }
	Camera& setAnglePitch( double aAngle );
	inline double getAnglePitch() { return angleX; }
	Camera& updateT();
	inline arma::Mat<double> getR() {
		return matT.submat(0,0,2,2);
	}
	//positive camera coords, use point-getT()
	inline arma::Col<double> getT() {
		return matT.col(3).head(3);
	}

	inline void setFOV(double aFOV) {
		if (aFOV < 30 || aFOV > 180) throw std::range_error( "FOV not supported! (Use [30; 180])" );
		FOV = aFOV;
		cx = 0.5 / tan( _DEG2RAD(FOV/2) );
	}
	
	inline void clear() {
		for (std::map<const arma::Col<double>*, sPoint*>::iterator it=screenCache.begin(); it!=screenCache.end(); ++it)
			free(it->second);
		screenCache.clear();
	}

	/** transform creates a one-time transformation. If you requre the same point multiple times (from the same col*) use getScreenPoint for a cache.
	 * Please don't create your own point cache.
	 * Also don't forget to clear the camera cache after drawing all points or changing them.
	 */
	sPoint* transform( const arma::Col<double>& aVector );
	
	inline sPoint& getScreenPoint(const arma::Col<double>& aWorldPoint) {
		std::map<const arma::Col<double>*, sPoint*>::iterator it = screenCache.find( &aWorldPoint );
		if (it == screenCache.end()) {
			sPoint* point = transform ( aWorldPoint );
			screenCache[ &aWorldPoint ] = point;
			return *point;
		} else {
			return *(it->second);
		}
	}
};

//__CAMERA_H__
#endif 
