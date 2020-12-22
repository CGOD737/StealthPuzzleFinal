#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#include "Enemy.h"
#include "../GameStateManager.h"
#include "../Tile.h"

void BasicGuard::Init(GameStateManager* game, unsigned int seed, int startX, int startY, Tile** levelMap, int levelWidth, int levelHeight) {

	globalX = startX;
	globalY = startY;
	direction = 0;
	distanceCounter = 0;
	enemyTexture = loadImage("assets/tiles/greasy_guard.png", game);
	stateTexture = loadImage("assets/enemysymbols.png", game);
    	speed = 200.0;
    	viewField = 40;
    //srand(seed);

}

void BasicGuard::Cleanup() {



}

void BasicGuard::Draw(GameStateManager* game, SDL_Rect* dest) {

	SDL_Rect src;

	switch(direction) {
		case 0: //Down
			src = {0, 0, tileSize, tileSize};
		break;
		case 1: //Up
			src = {80, 0, tileSize, tileSize};
		break;
		case 2: // Left
			src = {160, 0, tileSize, tileSize};
		break;
		case 3: //Right 
			src = {240, 0, tileSize, tileSize};
		break;
	} 
	SDL_RenderCopy(game->gRenderer, enemyTexture, &src, dest);

}
