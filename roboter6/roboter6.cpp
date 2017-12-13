#include"roboter6.h"

roboter6::roboter6(const simtype aTarget):posHW(4),Endeffektor(4)
	{
	target = aTarget;
	for (int i=0; i<7;i++)//element0 ist blind, aber trotzdem enthalten, damit wir
				// z.B. gelenke[6] angeben können, wenn wir das sechste gelenk meinen
		{
		gelenk temp(i);
		gelenke.push_back(temp);
		}
	}

void roboter6::abn(std::ifstream& s, std::string& a, std::string& b, int& n, int& lc)
{
	lc++;
	s >> a >> b;
	if (s.peek()==' ') s >> n; else n = -1;
}

roboter6::roboter6(const char* MODEL_FILE):posHW(4),Endeffektor(4)
{
	std::string FILE_HEADER = "ROBMDL ";
	FILE_HEADER += VERSION;

	//fill vector with temporary elements to allow out of order joints in the model
	for (int i=0; i<7; i++)
		gelenke.push_back(gelenk(i));

	try {
		std::ifstream fin (MODEL_FILE, std::ifstream::in);
		std::string line,a,b; //block tokens
		int n; //blok param
		std::getline(fin, line);
		int linecounter=1;

		if (line != FILE_HEADER) {
			throw std::runtime_error ( string("Invalid model header for ")+MODEL_FILE );
		}

		while (fin.good()) {
			abn(fin, a, b, n, linecounter);
			if (a == "BEGIN") {
				if (b == "ROBOT") {
					if (n<1 || n>2)
						throw parse_error( "ROBOTER requires a simtype of {1=Value;2=Target}", linecounter);
					target = simtype(n-1);

					abn(fin, a, b, n, linecounter);
					while (a != "END" && b != "ROBOT") {

						if (a == "BEGIN" && b == "JOINT") {
							int geli = n; //gelenk index
							if (geli <1 || geli>6)
								throw parse_error( "JOINT requires a index between [1; 6]", linecounter );
							double min,max,val,spd; //parsed param args
							std::string target; //target
						  char valid=0;
							//all variable for the joint
							bDouble gat, gah;
							double gar, gaa;

							//fill four arguments
							for (int i = 0; i < 4; i++) {
								linecounter++;
								fin >> target >> min;
								if (target == "Alpha") {
									valid |= 0b00000001;
									gaa = min;
								} else if (target == "TransX") {
									valid |= 0b00000010; 
									gar = min/1000.0;
								} else if (target == "Theta") {
									valid |= 0b00000100;
									fin >> max >> val >> spd;
									gat = bDouble(min, max, val, spd);
								} else if (target == "TransZ") {
									valid |= 0b00001000;
									fin >> max >> val >> spd;
									gah = bDouble(min/1000.0, max/1000.0, val/1000.0, spd/1000.0);
								} else throw parse_error( "Invalid value target in JOINT block", linecounter );

							}
							abn(fin, a, b, n, linecounter);
							if (a != "END" || b != "JOINT")
								throw parse_error( "JOINT does not end after values", linecounter );
							if (valid != 0b00001111)
								throw parse_error( "JOINT did not get all params: Alpha(1), Theta(4), TransZ(4), TransX(1)", linecounter );
							std::cout << "Loaded joint " << geli << " with DH " << gat << ", " << gah << ", " << gar << ", " << gaa << std::endl;
							gelenke.at(geli) = gelenk(geli, gat, gah, gar, gaa);
						} else throw new parse_error( "Invalid block within ROBOT, begin JOINT or end ROBOT", linecounter );

						abn(fin, a, b, n, linecounter);
					}
				} else if (b == "COMMENT") {
					std::string line="";
					while (line != "END COMMENT") {
						std::getline(fin, line);
						std::cout << line << std::endl;
						linecounter++;
					}
				}	else throw parse_error( "Unknown begin block", linecounter );
			} else if (!fin.eof()) throw parse_error( "At root level a block begin is expected", linecounter );
		}
	} rethrow
}

gelenk& roboter6::getGelenk(int n) {
	if (n<1 || n>6) throw std::range_error( "Kein Gelenk mit dieser Nummer verfügbar!" );
	return gelenke.at(n);
}

void roboter6::BasKin1()
	{
	double lEE2HW;
	Mat<double> OrientierungEE (3,3);
	Col<double> posEE (3);
	lEE2HW = gelenke[6].laenge() + gelenke[5].laenge();
	OrientierungEE =Endeffektor.rotation();
	posEE = Endeffektor.translation().head(3);
	posHW = posEE - (lEE2HW * OrientierungEE.col(2));

	theta1BasKin1[0] = atan2 (posHW[1],posHW[0]);
	theta1BasKin1[1] = theta1BasKin1[0] + M_PI;

	theta1BasKin1[0] = _RAD2DEG(theta1BasKin1[0]);
	theta1BasKin1[1] = _RAD2DEG(theta1BasKin1[1]);
	for (int x= 0 ; x<4 ; x++)
		{
		theta[0][x] = theta1BasKin1[0];
		theta[0][3+x] = theta1BasKin1[1];
		}
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
	l3 = gelenke[3].laenge()+gelenke[4].laenge();
	std::cout << l2 << ", " << l3 << "; ";
	// S0 von BasKin2 entspricht S1 in Weltkoordinaten
	// deshalb müssen wir S1 (Welt) nach S0 (BasKin2) umrechnen
	// dabei gibt es zwei Fälle:
	for (int i=0;i<2;i++)
		{
		gelenke[1].thetaIs(theta1BasKin1[i]);
		gelenke[1].makeTargetTransformMatrix();
		if (i==0)
			{
			systemTransform[0][0] = gelenke[1].getTransformation();
			systemTransform[0][1] = systemTransform[0][0];
			systemTransform[0][2] = systemTransform[0][0];
			systemTransform[0][3] = systemTransform[0][0];
			T10 = - systemTransform[0][0];
			}
		else
			{
			systemTransform[0][4] = gelenke[1].getTransformation();
			systemTransform[0][5] = systemTransform[0][4];
			systemTransform[0][6] = systemTransform[0][4];
			systemTransform[0][7] = systemTransform[0][4];
			T10 = - systemTransform[0][4];
			}
		//Handwurzelposition von S0 in S1 umrechnen
		HWinS1 = T10 * posHW;
		x[i]= HWinS1[0];
		y[i]= HWinS1[1];

		//Prüfung auf Erreichbarkeit des Punktes
		ref = sqrt (x[i] * x[i] + y[i] *y[i]);
		std::cout << ref << std::endl;
		if ( ref <= abs(l2-l3) || ref >= (l2+l3) )
			{
			throw runtime_error("BasKin2 nicht lösbar, da Handwurzel außerhalb des Arbeitsraums");
			}
		if( (abs((x[i] - 0.0))<= 1e-14) && (abs((y[i]-0.0))<=1e-14 ))
			{
			throw runtime_error(" Handwurzel in S1, Tangens nicht rechenbar");
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
		for (int x=0 ; x < 8 ; x+=2)
			{
			theta[1][x] = theta2BasKin2[x/2];
			theta[1][x+1] =theta[1][x];
			
			theta[2][x] = theta3BasKin2[x/2];
			theta[2][x+1] = theta[2][x+1];
			}

		}

		for (int n =0 ; n < 4 ; n++)
			{
			gelenke[2].thetaIs(theta2BasKin2[n]);
			gelenke[2].makeTargetTransformMatrix();
			systemTransform[1][n*2] = gelenke[2].getTransformation();
			systemTransform[1][n*2+1] =systemTransform[1][n*2];

			gelenke[3].thetaIs(theta3BasKin2[n]);
			gelenke[3].makeTargetTransformMatrix();
			systemTransform[2][n*2] = gelenke[3].getTransformation();
			systemTransform[2][n*2+1] = systemTransform[2][n*2];
			}

	}

void roboter6::BasKin3()
	{
	trmat T30[4];
	trmat T36[4];

	for (int n =0 ; n<4 ; n++)
		{
		Mat<double> thetaHolder (3,2);
		T30[n] = -( systemTransform[0][n*2] * systemTransform[1][n*2] * systemTransform[2][n*2]);
		T36[n] = T30[n] * Endeffektor;
		R36[n] = T36[n].rotation();
		thetaHolder = InvEuler323 (n);
		for (int x = 0 ; x<6 ; x++)
			thetaHolder[x] = _RAD2DEG (thetaHolder[x]);
		
		theta[3][2*n] = thetaHolder(0,0);
		theta[3][2*n+1]= thetaHolder(0,1);
		gelenke[4].thetaIs(thetaHolder(0,0));
		gelenke[4].makeTargetTransformMatrix();
		systemTransform[3][2*n] = gelenke[4].getTransformation();
		gelenke[4].thetaIs(thetaHolder(0,1));
		gelenke[4].makeTargetTransformMatrix();
		systemTransform[3][2*n+1]= gelenke[4].getTransformation();
		
		theta[4][2*n] = thetaHolder(1,0);
		theta[4][2*n+1] = thetaHolder(1,1);
		gelenke[5].thetaIs(thetaHolder(1,0));
		gelenke[5].makeTargetTransformMatrix();
		systemTransform[4][2*n] = gelenke[5].getTransformation();
		gelenke[5].thetaIs(thetaHolder(1,1));
		gelenke[5].makeTargetTransformMatrix();
		systemTransform[4][2*n+1] = gelenke[5].getTransformation();

		theta[5][2*n] = thetaHolder(2,0);
		theta[5][2*n+1] = thetaHolder(2,1);
		gelenke[6].thetaIs(thetaHolder(2,0));
		gelenke[6].makeTargetTransformMatrix();
		systemTransform[5][2*n] = gelenke[6].getTransformation();
		gelenke[6].thetaIs(thetaHolder(2,1));
		gelenke[6].makeTargetTransformMatrix();
		systemTransform[5][2*n+1] =gelenke[6].getTransformation();
		}
	}

Mat<double> roboter6::InvEuler323(const int n)
	{
	Mat<double> temp (3,2);
	double ref = sqrt (R36[n](2,0) * R36[n](2,0) + R36[n](2,1) * R36[n](2,1));
	double sb[2];
		
		for (int c=0; c<2; c++)
		{
		if(c==0)
			temp(1,c) = atan2 (ref,R36[n](2,2)); //beta, entspricht theta2
		else
			temp(1,c) = atan2 (-ref,R36[n](2,2)); //beta, entspricht theta2

		sb[c] = sin (temp(1,c));
		if(abs(M_PI -sb[c])<=1e-14)
		{
		temp(0,c) = M_PI/3;
		//b = a-g --> g = a - b
		temp(2,c) = temp(0,c) - temp(1,c);
		}
		else if (abs(sb[c]-0.0)<=1e-14)
		{
		temp(0,c) = M_PI/3;
		//b= a + g --> g= b - a
		temp(2,c)= temp(1,c) - temp(1,c);
		}
		else
		 {
		temp(0,c) = atan2 (R36[n](1,2)/sb[c] , R36[n](0,2)/sb[c]); //alpha, entspricht theta1
		temp(2,c) = atan2 (R36[n](2,1)/sb[c], -R36[n](2,0)/sb[c]); // gamma, entspricht theta3
		}

		}


	return temp;
	}


void roboter6::calcWorldTransformations()
	{
	worldTransform[0] = gelenke[1].getTransformation();

	for (int i = 1 ; i<6 ; i++)
		{
		worldTransform[i] = worldTransform[i-1] * (gelenke[i+1].getTransformation());
		}
	}

void roboter6::updateMatrices()
	{
	for (int i= 1 ; i<7 ; i++)
		gelenke[i].makeValueTransformMatrix();
	calcWorldTransformations();
	}

Col<double> roboter6::giveAnkleNPositionInWorld (const int ankle)
	{
	if (ankle <1 && ankle > 6)
		throw logic_error("Just 6 ankles");

	return worldTransform[ankle-1].translation();
	}

Mat<double> roboter6::giveAnkleNRotationInWorld(const int ankle)
	{
	if (ankle < 1 && ankle > 6)
		throw logic_error("only 6 ankles");

	return worldTransform[ankle-1].rotation();
	}

void roboter6::setEndEffektor (const trmat& other)
	{
		try {
			Endeffektor = other;
			BasKin1();
			BasKin2();
			BasKin3();
		} rethrow
	}
void roboter6::giveInverseOptions(double* aTheta)
	{
	for (int y = 0 ; y < 6 ; y++) {
		for (int x = 0 ;x < 8 ; x++) {
			*(aTheta+8*y+x) = theta[y][x];
		}
	}
	}
/*
void roboter6::sortPossibilites()
	{
	double differenceOfTheta [8]
	for (int i = 0 ; i < 8 ; i++)
		for (int j= 0 ; j < 6 ; j++)
		differenceOfTheta[i] += abs(theta[j][i] - gelenke[j+1].theta);
			
	}*/
