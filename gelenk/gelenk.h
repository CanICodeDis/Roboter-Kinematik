#ifndef _GELENK_H
#define _GELENK_H
#include<iostream>
#include<armadillo>
#include<math.h>
#include"../defines/defines.h"
#include"../bdouble/BoundDouble.h"
using namespace std;
using namespace arma;
class gelenk
{
	private:
		int n;
		// @ n: Nummer des Gelenks im Arm

		Col<double> origin;
		// @ ?0: Postition des Gelenkkoordinatensystems in Weltkoordinaten
		// notwendig für Visualisierung

		Mat<double> eMatrix;
		//Orientierug der Achsen des Gelenkkoordinatensystems in Weltkoordinaten

		double l;
		// @ l: Abstand des Gelenks zum nächsten Gelenk

		bDouble theta, alpha , d, r;
		// @ theta: Rotation der (n-1)ten Z-Achse ausgehend von
		// der (n-1)ten X-Achse
		// @ alpha: Winkel zwischen der nten Z-Achse und der (n-1)ten
		// Z-Achse um die nte X-Achse
		// @ d: Abstand des Gelenks entlang der (n-1)ten Z-Achse
		// @ r: Abstand des Gelenks entlang der nten X-Achse
		bool isTransformValid;

		
	public:
		gelenk ();
		// Gelenk 0
		gelenk (const int an);
		// Gelenk 1 bis n
		~gelenk()=default;
		gelenk (gelenk&) = delete;
		//----Gelenknummer----------------
		int nummer () {return n;}
		//----Gelenkbegrenzungen----------
		void minThetaIs (const double aMinTheta);
		void maxThetaIs (const double aMaxTheta);
		void minAlphaIs (const double aMinAlpha);
		void maxAlphaIs (const double aMaxAlpha);
		void minRis (const double aMinR);
		void maxRis (const double aMaxR);
		void minDis (const double aMinD);
		void maxDis (const double aMaxD);
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
		double giveAlpha (void) {return alpha.getTarget();}
		void dIs (const double ad);
		// setzt den Abstand des Koordinatenursprungs zum
		// vorigen Koordinatenursprung
		// entlang der (n-1)ten Z-Achse fest
		double giveD (void) {return d.getTarget();}
		void rIs (const double ar);
		// setzt den Abstand des Koordinatenursprungs zum
		// vorigen Koordinatenursprung
		// entlang der n-ten X-Achse
           	double giveR(void) {return r.getTarget();} 
		//----Position und Orientierung in S0--------------
		Col<double> posInWorld(Mat<double>);
		// gibt die Position in Weltkoordinaten in einem
		// Spaltenvektor ( x ; y ; z ) aus
		
		Mat<double> baseInWorld(void);
		// gibt die Basisvektoren in Weltkoordinaten aus
		
		//----Position im Koordinatensystem des vorigen Gelenks-------
		Col<double> posInParent (const gelenk& parent);
		// gibt die Position des Ursprung Koordinatensystems im Eltern-Koordinatensystems aus
		
		Mat<double> baseInParent (const gelenk& parent);
		//gibt die Orientierung der Basisvektoren im Eltern-Koordinatensystem aus


};

#endif
