#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../GameStateManager.h"


class EnemyManager{
	public:
		void Init(GameStateManager* game, unsigned int seed, int c_width, int c_height, Tile** levelMap, int levelWidth, int levelHeight, Turret *turrets[], int numTurrets);
		void Cleanup();
		
		void TurretsFire();
		void DrawTurrets();
		void DrawTurrets_Hacker();
	private:
		int tile_size = 80;
		int cam_width;
		int cam_height;
		
}

