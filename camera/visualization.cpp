#include "visualization.h"

void sdlEventHandler();
void think(int dt); //dt is time since last frame in ms
void render();
void swapBuffers();

//used for timings, frametime since last frame, do not write!
int frameTime=0; 
double smoothFPS=0.0;

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
	cam.setAngleYaw(-135.0).setAnglePitch(15.0).setDist(10.0).updateT();

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
		if (frameTime<10) {
			SDL_Delay(10-frameTime); //frame-limiting, because cinematic fps ;)
			frameTime=10;
		}
		start+=frameTime;
		smoothFPS = (smoothFPS*2+1000.0/frameTime)/3.0;
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
				double dx = -0.1*e.motion.xrel;
				double dy = 0.1*e.motion.yrel;
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
	roboter->updateMatrices();
//	roboter->getGelenk(1).makeValueTransformMatrix();
}

void renderCoordSys(const Mat<double>& R, const arma::Col<double>& P) {
	Col<double> px = R(span(0,2),0)*0.5+P;
	Col<double> py = R(span(0,2),1)*0.5+P;
	Col<double> pz = R(span(0,2),2)*0.5+P;
	sPoint *sp1, *sp2;

	sp1 = cam.transform(P);

	sp2 = cam.transform(px);
	SDL_SetRenderDrawColor(renderer,  50, 100, 255, 255);
	SDL_RenderDrawLine( renderer, sp1->X(), sp1->Y(), sp2->X(), sp2->Y() );
	delete (sp2); sp2 = cam.transform(py);
	SDL_SetRenderDrawColor(renderer, 255, 200,  50, 255);
	SDL_RenderDrawLine( renderer, sp1->X(), sp1->Y(), sp2->X(), sp2->Y() );
	delete (sp2); sp2 = cam.transform(pz);
	SDL_SetRenderDrawColor(renderer, 255,  50, 75, 255);
	SDL_RenderDrawLine( renderer, sp1->X(), sp1->Y(), sp2->X(), sp2->Y() );
	delete (sp2);
	delete (sp1);
}

void render() {
	//SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
	SDL_RenderFillRect(renderer, &screenSurface->clip_rect);
//	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00) );
	char smsg[128]={0};
	sprintf(smsg, "FPS:% 5i (% 3ims)", (int)smoothFPS, frameTime);
	printText(smsg, 4, 4);
	sprintf(smsg, "Yaw: %.2f  Pitch %.2f  Dist: %.2f", cam.getAngleYaw(), cam.getAnglePitch(), cam.getDist());
	printText(smsg, 4, 20);

	for (int i=1; i<7; i++) {
		gelenk g = roboter->getGelenk(i);
		sprintf(smsg, "Gelenk %i: % 7.3f, % 7.3f, % 7.3f, % 7.3f", g.nummer(), g.giveTheta(), g.giveH(), g.giveR(), g.giveAlpha());
		printText(smsg, 4, 4+16*(i+1) );
	}
//*///


	arma::Col<double> point = {0,0,0};
	arma::Mat<double> rot(3,3,arma::fill::eye);
	sPoint* spPre=cam.transform(point), spNext;

	for (int i=1; i<7; i++) {
		point = roboter->giveAnkleNPositionInWorld(i).head(3);

		sPoint* spNext=cam.transform(point);

		SDL_SetRenderDrawColor( renderer, 255,255,255,255 );
		SDL_RenderDrawLine( renderer, spPre->X(), spPre->Y(), spNext->X(), spNext->Y());
//		std::cout << "Line " << i << " from " << spPre->X() << ", " << spPre->Y() << " to " << spNext->X() << ", " << spNext->Y() << std::endl;
//		std::cout << point << std::endl;

		delete (spPre);
		spPre = spNext;
	}
	delete (spPre);

	point = {0,0,0};
	renderCoordSys(rot, point);
	for (int i=1; i<7; i++) {
		point = roboter->giveAnkleNPositionInWorld(i).head(3);
		rot = roboter->giveAnkleNRotationInWorld(i);

		renderCoordSys(rot, point);
	}

/*///
	arma::Col<double> point = {0,0,0};
	sPoint* sp1 = cam.transform(point);
	point[0]=1;
	sPoint* spX = cam.transform(point);
	point[0]=0; point[1]=1;
	sPoint* spY = cam.transform(point);
	point[1]=0; point[2]=1;
	sPoint* spZ = cam.transform(point);

	SDL_SetRenderDrawColor(renderer,  50, 100, 255, 128);
	SDL_RenderDrawLine( renderer, sp1->X(), sp1->Y(), spX->X(), spX->Y() );
	SDL_SetRenderDrawColor(renderer, 255, 200,  50, 128);
	SDL_RenderDrawLine( renderer, sp1->X(), sp1->Y(), spY->X(), spY->Y() );
	SDL_SetRenderDrawColor(renderer, 255,  50, 100, 128);
	SDL_RenderDrawLine( renderer, sp1->X(), sp1->Y(), spZ->X(), spZ->Y() );

	delete(sp1); delete(spX); delete(spY); delete (spZ);
//*///
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
