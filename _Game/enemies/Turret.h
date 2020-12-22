#pragma once
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h> 
#include <iterator>
#include <math.h>
#include "../GameStateManager.h"
#include "../Tile.h"

class Turret{
	public:
		void Init(GameStateManager* game, unsigned int seed, int startX, int startY, int facing, int levelWidth, int levelHeight, int c_width, int c_height);
		void Cleanup();
		void checkFOV(GameStateManager* game, SDL_Rect* agentbox);
		void Draw(SDL_Rect* cam_offset, GameStateManager* game);
		
		bool firing;
		bool gam_ovr;
		int globalX;
		int globalY;
		
	private:
		SDL_Texture* loadImage(std::string fname, GameStateManager* game){
			SDL_Texture* newText = nullptr;

			SDL_Surface* startSurf = IMG_Load(fname.c_str());
			if (startSurf == nullptr) {
				std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
				return nullptr;
			}

			newText = SDL_CreateTextureFromSurface(game->gRenderer, startSurf);
			if (newText == nullptr) {
				std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
				return nullptr;
			}

			SDL_FreeSurface(startSurf);

			return newText;
		};
		
		int direction;
		SDL_Texture* turretTexture;
		SDL_Texture* turretFiringTexture;
		SDL_Rect fovbox;
		SDL_Rect laserbox;
		SDL_Rect guardbox;
		int cam_width;
		int cam_height;
};
