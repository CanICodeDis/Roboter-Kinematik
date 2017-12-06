#include "ThreadMessageQueue.h"
#include "../Executable.h"

void SetParamTM::handle() {
	gelenk gel = roboter->getGelenk(g);
	switch(p) {
		case Theta:
			std::cout << "Theta ";
			switch(s) {
				case Min:
					gel.minThetaIs(v);
					break;
				case Max:
					gel.maxThetaIs(v);
					break;
				case Value:
					gel.thetaIs(v);
					break;
//				case Speed: g.speed
				default: break;
			}
		 	break;
		case TransZ: 
			switch(s) {
				case Min:
					gel.minHIs(v);
					break;
				case Max:
					gel.maxHIs(v);
					break;
				case Value:
					gel.hIs(v);
					break;
//				case Speed: g.speed
				default: break;
			}
		 	break;
		case TransX:
			gel.rIs(v);
			break;
		case Alpha:
			gel.alphaIs(v);
			break;
		default: break;
	}
}
