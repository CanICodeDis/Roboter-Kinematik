#ifndef __MESSAGEQUEUE_H__
#define __MESSAGEQUEUE_H__

#include <unistd.h>
#include <cstdio>
#include <SDL2/SDL.h>
#include <queue>
#include <stdexcept>
#include "../gelenk/gelenk.h"
#include "../roboter6/roboter6.h"
#include "../Transformation/Transformation.h"
#include "../terminal/Termios.h"

extern roboter6* roboter;

class AbstractThreadMessage {
public:
	inline AbstractThreadMessage() {}
	inline ~AbstractThreadMessage() {}
	virtual void handle() {}
};

enum ParamTypeTM { Theta, TransZ, TransX, Alpha };
enum ParamSubTypeTM { Min, Max, Value, Speed };

class SetParamTM : public AbstractThreadMessage {
	int g;
	ParamTypeTM p;
	ParamSubTypeTM s;
	double v;
public:
	inline SetParamTM(int gelenkno, ParamTypeTM parameter, ParamSubTypeTM valuetype, double value)
			:g(gelenkno), p(parameter), s(valuetype), v(value) {
		if (p!=Theta && p!=TransZ && s!=Value)
			throw std::runtime_error( "Parameter does not support this value type" );
	}
	void handle();
};

class SetEEinDH : public AbstractThreadMessage {
	double rz, tz, tx, rx;
public:
 	inline SetEEinDH(double aRz, double aTz, double aTx, double aRx)
			:rz(aRz),tz(aTz),tx(aTx),rx(aRx){
	}
	void handle();
};

class SetEEinTR : public AbstractThreadMessage {
	double x,y,z,u,v,w;
public: 
	inline SetEEinTR(double ax, double ay, double az, double ayaw, double apitch, double aroll)
			:x(ax),y(ay),z(az),u(ayaw),v(apitch),w(aroll) {
	}
	void handle();
};

class JumpRobotJoints : public AbstractThreadMessage {
	double cfg[6][2];
	int n;
public:
	inline JumpRobotJoints( double* v, int o, double* p ):n(o) {
		for (int i=0; i<6; i++) {
			cfg[i][0] = p[i];
			cfg[i][1] = *(v+i*8+n);
		}
	}
	inline JumpRobotJoints( double* val ) {
		n=0;
		for (int i=0; i<6; i++) {
			cfg[i][0] = cfg[i][1] = val[i];
		}
	}
	void handle();
};

class RetrieveInverseOptions : public AbstractThreadMessage {
	double values [6][8];
	bool valid[8];
public:
	inline RetrieveInverseOptions(const arma::Col<double>& atarget) {
		roboter->giveInverseOptions( &values[0][0] );
		for (int o=0; o<8; o++) {
			arma::Mat<double> trt(4,4,arma::fill::eye);
			trmat Target = trmat(trt,0,4);
			bool v = true; double det;
			for (int i=0; i<6; i++) {
				gelenk gel = roboter->getGelenk(i+1);
				trmat	test = gel.getTransformation();
				test.transform(values[i][o], gel.giveAlpha(), gel.giveH(), gel.giveR());
				det = test.validateRotation();
				if (det <= 0.999 || det >= 1.001 ) {
					v = false;
				}
				if (i<=3) {
					Target = Target * test;
					if (i==3) {
						arma::Col<double> dist = Target.translation()-atarget;
						if (dist(0)*dist(0)+dist(1)*dist(1)+dist(2)*dist(2) > 0.00001) {
							v=false; //abweichung von handwurzel zu ziel zu groß
						}
					}
				}
			}
			valid[o] = v;
		}
	}
	void handle();
};

class ThreadMessageQueue {
	SDL_mutex* mutex;
	std::queue<AbstractThreadMessage*> messages;
public:
	inline ThreadMessageQueue() {
		mutex = SDL_CreateMutex();
	}
	inline ~ThreadMessageQueue() {
		SDL_DestroyMutex(mutex);
		while (!messages.empty()) {
			AbstractThreadMessage* msg = messages.front();
			messages.pop();
			free(msg);
		}
	}
	inline void pop() {
		if (SDL_LockMutex(mutex) == 0) {
			while (!messages.empty()) {
				AbstractThreadMessage* msg = messages.front();
				messages.pop();
				msg->handle();
				free(msg);
			}
			SDL_UnlockMutex(mutex);
		}
	}
	inline void push(AbstractThreadMessage* msg) {
		if (SDL_LockMutex(mutex) == 0) {
			messages.push(msg);
			SDL_UnlockMutex(mutex);
		}
	}
};

#endif
