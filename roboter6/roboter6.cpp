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

void roboter6::abn(std::ifstream& s, std::string& a, std::string& b, int& n, int& lc)
{
	lc++;
	s >> a >> b;
	if (s.peek()==' ') s >> n; else n = -1;
}

roboter6::roboter6(const char* MODEL_FILE):posHW(4)
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
									gar = min;
								} else if (target == "Theta") {
									valid |= 0b00000100; 
									fin >> max >> val >> spd;
									gat = bDouble(min, max, val, spd);
								} else if (target == "TransZ") {
									valid |= 0b00001000; 
									fin >> max >> val >> spd;
									gah = bDouble(min, max, val, spd);
								} else throw parse_error( "Invalid value target in JOINT block", linecounter );
							
							}
							abn(fin, a, b, n, linecounter);
							if (a != "END" || b != "JOINT")
								throw parse_error( "JOINT does not end after values", linecounter );
							if (valid != 0b00001111)
								throw parse_error( "JOINT did not get all params: Alpha(1), Theta(4), TransZ(4), TransX(1)", linecounter );
							gelenke.at(geli) = gelenk(geli, gat, gah, gar, gaa);
						} else throw new parse_error( "Invalid block within ROBOT, begin JOINT or end ROBOT", linecounter );
	
						abn(fin, a, b, n, linecounter);
					}
				} else if (b == "COMMENT") {
					std::string line="";
					while (line != "END COMMENT") {
						std::getline(fin, line);
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

