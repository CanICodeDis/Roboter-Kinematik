#ifndef _GELENK_H
#define _GELENK_H
#include<iostream>
#include<armadillo>
#include<math.h>
#include"../defines/defines.h"
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

		double theta, alpha , d, r;
		// @ theta: Rotation der (n-1)ten Z-Achse ausgehend von
		// der (n-1)ten X-Achse
		// @ alpha: Winkel zwischen der nten Z-Achse und der (n-1)ten
		// Z-Achse um die nte X-Achse
		// @ d: Abstand des Gelenks entlang der (n-1)ten Z-Achse
		// @ r: Abstand des Gelenks entlang der nten X-Achse

		double minTheta, maxTheta;
		// hält die minimale und maximale Größe des Gelenkwinkels
		// Theta fest

		double minAlpha, maxAlpha;
		// hält die minimale und maximale Größe des Verdrehwinkel
		// alpha fest

		Mat <double> Transform;
		// @ Transform: homogene Transformationsmatrix

		bool isTransformValid;
		// @ isTransformValid: wahr, if ( transform==gültig && keine Änderung in
		// den (vorherigen) Gelenken vorgenommen wurde)

		bool isBaseValid;
		// @ isBaseValid: wahr, wenn die Basisvektoren richtig mit den Transformationen
		// multipliziert wurden

		bool isOriginValid;
		// @ isOriginValid: wahr, wenn der Ursprung der Koordinaten richtig transormiert
		// wurde

		
		bool angleOverflow;
		// @ angleOverflow: wahr, wenn Eine Überschreitung in einem positiven Gelenkwinkel aufgetreten
		// ist
		
		bool angleUnderflow;
		// @ angleUnderflow: wahr, wenn eine Unterschreitung in einem negativen Gelenkwinkel
		// aufgetreten ist
	public:
		gelenk ();
		// Gelenk 0
		gelenk (const int an);
		// Gelenk 1 bis n
		~gelenk()=default;
		gelenk (gelenk&) = delete;

		//----Gelenkbegrenzungen----------
		void minThetaIs (const double aMinTheta){minTheta=aMinTheta;}
		void maxThetaIs (const double aMaxTheta){maxTheta=aMaxTheta;}
		void minAlphaIs (const double aMinAlpha){minAlpha=aMinAlpha;}
		void maxAlphaIs (const double aMaxAlpha){maxAlpha=aMaxAlpha;}

		//----Direkte Kinematik-----------
		void thetaIs (const double atheta);
		//legt den Gelenkwinkel theta fest
		// @ param atheta: setzt den Gelekwinkel
		//  theta fest
		double giveTheta (void) {return theta;}
		// gibt den Gelenkwinkel Theta in DEG zurück
		void alphaIs (const double aalpha);
		// legt die Orientierung zwischen der nten
		// und der (n-1)ten Drehachse entlang der nten
		// X-Achse fest
		double giveAlpha (void) {return alpha;}
		void dIs (const double ad);
		// setzt den Abstand des Koordinatenursprungs zum
		// vorigen Koordinatenursprung
		// entlang der (n-1)ten Z-Achse fest
		double giveD (void) {return d;}
		void rIs (const double ar);
		// setzt den Abstand des Koordinatenursprungs zum
		// vorrigen Koordinatenursprung
		// entlang der n-ten X-Achse
		double giveR (void) {return r;}
		Mat<double> transform (gelenk& voher);
		// gibt die Transformationsmatrix vom n-ten zum
		// (n-1)ten Gelenk aus

		//----Position und Orientierung in S0--------------
		Col<double> posInWorld(void);
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
