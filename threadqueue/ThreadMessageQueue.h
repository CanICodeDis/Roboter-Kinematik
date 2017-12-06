#ifndef __MESSAGEQUEUE_H__
#define __MESSAGEQUEUE_H__

#include <SDL2/SDL.h>
#include <queue>
#include <stdexcept>
#include "../gelenk/gelenk.h"

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
