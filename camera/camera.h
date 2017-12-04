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
	std::map<arma::Col<double>*, sPoint*> screenCache;
	double FOV, dist, angleY, angleX;
	double cx; //korrecturfaktor

	//why are those separated:
	//Everything revolves around the world! not the camera.
	//So in order to rotate the point around the camera we need to put the camera
	//into world origin first. (Pw-Tc)
	//Now that the camera is origin we need to ONLY rotate and everything should
	//be honkydory
	arma::Mat<double> matR; //Rotational matrix
	arma::Col<double> colT; //Trnaslation vector
	sPoint* transform( const arma::Col<double>& aVector );
public:
	Camera ():dist(10.0), angleY(135.0), angleX(-10.0) {
		matR = arma::Mat<double>(3,3);
		colT = arma::Col<double>(3);
		try {
			setFOV(90);
		} rethrow
		updateT();
		screenCache = {};
	}	
	Camera (double aFOV, double aDist, double aAngleY, double aAngleX):dist(aDist),angleY(aAngleY),angleX(aAngleX) {
		matR = arma::Mat<double>(3,3);
		colT = arma::Col<double>(3);
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
	inline arma::Mat<double>& getR() {
		return matR;
	}
	//positive camera coords, use point-getT()
	inline arma::Col<double>& getT() {
		return colT;
	}

	inline void setFOV(double aFOV) {
		if (aFOV < 30 || aFOV > 180) throw std::range_error( "FOV not supported! (Use [30; 180])" );
		FOV = aFOV;
		cx = 2.0 / tan( _DEG2RAD(FOV/2) );
	}
	
	inline void clear() {
		for (std::map<arma::Col<double>*, sPoint*>::iterator it=screenCache.begin(); it!=screenCache.end(); ++it)
			free(it->second);
		screenCache.clear();
	}
	
	inline sPoint& getScreenPoint(arma::Col<double>& aWorldPoint) {
		std::map<arma::Col<double>*, sPoint*>::iterator it = screenCache.find( &aWorldPoint );
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
