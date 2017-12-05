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
