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
	inline sPoint(int ax, int ay, double az): x(ax),y(ay),d(az) {}
	/*expects a screen position in pixel x,z. Depth is untouched axis. */
	inline sPoint(const arma::Col<double>& c) {
		x=(int)c(0);
		y=(int)c(2);
		d=c(1);
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
	double FOV, dist, angleY;
	double cx, cy; //korrecturfaktoren in x und y
	arma::Mat<double> matT; //main diagonal initialized with 1 (so bottom row is correct by default, matrix sais we are equal to ~T_0)

	sPoint* transform( const arma::Col<double>& aVector );
public:
	Camera ():dist(10.0), angleY(135.0) {
		matT = arma::Mat<double>(4,4,arma::fill::eye);
		try {
			setFOV(90);
		} rethrow
		updateT();
		screenCache = {};
	}	
	Camera (double aFOV, double aDist, double aAngleY):dist(aDist),angleY(aAngleY) {
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
	Camera& setAngleY( double aAngle );
	inline double getAngleY() { return angleY; }
	Camera& updateT();
	inline arma::Mat<double>& getT() {
		return matT;
	}

	inline void setFOV(double aFOV) {
		if (aFOV < 30 || aFOV > 180) throw std::range_error( "FOV not supported! (Use [30; 180])" );
		FOV = aFOV;
		cx = (1.0/ (2.0 * atan(FOV/2)));
		cy = (-8.0/ (9.0 * atan(FOV/2)));
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
