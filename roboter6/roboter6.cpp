#include"roboter6.h"

roboter6::roboter6(const simtype aTarget):posHW(4)
	{
	target = aTarget;
	for (int i=0; i<7;i++)//element0 ist blind, aber trotzdem enthalten, damit wir
				// z.B. gelenke[6] angeben können, wenn wir das sechste gelenk meinen
		{
		gelenk temp(i);
		gelenke.push_back(temp);
		}
	}

void roboter6::BasKin1()
	{
	double lEE2HW;
	Mat<double> OrientierungEE (3,3);
	Col<double> posEE (4);
	lEE2HW = gelenke[6].laenge() + gelenke[5].laenge();
	OrientierungEE =Endeffektor.rotation();
	posEE = Endeffektor.translation();
	posHW = posEE - lEE2HW * OrientierungEE.col(2);

	theta1BasKin1[0] = atan2 (posHW[1],posHW[0]);
	theta1BasKin1[1] = theta1BasKin1[0] + M_PI;

	theta1BasKin1[0] = _RAD2DEG(theta1BasKin1[0]);
	theta1BasKin1[1] = _RAD2DEG(theta1BasKin1[1]);
	}

void roboter6::BasKin2()
	{

	//gegeben sind die Handwurzelkoordinaten in Weltkoordinaten,
	//benötigt wird aber x und y in Bezug zu S1
	double x[2],y[2];

	// Hilfsgrößen
	double p[2], r[2], alpha[2], beta[2],phi[2];
	trmat T10;
	Col<double>HWinS1 (4);
	double ref;
	//Längen der Gelenksttangen
	double l2 , l3;
	l2 = gelenke[2].laenge();
	l3 = gelenke[3].laenge();
	// S0 von BasKin2 entspricht S1 in Weltkoordinaten
	// deshalb müssen wir S1 (Welt) nach S0 (BasKin2) umrechnen
	// dabei gibt es zwei Fälle:
	for (int i=0;i<2;i++)
		{
		gelenke[1].thetaIs(theta1BasKin1[i]);
		gelenke[1].makeTargetTransformMatrix();
		if (i==0)
			{
			systemTransform ["01_1"] = gelenke[1].getTransformation();
			posS1.push_back(systemTransform["01_1"].translation());
			T10 = - systemTransform["01_1"];
			}
		else
			{
			systemTransform["01_2"] = gelenke[1].getTransformation();
			posS1.push_back(systemTransform["01_2"].translation());
			T10 = - systemTransform["01_2"];
			}
		HWinS1 = T10 * posHW;
		x[i]= HWinS1[0];
		y[i]= HWinS1[1];

		//Prüfung auf Erreichbarkeit des Punktes
		ref = sqrt (x[i] * x[i] + y[i] *y[i]);
		if ( !( (abs( l2 - l3 )<= ref) && ((l2 + l3)>= ref) ) )
			{
			cout<<"BasKin2 nicht lösbar, da Handwurzel außerhalb des Arbeitsraums"<<endl;
			//throw stuff
			continue;
			}
		if( (abs((x[i] - 0.0))<= 1e-14) && (abs((y[i]-0.0))<=1e-14 ))
			{
			cout<<" Handwurzel in S1, Tangens nicht rechenbar"<<endl;
			continue;
			//throw stuff
			}
		phi[i]= atan2 (y[i],x[i]);
		p[i] = 0.5 *(l2  + l3 + ref);
		r[i] = sqrt( ( (p[i] - l2 ) * (p[i]- l3 ) * (p[i] - ref ) ) /p[i] );
		alpha[i] = 2 * atan ( r[i] / ( p[i] - l2 ) );
		beta[i] = 2 * atan ( r[i] / ( p[i] - l3 )  );
		theta2BasKin2 [2*i] =_RAD2DEG( phi[i] + beta[i]);
		theta2BasKin2 [2*i+1] = _RAD2DEG(phi[i] - beta[i]);
		theta3BasKin2 [2*i] = _RAD2DEG(-(alpha[i] + beta[i]));
		theta3BasKin2 [2*i+1] = _RAD2DEG((alpha[i] + beta[i]));
		}
	//mit ein bisschen Hirnschmalz hätte man sich jetzt einen besseren Algorhitmus überlegen können

	gelenke[2].thetaIs(theta2BasKin2[0]);
	gelenke[2].makeTargetTransformMatrix();
	systemTransform["12_1"]	= gelenke[2].getTransformation();


	gelenke[2].thetaIs(theta2BasKin2[1]);
	gelenke[2].makeTargetTransformMatrix();
	systemTransform["12_2"]	= gelenke[2].getTransformation();


	gelenke[2].thetaIs(theta2BasKin2[2]);
	gelenke[2].makeTargetTransformMatrix();
	systemTransform["12_3"]	= gelenke[2].getTransformation();


	gelenke[2].thetaIs(theta2BasKin2[3]);
	gelenke[2].makeTargetTransformMatrix();
	systemTransform["12_4"]	= gelenke[2].getTransformation();

	gelenke[3].thetaIs(theta3BasKin2[0]);
	gelenke[3].makeTargetTransformMatrix();
	systemTransform["32_1"] = gelenke[3].getTransformation();
	

	gelenke[3].thetaIs(theta3BasKin2[1]);
	gelenke[3].makeTargetTransformMatrix();
	systemTransform["32_2"] = gelenke[3].getTransformation();
	
	gelenke[3].thetaIs(theta3BasKin2[2]);
	gelenke[3].makeTargetTransformMatrix();
	systemTransform["32_3"] = gelenke[3].getTransformation();

	gelenke[3].thetaIs(theta3BasKin2[3]);
	gelenke[3].makeTargetTransformMatrix();
	systemTransform["32_4"] = gelenke[3].getTransformation();
	}

void roboter6::BasKin3()
	{

	}

void roboter6::BasKin4()
	{

	}

gelenk& roboter6::getGelenk (const int aNummer)
	{
	vector<gelenk>::iterator it;
	for (it=gelenke.begin(); it != gelenke.end() ; it++)
		{
		if (it->nummer() == aNummer)
			{
			return *it;
			break;
			}
		}
	//throw ;
	}
