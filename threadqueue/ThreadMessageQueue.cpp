#include "ThreadMessageQueue.h"
#include "../Executable.h"

void SetParamTM::handle() {
	gelenk gel = roboter->getGelenk(g);
	switch(p) {
		case Theta:
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

void SetEEinDH::handle() {
	arma::Mat<double> matT(4,4);
	trmat t06(matT, 0, 6);
	t06.transform(rz,tz,tx,rx);
	roboter->setEndEffektor(t06);
	toCli.push(new RetrieveInverseOptions());
}

void SetEEinTR::handle() {
	arma::Mat<double> matT(4,4,arma::fill::eye);
	
	double cy = cos(u), sy = sin(u), cp = cos(v), sp = sin(v), cr = cos(w), sr = sin(w);

	matT(0,0) = cp*cy;
	matT(0,1) = cp*sy;
	matT(0,2) = -sp;
	matT(1,0) = sr*sp*cy - cr*sy;
	matT(1,1) = sr*sp*sy + cr*cy;
	matT(1,2) = sr*cp;
	matT(2,0) = cy*sp*cr + sr*sy;
	matT(2,1) = cr*sp*sy - sr*cy;
	matT(3,2) = cr*cp;

	matT(3,0) = x;
	matT(3,1) = y;
	matT(3,2) = z;
	
	trmat t06(matT, 0, 6);
	roboter->setEndEffektor(t06);
	toCli.push(new RetrieveInverseOptions());
}

void JumpRobotJoints::handle() {
	try {
		for (int i=0; i<6; i++) {
			roboter->getGelenk(i+1).theta.jump(cfg[i][1]);
		}
	} catch (...) {
		std::cout << "Configuration " << n << " exceeds joint limitations!" << std::endl;
		for (int i=0; i<6; i++) {
			roboter->getGelenk(i+1).theta.jump(cfg[i][0]);
		}
	}
}

void RetrieveInverseOptions::handle() {
	std::cout << "Please select an option (numbers will preview, return will confirm)" << std::endl;

	char buffer[512];
	for (int i=0; i<8; i++) {
		sprintf(buffer, "%s%i) % 6.2f, % 6.2f, % 6.2f, % 6.2f, % 6.2f, % 6.2f%s", 
			(valid[i]?"":"\033[1;31m"),
			i+1, values[0][i], values[1][i], values[2][i], values[3][i], values[4][i], values[5][i],
			(valid[i]?"":"\033[0m"));
		std::cout << buffer << std::endl;
	}

	double pre[6];
	for (int i=0; i<6; i++)
		pre[i] = roboter->getGelenk(i+1).theta.getTarget();
	char in; int n;
	while ((in=getch_())!='\n') {
		n=in-'0'-1;
		if (n>=0 && n<=7) {
			if (valid[n])
				toVis.push(new JumpRobotJoints(values, n, pre));
			else
				std::cout << "Configuration " << n << " has a invalid rotation!" << std::endl;
		} else if (n==-1) {
			toVis.push(new JumpRobotJoints(pre));
		}
	}
	toVis.push(new JumpRobotJoints(pre));
	if ((n>=0 && n<=7) && valid[n]) {
		for (int i=0; i<6; i++) {
			toVis.push(new SetParamTM(i+1, ParamTypeTM::Theta, ParamSubTypeTM::Value, values[i][n]));
		}
	}
}
