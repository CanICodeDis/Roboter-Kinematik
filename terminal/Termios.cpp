#include "Termios.h"

void initTermios(int echo) {
	tcgetattr(0, &oldTermios);
	newTermios = oldTermios;
	newTermios.c_lflag &=~ ICANON;
	if (echo)
		newTermios.c_lflag |= ECHO;
	else
		newTermios.c_lflag &=~ ECHO;
	tcsetattr(0, TCSANOW, &newTermios);
}
void resetTermios() {
	tcsetattr(0, TCSANOW, &oldTermios);
}
char getch_(int echo) {
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}
char getch_() {
	return getch_(0);
}
char getche_() {
	return getch_(1);
}
