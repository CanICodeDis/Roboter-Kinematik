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
	gelenk g1 = roboter->getGelenk(1);

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
		SDL_Delay(100);
	}

	SDL_WaitThread(vThread, NULL);
	SDL_Quit();
	free(roboter);
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
		return true;
	}
	return false;
}
void handleConsoleInput() {
	std::string token;
	std::cin >> token;

	if (token == "quit") {
		std::cout << "Good bye!\n";
		SDL_SemPost(running);
	} else if (token == "help") {

		if (nextConsoleToken(token)) {
		
			if (token == "quit") {
				std::cout << "  == Help 'quit' ==\nThis command quits the application\n -] Params:\n  NONE\n";
			} else if (token == "set") {
				std::cout << "  == Help 'set' ==\nThis command is used to live manipulate the robot\nWe would like to apologize for the lack of graphical manipulation methods\n -] Params: <JOINT> <PARAM> <SUBPA> <VALUE>\n  JOINT - The joint index to manipulate\n  PARAM - The name of the DH param to change. One of: Theta, TransZ, TransX, Alpha.\n  SUBPA - The name of a sub parameter to change. Theta ans TransZ support Min, Max and Speed in addition to Value accepted for all parameters.\n  VALUE - The new value to set this parameter to.";
			} else {
				std::cout << "  == Help '" << token << "' ==\nUnknown command\n";
			}
		
		} else {
			std::cout << " =] Robo-Kinematik [= \n"
				<< "Brought to you by Michael and Stefan\n"
				<< "Powered By SDL and Armadillo\n"
				<< "You are running version " << VERSION << " \\o/\n"
				<< "\n  Available commands:\n"
				<< "quit, set"
				<< "\n Use help <command> for more information\n";
		}

	} else if (token == "set") {
		int g;
		std::string p,s;
		double v;
		ParamTypeTM n;
		ParamSubTypeTM m;

		if (nextConsoleToken(g) && nextConsoleToken(p) && nextConsoleToken(s) && nextConsoleToken(v)) {
			if (p=="Theta") n = Theta;
			else if (p=="TransZ" || p=="H") n = TransZ;
			else if (p=="TransX" || p=="R") n = TransX;
			else if (p=="Alpha") n = Alpha;
			else throw runtime_error((string("No such parameter ")+p).c_str());

			if (s=="Min") m = Min;
			else if (s=="Max") m = Max;
			else if (s=="Speed") m = Speed;
			else if (s=="Value" || s=="Target") m = Value;
			else throw runtime_error((string("No such sub-parameter ")+s).c_str());

			try {
				toVis.push(new SetParamTM(g,n,m,v));
			} rethrow

			std::cout << "Setting " << s << " for joint " << g << ' ' << p << " to " << v << std::endl;
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
