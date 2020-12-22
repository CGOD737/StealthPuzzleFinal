#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "TurretManager.h"
#include "../GameStateManager.h"
#include "../scenes/game_over.h"


void TurretManager::Init(GameStateManager* game, unsigned int seed, int c_width, int c_height, Tile** levelMap, int levelWidth, int levelHeight, Turret *turrets[], int numTurrets) {
	cam_width = c_width;
	cam_height = c_height;
}
		
void TurretManager::Cleanup();
		
void TurretManager::TurretsFire();
void TurretManager::DrawTurrets();
void TurretManager::DrawTurrets_Hacker();
		
