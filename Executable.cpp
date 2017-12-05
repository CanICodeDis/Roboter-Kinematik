#include "Executable.h"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>


#define VERSION "BETA 1.0"

SDL_sem* running;

void handleConsoleInput();
	fd_set readfds;
	fd_set savefds;
	struct timeval timeout;

int main() {
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
			handleConsoleInput();
		}
		SDL_Delay(15);
	}

	SDL_WaitThread(vThread, NULL);
//	SDL_Quit();
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
			} else {
				std::cout << "  == Help '" << token << "' ==\nUnknown command\n";
			}
		
		} else {
			std::cout << " =] Robo-Kinematik [= \n"
				<< "Brought to you by Michael and Stefan\n"
				<< "Powered By SDL and Armadillo\n"
				<< "You are running version " << VERSION << " \\o/\n"
				<< "\n  Available commands:\n"
				<< "quit"
				<< "\n Use help <command> for more information\n";
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
