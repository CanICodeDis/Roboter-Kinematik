#ifndef _GELENK_H
#define _GELENK_H
#include<iostream>
#include<armadillo>
#include<math.h>
#include"../defines/defines.h"
#include"../bdouble/BoundDouble.h"
#include"../Transformation/Transformation.h"
using namespace std;
using namespace arma;
class gelenk
{
	private:
		int n;
		// @ n: Nummer des Gelenks im Arm

		trmat Transform;
		// @ Transform: Transformationsmatrix T(n-1)n
		double l;
		// @ l:Abstand zu vorigem Gelenk = Länge des Armsegments	
		
	public:
		double alpha;
		// @ alpha: Winkel zwischen der nten Z-Achse und der (n-1)ten
		// Z-Achse um die nte X-Achse
	 	
		double	r;
		// @ r: Abstand des Gelenks entlang der nten X-Achse
		
		bDouble theta;
		// @ theta: Rotation der (n-1)ten Z-Achse ausgehend von
		// der (n-1)ten X-Achse
	 	
		bDouble h;
		// @ h: Abstand des Gelenks entlang der (n-1)ten Z-Achse
		
		gelenk ();
		// Gelenk 0
		
		gelenk (const int an);
		// Gelenk 1 bis n
		
		~gelenk()=default;
		//	gelenk (gelenk&) = delete;
		
		//----Gelenknummer----------------
		int nummer () {return n;}
		
		void makeTargetTransformMatrix();
		double validateRotation();
		Col<double> translation();
		Mat<double> rotation();
		trmat getTransformation();
		void calcLaenge();
		double laenge();
};

#endif
