#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "Executable.h"

SDL_sem* running;

roboter6* roboter;

ThreadMessageQueue toVis;
ThreadMessageQueue toCli;

void handleConsoleInput();
fd_set readfds;
fd_set savefds;
struct timeval timeout;

int main(int argc, char** argv) {

	//arg 0 is always the executed application
	if (argc != 2) {
		std::cout << "Please specify a target model to load!" << std::endl;
		return 1;
	}
	try {
		roboter = new roboter6(argv[1]);
	} catch (std::runtime_error e) {
		std::cout << "Error while loading model: " << e.what() << std::endl;
		return 1;
	}
//	gelenk g1 = roboter->getGelenk(1);
	roboter->updateMatrices();
	for (int i = 1; i < 7; i++) {
		std::cout << roboter->getGelenk(i).getTransformation().giveTransform() << std::endl;
	}

	//block that allows to check for available data more reliably than peek()==EOF
	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);
	savefds = readfds;
	timeout.tv_sec=0;
	timeout.tv_usec=0;

	running = SDL_CreateSemaphore(0);
	if (running == NULL) {
		std::cout<< "Failed to create running semaphore: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Thread* vThread = SDL_CreateThread(thVisual, "RenderThread", (void*)NULL);
	if (vThread == NULL) {
		std::cout<< "Failed to create render thread: " << SDL_GetError() << std::endl;
		return 1;
	}
//	SDL_DetachThread(vThread);

	//check if input from terminal is available before reading/parsing command in callback, that would block
	//this is neccessary to recognice, if the running semaphore wants us to quit execution
	while (SDL_SemValue(running)==0) {
		readfds = savefds;
		if (select(1, &readfds, NULL, NULL, &timeout)) {
			try {
				handleConsoleInput();
			} catch (std::runtime_error& e) {
				std::cout << e.what() << std::endl << std::endl;
			}
		}
		toCli.pop();
		SDL_Delay(100);
	}

	SDL_WaitThread(vThread, NULL);
	SDL_Quit();
	free(roboter);
}

char c2l (char c) {
	return (c>='A'&&c<='Z')
			? c|' '
			: c;
}
void tolower(std::string& s) {
	std::transform(s.begin(), s.end(), s.begin(), c2l);
}
bool nextConsoleToken(int& token) {
	char c;
	std::cin.get(c);
	if (c == ' ') {
		std::cin >> token;
		return true;
	}
	return false;
}
bool nextConsoleToken(double& token) {
	char c;
	std::cin.get(c);
	if (c == ' ') {
		std::cin >> token;
		return true;
	}
	return false;
}
bool nextConsoleToken(std::string& token) {
	char c;
	std::cin.get(c);
	if (c == ' ') {
		std::cin >> token;
		tolower(token);
		return true;
	}
	return false;
}
void handleConsoleInput() {
	std::string token;
	std::cin >> token;
	tolower(token);

	if (token == "quit") {
		std::cout << "Good bye!\n";
		SDL_SemPost(running);
	} else if (token == "help") {

		if (nextConsoleToken(token)) {
		
			if (token == "quit") {
				std::cout << "  == Help 'quit' ==\nThis command quits the application\n -] Params:\n  NONE\n";
			} else if (token == "set") {
				std::cout << "  == Help 'set' ==\nThis command is used to live manipulate the robot\nWe would like to apologize for the lack of graphical manipulation methods\n -] Params: <JOINT> <PARAM> <SUBPA> <VALUE>\n  JOINT - The joint index to manipulate\n  PARAM - The name of the DH param to change. One of: Theta, TransZ, TransX, Alpha.\n  SUBPA - The name of a sub parameter to change. Theta ans TransZ support Min, Max and Speed in addition to Value accepted for all parameters.\n  VALUE - The new value to set this parameter to.";
			} else if (token == "movedh") {
				std::cout << " == Help 'movedh' ==\nThis command solves the inverse kinematic equations for an ee-point in DH-coordinates\n -] Params: <RotZ> <TransZ> <TransX> <RotX>\n  RotZ - Rotation around the Z axis (up)\n  TransZ - Translation in Z axis (up)\n  TransX - Translation in rotated X axis (forward)\n  RotX - Rotation around the rotated X axis (forward).";
			} else if (token == "moteto") {
				std::cout << " == Help 'moveto' ==\nThis command solves the inverse kinematic  equations for an ee-point in (X;Y;Z) with a Yaw-Pitch-Roll rotation\n -] Params: <X> <Y> <Z> <Yaw> <Pitch> <Roll>\n  X - Target position in X\n  Y - Target position in Y\n  Z - Exactly, Target position in W; No wait, Z\n  Yaw - Rotation around Z axis (first)\n  Pitch - Rotation around Y axis (second)\n  Roll - Rotation around X axis (last)";
			} else {
				std::cout << "  == Help '" << token << "' ==\nUnknown command\n";
			}
		
		} else {
			std::cout << " =] Robo-Kinematik [= \n"
				<< "Brought to you by Michael and Stefan\n"
				<< "Powered By SDL and Armadillo\n"
				<< "You are running version " << VERSION << " \\o/\n"
				<< "\n  Available commands:\n"
				<< "quit, set, movedh, moveto"
				<< "\n Use help <command> for more information\n";
		}

	} else if (token == "set") {
		int g;
		std::string p,s;
		double v;
		ParamTypeTM n;
		ParamSubTypeTM m;

		if (nextConsoleToken(g) && nextConsoleToken(p) && nextConsoleToken(s) && nextConsoleToken(v)) {
			if (p=="theta") n = Theta;
			else if (p=="transz" || p=="h") n = TransZ;
			else if (p=="transx" || p=="r") n = TransX;
			else if (p=="alpha") n = Alpha;
			else throw runtime_error((string("No such parameter ")+p).c_str());

			if (s=="min") m = Min;
			else if (s=="max") m = Max;
			else if (s=="speed") m = Speed;
			else if (s=="value" || s=="target") m = Value;
			else throw runtime_error((string("No such sub-parameter ")+s).c_str());

			try {
				toVis.push(new SetParamTM(g,n,m,v));
			} rethrow

			std::cout << "Setting " << s << " for joint " << g << ' ' << p << " to " << v << std::endl;
		} else {
			std::cout << "Missing arguments!" << std::endl;
		}
	} else if (token == "movedh") {
		double rz, tz, tx, rx;
		if (nextConsoleToken(rz) && nextConsoleToken(tz) && nextConsoleToken(tx) && nextConsoleToken(rx)) {
			toVis.push(new SetEEinDH(rz,tz,tx,rx));
		} else {
			std::cout << "Missing arguments!" << std::endl;
		}
	} else if (token == "moveto") {
		double x,y,z,u,v,w;
		if (nextConsoleToken(x) && nextConsoleToken(y) && nextConsoleToken(z) && nextConsoleToken(u) && nextConsoleToken(v) && nextConsoleToken(w)) {
			toVis.push(new SetEEinTR(x,y,z,u,w,v));
		} else {
			std::cout << "Missing arguments!" << std::endl;
		}
	} else {
		std::cout << "Command not recognized! Try 'help' :)" << std::endl;
	}

	std::cout << std::endl; //a bit of bottom spacing

	readfds = savefds;
	if (select(1, &readfds, NULL, NULL, &timeout)) {
		std::cin.ignore(1000, '\n');
	}

}
