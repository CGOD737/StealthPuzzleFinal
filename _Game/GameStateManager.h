#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class GameState;

class GameStateManager{
	public:
		void Init(const char* title, int width=1080, int height=720);
		void Cleanup();

		void ChangeState(GameState* state);
		void PushState(GameState* state);
		void PopState();

		void HandleEvents();
		void Update();
		void Draw();

		bool Running(){ return running; }
		void Quit(){ running = false; }

		SDL_Window* gWindow = nullptr;
		SDL_Renderer* gRenderer = nullptr;

	private:
		// the stack of states
		std::vector<GameState*> states;

		bool running;
};