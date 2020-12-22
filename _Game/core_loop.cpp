#include "GameStateManager.h"
#include "scenes/main_menu.h"

int main ( int argc, char *argv[] )
{

	GameStateManager game;
	game.Init("Hatman and Login");
	game.ChangeState( main_menu::Instance() );

	// main loop
	while ( game.Running() )
	{
		game.HandleEvents();
		game.Update();
		game.Draw();

	}

	// cleanup the engine
	game.Cleanup();

	return 0;
}
