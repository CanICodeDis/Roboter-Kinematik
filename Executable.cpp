#include <armadillo>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <ctime>

#define TFONT_NAME "DejaVuSansMono.ttf"
#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 480

void sdlEventHandler();
void think(int dt); //dt is time since last frame in ms
void render();
void swapBuffers();

//The main loop will not exit until this variable is set to false:
bool running = true;

//used for timings, frametime since last frame, do not write!
int frameTime=0;

//The window we'll be rendering to 
SDL_Window* window = NULL; 
SDL_Renderer* renderer = NULL;

//The surface contained by the window 
SDL_Surface* screenSurface = NULL; 

TTF_Font* _tfont; //The default font to draw with in printText
SDL_Color _tcolor = {255, 255, 255, 255}; //default text color for printText (format: rgba)
void printText(char*, int x, int y); //paint a text at x,y within a rect of w,h

int main(int argc, char** argv) {

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() ); 
		return 1;
	}

	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	if( window == NULL || renderer == NULL ) {
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return 1;
	}
	
	screenSurface = SDL_GetWindowSurface( window );

	TTF_Init();
	_tfont = TTF_OpenFont(TFONT_NAME, 16); //The default font to draw with in printText
	if( _tfont == NULL ) {
		printf( "Could not find font! (%s)\n", TFONT_NAME );
		return 1;
	}

	clock_t start = clock();
	for(;running;) {
		sdlEventHandler();
		think(frameTime);
		render();
		swapBuffers();

		frameTime = float(clock()-start)*1000.0/CLOCKS_PER_SEC;
		start = clock();
	}

	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void sdlEventHandler() {
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 ) { //handle all events happening since the last main-loop
		if( e.type == SDL_QUIT ) {
			running = false;
		}
	}
}

void think(int ms) {
	SDL_Delay(5);
}

void render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
//	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00) );
	char fpsmsg[64]={0};
	sprintf(fpsmsg, "DT: %ims", frameTime);
	printText(fpsmsg, 0, 0);
}

void swapBuffers() {
//	SDL_UpdateWindowSurface( window );
	SDL_RenderPresent( renderer );
}

void printText(char* msg, int x, int y) {
	SDL_Surface* surfMsg = TTF_RenderText_Solid(_tfont, msg, _tcolor);
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfMsg);
	SDL_Rect msRect;
	msRect.x=x;
	msRect.y=y;
	msRect.w=surfMsg->w;
	msRect.h=surfMsg->h;
	SDL_RenderCopy(renderer, message, NULL, &msRect);
//	free(message);
//	free(surfMsg);
	SDL_DestroyTexture(message);
	SDL_FreeSurface(surfMsg);
}
