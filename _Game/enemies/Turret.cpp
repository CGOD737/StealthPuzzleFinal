#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../GameStateManager.h"
#include "../scenes/game_over.h"
#include "Turret.h"


void Turret::Init(GameStateManager* game, unsigned int seed, int startX, int startY, int facing, int levelWidth, int levelHeight, int c_width, int c_height) {
	globalX = startX;
	globalY = startY;
	direction = facing;
	turretTexture = loadImage("assets/tiles/turret.png", game);
	turretFiringTexture = loadImage("assets/tiles/turret_firing.png", game);   
	firing = false;
	cam_width = c_width;
	cam_height = c_height;
	
	switch(direction) {
		case 0: // right
			fovbox = {globalX + 80, globalY - 40, 160, 160};
			laserbox = {globalX + 80, globalY + 28, 160, 10};
			break;
		case 1: // down
			fovbox = {globalX - 40, globalY + 80, 160, 160};
			laserbox = {globalX + 27, globalY + 80, 24, 160};
			break;
		case 2: // left
			fovbox = {globalX - 160, globalY - 40, 160, 160};
			laserbox = {globalX, globalY + 28, 160, 10};
			break;
		case 3: // up
			fovbox = {globalX - 40, globalY - 160, 160, 160};
			laserbox = {globalX + 27, globalY, 24, 160};
			break;
	}
}

void Turret::Cleanup() {
	
}

void Turret::checkFOV(GameStateManager* game, SDL_Rect* agentbox) {
	// laser hits player
	// check before checking intersection so the lasers will render for at least one frame 
	if(firing && SDL_HasIntersection(agentbox, &laserbox)) {
		gam_ovr = true;
		game->ChangeState(game_over::Instance());
	}
	
	if(SDL_HasIntersection(agentbox, &fovbox)) {
		firing = true;
	}
	else {
		firing = false;
	}
}

void Turret::Draw(SDL_Rect* cam_offset, GameStateManager* game) {

	SDL_Rect src;
	
	int sc_pos_x = globalX - cam_offset->x + (cam_width/2) - 40; 
	int sc_pos_y = globalY - cam_offset->y + (cam_height/2) - 40;
		
	// Make sure the tiles are inside the camera
	bool draw_x = false;
	bool draw_y = false;
	// do -tile_size to make sure if tile starts off screen but should still show
	if (sc_pos_x > -80 && sc_pos_x < cam_width){
		draw_x = true;
	} 

	if (sc_pos_y > -80 && sc_pos_y < cam_height){
		 draw_y = true;
	}
		  
	SDL_Rect dest = {sc_pos_x, sc_pos_y, 80, 80};
	// Draw the corresponding tile
	if (draw_x && draw_y){
		SDL_SetRenderDrawColor(game->gRenderer, 255, 0, 0, 255);
		switch(direction) {
			case 0: //right
			src = {240, 0, 80, 80};
			if(firing) { // draw lasers
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x + 80, sc_pos_y + 28, sc_pos_x + 240, sc_pos_y + 28);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x + 80, sc_pos_y + 29, sc_pos_x + 240, sc_pos_y + 29);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x + 80, sc_pos_y + 32, sc_pos_x + 240, sc_pos_y + 32);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x + 80, sc_pos_y + 33, sc_pos_x + 240, sc_pos_y + 33);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x + 80, sc_pos_y + 36, sc_pos_x + 240, sc_pos_y + 36);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x + 80, sc_pos_y + 37, sc_pos_x + 240, sc_pos_y + 37);
			}
			break;
		case 1: //down
			src = {0, 0, 80, 80};
			if(firing) { // draw lasers
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x + 27, sc_pos_y + 80, sc_pos_x + 27, sc_pos_y + 240);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x + 28, sc_pos_y + 80, sc_pos_x + 28, sc_pos_y + 240);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x + 50, sc_pos_y + 80, sc_pos_x + 50, sc_pos_y + 240);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x + 51, sc_pos_y + 80, sc_pos_x + 51, sc_pos_y + 240);
			}
			break;
		case 2: //left 
			src = {80, 0, 80, 80};
			if(firing) { // draw lasers
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x, sc_pos_y + 28, sc_pos_x - 160, sc_pos_y + 28);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x, sc_pos_y + 29, sc_pos_x - 160, sc_pos_y + 29);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x, sc_pos_y + 32, sc_pos_x - 160, sc_pos_y + 32);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x, sc_pos_y + 33, sc_pos_x - 160, sc_pos_y + 33);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x, sc_pos_y + 36, sc_pos_x - 160, sc_pos_y + 36);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x, sc_pos_y + 37, sc_pos_x - 160, sc_pos_y + 37);
			}
			break;
		case 3: //up 
			src = {160, 0, 80, 80};
			if(firing) { // draw lasers
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x + 27, sc_pos_y, sc_pos_x + 27, sc_pos_y - 160);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x + 28, sc_pos_y, sc_pos_x + 28, sc_pos_y - 160);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x + 50, sc_pos_y, sc_pos_x + 50, sc_pos_y - 160);
				SDL_RenderDrawLine(game->gRenderer, sc_pos_x + 51, sc_pos_y, sc_pos_x + 51, sc_pos_y - 160);
			}
			break;
		} 
	
		// reset color for agent line 
		SDL_SetRenderDrawColor(game->gRenderer, 0, 0, 0, 255);
		
		if(firing) {
			SDL_RenderCopy(game->gRenderer, turretFiringTexture, &src, &dest);
		}
		else {
			SDL_RenderCopy(game->gRenderer, turretTexture, &src, &dest);
		}

	}
	
	
}






