#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Enemy.h"
#include "../GameStateManager.h"

void PaulBlart::Init(GameStateManager* game, unsigned int seed, int startX, int startY, Tile** levelMap, int levelWidth, int levelHeight) {

	globalX = startX;
	globalY = startY;
	direction = 0;
	distanceCounter = 0.0;
	enemyTexture = loadImage("assets/tiles/PaulBlartSpriteSheet.png", game);
    	speed = 150.0;
    	srand(seed);
    

}

void PaulBlart::Cleanup() {



}

void PaulBlart::Draw(GameStateManager* game, SDL_Rect* dest) {

	SDL_Rect src;

	switch(direction) {
		case 0: //moving right
			src = {240, 0, tileSize, tileSize};
		break;
		case 1: //moving down
			src = {80, 0, tileSize, tileSize};
		break;
		case 2: // moving left 
			src = {0, 0, tileSize, tileSize};
		break;
		case 3: // moving up 
			src = {160, 0, tileSize, tileSize};
		break;
	} 
	SDL_RenderCopy(game->gRenderer, enemyTexture, &src, dest);

}






