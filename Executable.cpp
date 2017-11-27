#include <armadillo>
#include <SDL2/SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 480

int main(int argc, char** argv) {
	//The window we'll be rendering to 
	SDL_Window* window = NULL; 
	
	//The surface contained by the window 
	SDL_Surface* screenSurface = NULL; 
	
	//Initialize SDL 
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() ); 
		return 1;
	}

	window = SDL_CreateWindow( "SDL_ Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if( window == NULL ) {
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return 1;
	}
	
	screenSurface = SDL_GetWindowSurface( window );
	
	//Fill the surface white
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF) );
	
	//Update surface - swapBuffers
	SDL_UpdateWindowSurface( window );

	//Wait 
	SDL_Delay( 2000 );
}
