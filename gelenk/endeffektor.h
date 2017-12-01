#ifndef _ENDEFFEKTOR_H
#define _ENDEFFEKTOR_H
#include"gelenk.h"
#include"../defines/defines.h"
class endeffektor : public gelenk
{
	public:
	endeffektor(const int n);
	~endeffektor()=default;
	endeffektor(endeffektor&) =delete;	

	void Basiskinematik1 (gelenk& turm);
};

#endif
