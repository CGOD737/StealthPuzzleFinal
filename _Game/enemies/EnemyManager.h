#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../GameStateManager.h"
#include "Enemy.h"

class EnemyManager{
	public:
		void Init(GameStateManager* game, unsigned int seed, int c_width, int c_height, Tile** levelMap, int levelWidth, int levelHeight, Enemy *enemies[], int numEnemies);
		void Cleanup();

		void KO(double agentX, double agentY, Enemy *enemies[], int numEnemies);

		void MoveEnemies(GameStateManager* game, double timeStep, Tile** levelMap, int levelWidth, int levelHeight, SDL_Rect* agentbox, double agentX, double agentY, Enemy *enemies[], int numEnemies, int alarmX, int alarmY, bool endGame, bool* game_ovr);
		void DrawEnemies(SDL_Rect* cam_offset, GameStateManager* game, Enemy *enemies[], int numEnemies);
		void DrawEnemies_Hacker(SDL_Rect* cam_offset, GameStateManager* game, Enemy *enemies[], int numEnemies);

  	private:
	// void GenerateLevel(int objectives, int rooms, int levelWidth, int levelHeight, int tileSize);

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

	/*
	Randomly selects an objective from a predetermined list, then generates psuedo-random x,y coordinates for objective. Each objective's location should be a minimum required distance from existing objectives placed previously. As of now, only place objectives on tiles marked with a 0. Based on the Objective selected, a min height/width will be drawn around it in the array.
	*/
	//void CreateObjective(int[][] levelArray);
	
	/*
	Generates a psuedo-random x,y starting position based on existing 0's, 1's, and 2's, then turns the 0's around the starting position to 1's based on the minHeight/minWidth. Finally, expands on the room psuedo-randomly (need to figure out an algorithm to keep the expansions from creating a ton of winding corridors that lead to nowhere).
	*/
	// void CreateRoom(int minHeight, int minWidth, int[][] levelArray);
	
	/*
	Psuedo-randomly fills in the space between rooms so the Agent can walk between them.
	*/
	// void ConnectRooms(int[][] levelArrayint);
	int mapSizeX = 5;
	int mapSizeY = 5;
	int tile_size = 80;
	int cam_width;
	int cam_height;
	unsigned int seed;

	SDL_Texture* hacker_tile_texture;
};
