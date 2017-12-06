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

		// @ theta: Rotation der (n-1)ten Z-Achse ausgehend von
		// der (n-1)ten X-Achse
		// @ alpha: Winkel zwischen der nten Z-Achse und der (n-1)ten
		// Z-Achse um die nte X-Achse
		// @ h: Abstand des Gelenks entlang der (n-1)ten Z-Achse
		// @ r: Abstand des Gelenks entlang der nten X-Achse
		trmat Transform;
		// @ Transform: Transformationsmatrix T(n-1)n
		double l;
		// @ l:Abstand zu vorigem Gelenk = Länge des Armsegments

	public:
		double alpha, r;
		bDouble theta, h;
		gelenk ();
		// Gelenk 0
		gelenk (const int an);
		gelenk (const int an, bDouble theta, bDouble h, double r, double alpha);
		// Gelenk 1 bis n
		~gelenk()=default;
		gelenk (const gelenk&);
		//----Gelenknummer----------------
		int nummer () {return n;}
		//----Gelenkbegrenzungen----------
		void minThetaIs (const double aMinTheta);
		void maxThetaIs (const double aMaxTheta);
		void minHIs (const double aMinD);
		void maxHIs (const double aMaxD);
		//----Direkte Kinematik-----------
		void thetaIs (const double atheta);
		//legt den Gelenkwinkel theta fest
		// @ param atheta: setzt den Gelekwinkel
		//  theta fest
		double giveTheta (void) {return theta.getTarget();}
		// gibt den Gelenkwinkel Theta in DEG zurück
		void alphaIs (const double aalpha);
		// legt die Orientierung zwischen der nten
		// und der (n-1)ten Drehachse entlang der nten
		// X-Achse fest
		double giveAlpha (void) {return alpha;}
		void hIs (const double ad);
		// setzt den Abstand des Koordinatenursprungs zum
		// vorigen Koordinatenursprung
		// entlang der (n-1)ten Z-Achse fest
		double giveH (void) {return h.getTarget();}
		void rIs (const double ar);
		// setzt den Abstand des Koordinatenursprungs zum
		// vorigen Koordinatenursprung
		// entlang der n-ten X-Achse
           	double giveR(void) {return r;}

		void makeTargetTransformMatrix();
		void makeValueTransformMatrix();
		double validateRotation();
		Col<double> translation();
		Mat<double> rotation();

		trmat getTransformation();
		void calcLaenge();
		double laenge();

		gelenk operator = (const gelenk&);
};

#endif
