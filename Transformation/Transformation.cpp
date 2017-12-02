#include"Transformation.h"

trmat::trmat (gelenk& aNach_gelenk):Transform(4,4)
{
ausgangssystem = aNach_gelenk.nummer();
eingangssystem = ausgangssystem-1;
transform (aNach_gelenk);
}

trmat::trmat():Transform(4,4)
{}
trmat trmat::operator * (trmat& aVor)
{
	if (this->eingangssystem != aVor.ausgangssystem)
		{
			std::cout<< "Ungültige Transformation!"<<std::endl;
			//throw
		} 
trmat produkt;
produkt.ausgangssystem=this->ausgangssystem;
produkt.eingangssystem= aVor.eingangssystem;
produkt.Transform= this->Transform * aVor.Transform;
return produkt;
}

Mat<double> trmat::transform (gelenk& aNach_gelenk)
{
	Transform[0]= cos(_DEG2RAD(aNach_gelenk.giveTheta()));
	Transform[1]= sin(_DEG2RAD(aNach_gelenk.giveTheta()));
	Transform[2]= 0;
	Transform[3]= 0;

	Transform[4]= - ( sin (_DEG2RAD(aNach_gelenk.giveTheta())) * cos (_DEG2RAD(aNach_gelenk.giveTheta())));
	Transform[5]= cos(_DEG2RAD(aNach_gelenk.giveTheta())) * cos(_DEG2RAD(aNach_gelenk.giveAlpha()));
        Transform[6]= sin (_DEG2RAD(aNach_gelenk.giveAlpha()));
	Transform[7]= 0;

	Transform[8] = sin(_DEG2RAD(aNach_gelenk.giveTheta())) * sin(_DEG2RAD(aNach_gelenk.giveAlpha()));
	Transform[9] = -(cos(_DEG2RAD(aNach_gelenk.giveTheta()))*sin(_DEG2RAD(aNach_gelenk.giveAlpha())));
	Transform[10]= cos(_DEG2RAD(aNach_gelenk.giveAlpha()));
	Transform[11]= 0;

	Transform[12]= aNach_gelenk.giveR() * cos(_DEG2RAD(aNach_gelenk.giveTheta()));
	Transform[13]= aNach_gelenk.giveR() * sin(_DEG2RAD(aNach_gelenk.giveTheta()));
	Transform[14]= aNach_gelenk.giveD();
	Transform[15]= 1;
	
	return Transform;
}
