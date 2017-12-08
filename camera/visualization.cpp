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

int thVisual(void* data) {

	SDL_SetRelativeMouseMode(SDL_TRUE);
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

	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void sdlEventHandler() {
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 ) { //handle all events happening since the last main-loop
		if( e.type == SDL_QUIT ) {
			SDL_SemPost(running);
		} else if (e.type == SDL_MOUSEMOTION) {
			if (e.motion.state == SDL_BUTTON_LMASK) {
				double dy = 0.04*e.motion.yrel;
				double dx = 0.04*e.motion.xrel;
				cam.setAngleYaw(cam.getAngleYaw()+dx)
					.setAnglePitch(cam.getAnglePitch()+dy)
					.updateT();
			} else if (e.motion.state == SDL_BUTTON_RMASK) {
				double d = 0.125*e.motion.yrel;
				cam.setDist(cam.getDist()+d)
					.updateT();
			}
		}
	}
}

void think(int ms) {
	toVis.pop();
	bDouble::tick(ms);
	roboter->getGelenk(1).makeValueTransformMatrix();
	if (frameTime<1) SDL_Delay(1); //frame-limiting, because cinematic fps ;)
}

void renderCoordSys(const Mat<double>& R, const arma::Col<double>& P) {
//	Col<double> px = {1,0,0};
//	Col<double> py = {0,1,0};
//	Col<double> pz = {0,0,1};
	std::cout << R << std::endl << std::endl;
	Col<double> px = R(span(0,2),0)+P;
	Col<double> py = R(span(0,2),1)+P;
	Col<double> pz = R(span(0,2),2)+P;
	sPoint sp1, sp2;
//	px = P + (R*px);
//	py = P + (R*py);
//	pz = P + (R*pz);

	sp1 = cam.getScreenPoint(P);

	sp2 = cam.getScreenPoint(px);
	SDL_SetRenderDrawColor(renderer,  50, 100, 255, 255);
	SDL_RenderDrawLine( renderer, sp1.X(), sp1.Y(), sp2.X(), sp2.Y() );
	sp2 = cam.getScreenPoint(py);
	SDL_SetRenderDrawColor(renderer, 255, 200,  50, 255);
	SDL_RenderDrawLine( renderer, sp1.X(), sp1.Y(), sp2.X(), sp2.Y() );
	sp2 = cam.getScreenPoint(pz);
	SDL_SetRenderDrawColor(renderer, 255,  50, 75, 255);
	SDL_RenderDrawLine( renderer, sp1.X(), sp1.Y(), sp2.X(), sp2.Y() );
}
//void renderCoordSys(const Mat<double>& T) {
//	renderCoordSys(T.submat(0,0,2,2), T(span(0,2),3));
//}

void render() {
	//SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
	SDL_RenderFillRect(renderer, &screenSurface->clip_rect);
//	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00) );
	char smsg[128]={0};
	sprintf(smsg, "DT: %ims", frameTime);
	printText(smsg, 4, 4);
	sprintf(smsg, "Yaw: %.2f  Pitch %.2f  Dist: %.2f", cam.getAngleYaw(), cam.getAnglePitch(), cam.getDist());
	printText(smsg, 4, 20);

	for (int i=1; i<7; i++) {
		gelenk g = roboter->getGelenk(i);
		sprintf(smsg, "Gelenk %i: % 3.2f, % 3.2f, % 3.2f, % 3.2f", g.nummer(), g.giveTheta(), g.giveH(), g.giveR(), g.giveAlpha());
		printText(smsg, 4, 4+16*(i+1) );
	}

	arma::Col<double> nul = {0,0,0};
	arma::Mat<double> eye(3,3,arma::fill::eye);
	SDL_SetRenderDrawColor( renderer, 255,255,255,255);

	sPoint* spNul=cam.transform(nul);
	nul = roboter->getGelenk(1).getTransformation().translation();
 	sPoint* spA=cam.transform(nul);
	SDL_RenderDrawLine( renderer, spNul->X(), spNul->Y(), spA->X(), spA->Y());
	delete (spNul);
	delete (spA);

	renderCoordSys(eye, nul);
	eye = roboter->getGelenk(1).getTransformation().rotation();
	renderCoordSys(eye, nul);

/*
	SDL_SetRenderDrawColor(renderer,  50, 100, 255, 128);
	SDL_RenderDrawLine( renderer, sp1.X(), sp1.Y(), spX.X(), spX.Y() );
	SDL_SetRenderDrawColor(renderer, 255, 200,  50, 128);
	SDL_RenderDrawLine( renderer, sp1.X(), sp1.Y(), spY.X(), spY.Y() );
	SDL_SetRenderDrawColor(renderer, 255,  50, 100, 128);
	SDL_RenderDrawLine( renderer, sp1.X(), sp1.Y(), spZ.X(), spZ.Y() );
*/
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
