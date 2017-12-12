#ifndef __TERMINAL_TERMIOS_GETCH_H__
#define __TERMINAL_TERMIOS_GETCH_H__

#include <stdio.h>
#include <termios.h>

static struct termios oldTermios, newTermios;

void initTermios(int echo);
void resetTermios();
char getch_(int echo);
char getch_();
char getche_();

#endif
