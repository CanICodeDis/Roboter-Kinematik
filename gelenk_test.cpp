#include<armadillo>
#include<iostream>
#include<math.h>
#include"gelenk/gelenk.h"
#include"defines/defines.h"
#include"Transformation/Transformation.h"
using namespace std;
using namespace arma;
int main()
{
	gelenk s2(2);
	Col<double> p1 (4);
	Col<double> p2 (4);
	p1 = {0 , 0 , 0 , 1};
	s2.minThetaIs(0.0);
	s2.maxThetaIs(90.0);
	s2.alphaIs(52.0);
	s2.minHIs(0.0);
	s2.maxHIs(500.0);
	s2.rIs(34.0);
	s2.hIs(65.0);
	s2.thetaIs(13.0);
	s2.makeTargetTransformMatrix();
	gelenk s3 = s2;
	cout<<"Determinante: "<<s2.validateRotation()<<endl;
	cout<<s2.translation()<<endl<<s2.rotation();	
	s2.calcLaenge();
	cout<<"Länge: "<< s2.laenge()<<endl;
	trmat T06,T60;
	T06 = s2.getTransformation();
	cout<< "T06: "<<endl<< T06.giveTransform()<<endl;
	T60 = -T06;
	cout<< "T60: "<< endl<<T60.giveTransform()<<endl;

	p2 = T06 * p1;
	cout<< "P2: "<<endl<<p2<<endl;

	p1 = T60 * p2;
	cout<<"P1: "<<endl<<p1;
}
