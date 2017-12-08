#include "ThreadMessageQueue.h"
#include "../Executable.h"

void SetParamTM::handle() {
	gelenk gel = roboter->getGelenk(g);
	switch(p) {
		case Theta:
			std::cout << "Theta ";
			switch(s) {
				case Min:
					roboter->getGelenk(g).theta.setMin(v);
					break;
				case Max:
					roboter->getGelenk(g).theta.setMax(v);
					break;
				case Value:
					if (roboter->getSimType() == simtype::value)
						roboter->getGelenk(g).theta.setSoft(v);
					else
						roboter->getGelenk(g).theta.jump(v);
					break;
				case Speed:
					roboter->getGelenk(g).theta.setSpeed(v);
					break;
				default: break;
			}
		 	break;
		case TransZ: 
			switch(s) {
				case Min:
					roboter->getGelenk(g).h.setMin(v);
					break;
				case Max:
					roboter->getGelenk(g).h.setMax(v);
					break;
				case Value:
					if (roboter->getSimType() == simtype::value)
						roboter->getGelenk(g).h.setSoft(v);
					else
						roboter->getGelenk(g).h.jump(v);
					break;
				case Speed:
					roboter->getGelenk(g).h.setSpeed(v);
					break;
				default: break;
			}
		 	break;
		case TransX:
			roboter->getGelenk(g).r = v;
			break;
		case Alpha:
			roboter->getGelenk(g).alpha = v;
			break;
		default: break;
	}
}
