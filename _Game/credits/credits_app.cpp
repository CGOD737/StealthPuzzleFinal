#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Surface* credits = NULL;
 

bool init(){
	bool valid = true;
	//init SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL was unable to initialize.");
		valid = false;
	}else{
		//create window, undef location on screen, def height/width
		window = SDL_CreateWindow("Credits", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if(window == NULL){
			printf("Window unable to be created.");
			valid = false;
		}else{
			//init PNG
			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags) & imgFlags)){
               	 		printf("SDL_image could not initialize!");
                		valid = false;
            		}else{
				surface = SDL_GetWindowSurface(window);
			}
		}

	}

	return valid;
}


void close(){
	//close everything down
	SDL_FreeSurface(credits);
	credits = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}

int main(int argc, char* args[]){
	if(!init()){
		printf("Unable to initalize.");
	}else{

	//INSERT IMAGE PATH IN "imgPaths" ARRAY!!!
		//order: marko, emma, connor, anna, turner, patrick, andrew, chris, celest, erik
		
		// Making sure the gitignore worked :)
		
		const char *imgPaths[10] = {"assets/marko.png", "assets/EmmaCredits.jpg", "assets/Connor Robinette.jpg", "assets/AnnaCredits.png", "assets/TurnerCredits.png", "assets/PatrickCredits.png", "assets/DrewSlide.jpg", "assets/ChrisCredits.bmp", "assets/celest-credits.bmp", "assets/ErikCredits.jpg"};



    for(int i = 0; i < 10; i++){
			credits = IMG_Load(imgPaths[i]);
			if(credits == NULL){
				printf("Unable to load image %s!" , i);

			}else{
				SDL_BlitSurface(credits, NULL, surface, NULL);
				SDL_UpdateWindowSurface(window);
				SDL_Delay(1000);
				//clear surface for next image
				SDL_FillRect(surface, NULL, 0x000000);

			}
		}
	close();
	return 0;
	}
}
