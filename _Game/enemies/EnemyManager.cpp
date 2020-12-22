#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "EnemyManager.h"
#include "../GameStateManager.h"
#include "Enemy.h"
#include "../scenes/game_over.h"

void EnemyManager::Init(GameStateManager* game, unsigned int seed, int c_width, int c_height, Tile** levelMap, int levelWidth, int levelHeight, Enemy *enemies[], int numEnemies) {
	
	for (int i = 0; i < numEnemies; i++) {

		int type = rand() % 3;

		int randX = -1;
		int randY = -1;

		while (randX == -1) {

			randX = rand() % levelWidth;
			randY = rand() % levelHeight;

			if (levelMap[randY][randX].getCollision() == true) {
				randX = -1;
				randY = -1;
			}
			
		}



		if (type == 0) {
			enemies[i] = new BasicGuard;
		} else if (type == 1) {
			enemies[i] = new AverageGuard;//new AverageGuard;
		} else if (type == 2) {
			enemies[i] = new ProGuard;//new ProGuard;
		} else {
			printf("Guard Skin Error\n");
		}
		enemies[i]->Init(game, seed, randX * 80, randY * 80, levelMap, levelWidth, levelHeight);
	}

	cam_width = c_width;
	cam_height = c_height;
	// will load in all tilemaps needed here, not just floor tilesj

}

void EnemyManager::Cleanup() {
	SDL_DestroyTexture(hacker_tile_texture);
	hacker_tile_texture = nullptr;
}

void EnemyManager::KO(double agentX, double agentY, Enemy *enemies[], int numEnemies) {

	for (int i = 0; i < numEnemies; i++) {

		double agentDistance = sqrt((enemies[i]->globalX - agentX) * (enemies[i]->globalX - agentX) + (enemies[i]->globalY - agentY) * (enemies[i]->globalY - agentY));

		if (agentDistance < 120 && enemies[i]->state == 0) {
			enemies[i]->state = 3;
		}

	}

}

void EnemyManager::MoveEnemies(GameStateManager* game, double timeStep, Tile** levelMap, int levelWidth, int levelHeight, SDL_Rect* agentbox, double agentX, double agentY, Enemy *enemies[], int numEnemies, int alarmX, int alarmY, bool endGame, bool* game_ovr) {
	
	for (int i = 0; i < numEnemies; i++) {

		enemies[i]->Enemy::RunAI(timeStep, levelMap, levelWidth, levelHeight, agentX, agentY, alarmX, alarmY);

		SDL_Rect paulabox = {enemies[i]->globalX + 20, enemies[i]->globalY + 10, 40, 60};
		if(SDL_HasIntersection(&paulabox, agentbox)) {
			enemies[i]->Enemy::ClearChasePath();
			enemies[i]->detectionRadius = 320.0;
			if(endGame){
				*game_ovr = true;
			}
		} 
		
		if(alarmX != -1 && enemies[i]->state == 0) {
			enemies[i]->Enemy::ClearChasePath();
			enemies[i]->Enemy::ClearMovePath();
			enemies[i]->Enemy::SetChasePath(agentX, agentY, levelMap, levelWidth, levelHeight);
            enemies[i]->state = 2; 
		} 
	}

	alarmX = -1;
}

// should be called every frame
// may eventually implement a Draw Early and Draw Late for before and after player is drawn if we decide to not make it pure top down view
void EnemyManager::DrawEnemies(SDL_Rect* cam_offset, GameStateManager* game, Enemy *enemies[], int numEnemies) {
  // 0,0 will be in the top left 

	for (int k = 0; k < numEnemies; k++) {
		int i = 0;
		int j = 0;
		SDL_Rect SrcR;
		SDL_Rect DestR;

		SrcR = {0,0,tile_size,tile_size};
		Enemy *cur = enemies[k];
		//calculate where on the screen tile should be based on camera offset
		int sc_pos_x = (cur->globalX) - cam_offset->x + (cam_width/2) - (tile_size/2); //(i*tilesize gives us unbiased tile pos)
		int sc_pos_y = (cur->globalY) - cam_offset->y + (cam_height/2) - (tile_size/2);
		
		// Make sure the tiles are inside the camera
		bool draw_x = false;
		bool draw_y = false;
		// do -tile_size to make sure if tile starts off screen but should still show
		if (sc_pos_x > -tile_size && sc_pos_x < cam_width){
			draw_x = true;
		} 

		if (sc_pos_y > -tile_size && sc_pos_y < cam_height){
		  draw_y = true;
		}
		  
		DestR = {sc_pos_x, sc_pos_y,tile_size,tile_size};
		// Draw the corresponding tile
		if (draw_x && draw_y){

			cur->Draw(game, &DestR);
			cur->Enemy::stateDraw(game, &DestR);
		}
	}

}

// should be called every frame
// may eventually implement a Draw Early and Draw Late for before and after player is drawn if we decide to not make it pure top down view
void EnemyManager::DrawEnemies_Hacker(SDL_Rect* cam_offset, GameStateManager* game, Enemy *enemies[], int numEnemies) {
	hacker_tile_texture = nullptr;

	SDL_Surface* startSurf1 = IMG_Load("assets/tiles/hacker_map_tiles.png");

	hacker_tile_texture = SDL_CreateTextureFromSurface(game->gRenderer, startSurf1);

	SDL_FreeSurface(startSurf1);

	// ABOVE NEEDS TO BE DELETED!!!! Should be put in init and done using loadImage, game breaks otherwise though. Find out why?
  // 0,0 will be in the top left 

	for (int k = 0; k < numEnemies; k++) {

		int i = 0;
		int j = 0;
		SDL_Rect SrcR;
		SDL_Rect DestR;

		
		Enemy *cur = enemies[k];
		//calculate where on the screen tile should be based on camera offset
		int sc_pos_x = (cur->globalX*.25) - cam_offset->x + (cam_width/2) - (20/2); //(i*tilesize gives us unbiased tile pos)
		int sc_pos_y = (cur->globalY*.25) - cam_offset->y + (cam_height/2) - (20/2);
		
		// Make sure the tiles are inside the camera
		bool draw_x = false;
		bool draw_y = false;
		// do -tile_size to make sure if tile starts off screen but should still show
		if (sc_pos_x > -20 && sc_pos_x < cam_width){
			draw_x = true;
		} 

		if (sc_pos_y > -20 && sc_pos_y < cam_height){
		  draw_y = true;
		}
		  
		DestR = {sc_pos_x, sc_pos_y,20,20};
		SrcR = {40,0,20,20};
		// Draw the corresponding tile
		if (draw_x && draw_y){
			SDL_RenderCopy(game->gRenderer, hacker_tile_texture, &SrcR, &DestR);
		}

	}

}
