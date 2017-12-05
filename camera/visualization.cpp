#include "visualization.h"

void sdlEventHandler();
void think(int dt); //dt is time since last frame in ms
void render();
void swapBuffers();

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

// == ROBOT RELATED STUFF ==
Camera cam;
arma::Col<double> p1 = arma::Col<double>(3);
arma::Col<double> pX = arma::Col<double>(3);
arma::Col<double> pY = arma::Col<double>(3);
arma::Col<double> pZ = arma::Col<double>(3);

int thVisual(void* data) {

	cam.setAngleYaw(0.0).setAnglePitch(-10.0).setDist(10.0).updateT();

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

	int start = SDL_GetTicks();
	for(;SDL_SemValue(running)==0;) {
		sdlEventHandler();
		think(frameTime);
		render();
		swapBuffers();

		frameTime = SDL_GetTicks()-start;
		start+=frameTime;
	}

//	TTF_Quit();
//	SDL_DestroyRenderer(renderer);
//	SDL_DestroyWindow(window);
}

void sdlEventHandler() {
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 ) { //handle all events happening since the last main-loop
		if( e.type == SDL_QUIT ) {
			SDL_SemPost(running);
		}
	}
}

double deg=0.0, deg2=0.0;
void think(int ms) {
	//360/1000
	deg+=0.036*ms;
	while (deg>=360.0) deg-=360.0;
	deg2+=0.01*ms;
	while (deg2>=360.0) deg2-=360.0;
	cam.setAngleYaw(deg).setAnglePitch(cos(_DEG2RAD(deg2))*5.0+10.0).updateT();

	//if (frameTime<5) SDL_Delay(5-frameTime); //frame-limiting, because cinematic fps ;)
}

void render() {
	//SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
	SDL_RenderFillRect(renderer, &screenSurface->clip_rect);
//	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00) );
	char smsg[64]={0};
	sprintf(smsg, "DT: %ims", frameTime);
	printText(smsg, 4, 4);
	sprintf(smsg, "Yaw: %.2f  Pitch %.2f  Dist: %.2f", cam.getAngleYaw(), cam.getAnglePitch(), cam.getDist());
	printText(smsg, 4, 20);

	sPoint sp1 = cam.getScreenPoint(p1);
	sPoint spX = cam.getScreenPoint(pX);
	sPoint spY = cam.getScreenPoint(pY);
	sPoint spZ = cam.getScreenPoint(pZ);
	/*
	std::cout << deg << "deg" << frameTime << "m  " << 
					sp1.X() << ";" << sp1.Y() << ";" << sp1.Depth() << ", " << 
					spX.X() << ";" << spX.Y() << ";" << spX.Depth() << ", " << 
					spY.X() << ";" << spY.Y() << ";" << spY.Depth() << ", " << 
					spZ.X() << ";" << spZ.Y() << ";" << spZ.Depth()  << std::endl;
					*/
	SDL_SetRenderDrawColor(renderer,  50, 100, 255, 128);
	SDL_RenderDrawLine( renderer, sp1.X(), sp1.Y(), spX.X(), spX.Y() );
	SDL_SetRenderDrawColor(renderer, 255, 200,  50, 128);
	SDL_RenderDrawLine( renderer, sp1.X(), sp1.Y(), spY.X(), spY.Y() );
	SDL_SetRenderDrawColor(renderer, 255,  50, 100, 128);
	SDL_RenderDrawLine( renderer, sp1.X(), sp1.Y(), spZ.X(), spZ.Y() );

	cam.clear();
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
