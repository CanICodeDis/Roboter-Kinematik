#include"roboter6.h"

roboter6::roboter6(const simtype aTarget)
	{
	target = aTarget;
	for (int i=0; i<7;i++)//element0 ist blind, aber drinnen damit wir
				// z.B. gelenke[6] angeben können, wenn wir das sechste gelenk meinen
		{
		gelenk temp(i);
		gelenke.push_back(temp);
		}
	}

void roboter6::abn(std::ifstream& s, std::string& a, std::string& b, int& n) {
	s >> a >> b;
	if (s.peek()==' ') s >> n; else n = -1;
}

roboter6::roboter6(const char* MODEL_FILE) {
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

		if (line != FILE_HEADER) {
			throw std::runtime_error ( string("Invalid model header for ")+MODEL_FILE );
		}

		while (fin.good()) {
			abn(fin, a, b, n);
			if (a == "BEGIN") {
				if (b == "ROBOT") {
					if (n<1 || n>2) 
						throw std::range_error( "ROBOTER requires a simtype of {1=Value;2=Target}" );
					target = simtype(n-1);
	
					abn(fin, a, b, n);
					while (a != "END" && b != "ROBOT") {
						
						if (a == "BEGIN" && b == "JOINT") {
							int geli = n; //gelenk index
							if (geli <1 || geli>6)
								throw std::range_error( "JOINT requires a index between [1; 6]" );
							double min,max,val,spd; //parsed param args
							std::string target; //target
						  char valid=0;
							//all variable for the joint
							bDouble gat, gah;
							double gar, gaa;

							//fill four arguments
							for (int i = 0; i < 4; i++) {

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
								} else throw std::runtime_error( "Invalid value target in JOINT block" );
							
							}
							abn(fin, a, b, n);
							if (a != "END" || b != "JOINT")
								throw std::runtime_error( "JOINT does not end after values" );
							if (valid != 0b00001111)
								throw std::runtime_error( "JOINT did not get all params: Alpha(1), Theta(4), TransZ(4), TransX(1)" );
							gelenke.at(geli) = gelenk(geli, gat, gah, gar, gaa);
						} else throw new std::runtime_error( "Invalid block within ROBOT, begin JOINT or end ROBOT" );
	
						abn(fin, a, b, n);
					}
				} else throw std::runtime_error( "Unknown begin block" );
			} else throw std::runtime_error( "At root level a block begin is expected" );
		}
	} rethrow
}

gelenk& roboter6::getGelenk(int n) {
	if (n<1 || n>6) throw std::range_error( "Kein Gelenk mit dieser Nummer verfügbar!" );
	return gelenke.at(n);
}

void roboter6::BasKin1()
	{
	if (! isBuilt)
	/*	throw logic_exception;*/ //throw stuff
	double vecEE2H;
	vecEE2H = gelenke[6].laenge() + gelenke[5].laenge();
	}

void roboter6::BasKin2()
	{
	
	}

void roboter6::BasKin3()
	{
	
	}

void roboter6::BasKin4()
	{
	
	}
