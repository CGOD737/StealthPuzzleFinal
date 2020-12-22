
all:	GameStateManager.o game_loop.o main_menu.o core_loop.o Tile.o Level.o hacker_window.o EnemyManager.o PaulBlart.o Paula.o BasicGuard.o AverageGuard.o ProGuard.o Turret.o game_won.o game_over.o button_puzzle.o wire_puzzle.o temp_puzzle.o keypad_puzzle.o PuzzleManager.o NetworkClient.o
	g++ GameStateManager.o game_loop.o main_menu.o core_loop.o Tile.o Level.o hacker_window.o EnemyManager.o PaulBlart.o Paula.o BasicGuard.o AverageGuard.o ProGuard.o Turret.o game_won.o game_over.o button_puzzle.o wire_puzzle.o keypad_puzzle.o temp_puzzle.o PuzzleManager.o NetworkClient.o -w -lSDL2 -lSDL2_image -lSDL2_net -o bin/game
	g++ _Game/server.cpp Tile.o Level.o -w -lSDL2 -lSDL2_image -lSDL2_net -o bin/server

core_loop.o:		_Game/core_loop.cpp
	g++ -c _Game/core_loop.cpp -w -lSDL2 -lSDL2_image

hacker_window.o:	_Game/scenes/hacker_window.cpp _Game/NetworkClient.h 
	g++ -c _Game/scenes/hacker_window.cpp -w -lSDL2 -lSDL2_image -lSDL2_net

NetworkClient.o:	_Game/NetworkClient.cpp _Game/NetworkClient.h
	g++ -c _Game/NetworkClient.cpp -w -lSDL2 -lSDL2_net

GameStateManager.o:	_Game/GameStateManager.cpp _Game/GameStateManager.h
	g++ -c _Game/GameStateManager.cpp -w -lSDL2 -lSDL2_image

Tile.o:	_Game/Tile.cpp _Game/Tile.h 
	g++ -c _Game/Tile.cpp -w -lSDL2 -lSDL2_image
	
Level.o:	_Game/Level.cpp _Game/Level.h _Game/Tile.h Tile.o
	g++ Tile.o -c _Game/Level.cpp -w -lSDL2 -lSDL2_image

EnemyManager.o:	_Game/enemies/EnemyManager.cpp _Game/enemies/EnemyManager.h 
	g++ -c _Game/enemies/EnemyManager.cpp -w -lSDL2 -lSDL2_image

PuzzleManager.o: _Game/puzzles/PuzzleManager.cpp _Game/puzzles/PuzzleManager.h 
	g++ -c _Game/puzzles/PuzzleManager.cpp -w -lSDL2 -lSDL2_image

button_puzzle.o: _Game/puzzles/button_puzzle.cpp
	g++ -c _Game/puzzles/button_puzzle.cpp -w -lSDL2 -lSDL2_image
	
wire_puzzle.o: _Game/puzzles/wire_puzzle.cpp
	g++ -c _Game/puzzles/wire_puzzle.cpp -w -lSDL2 -lSDL2_image
	
keypad_puzzle.o: _Game/puzzles/keypad_puzzle.cpp
	g++ -c _Game/puzzles/keypad_puzzle.cpp -w -lSDL2 -lSDL2_image

temp_puzzle.o: _Game/puzzles/temp_puzzle.cpp
	g++ -c _Game/puzzles/temp_puzzle.cpp -w -lSDL2 -lSDL2_image

PaulBlart.o:	_Game/enemies/PaulBlart.cpp _Game/enemies/Enemy.h 
	g++ -c _Game/enemies/PaulBlart.cpp -w -lSDL2 -lSDL2_image

Paula.o:	_Game/enemies/Paula.cpp _Game/enemies/Enemy.h 
	g++ -c _Game/enemies/Paula.cpp -w -lSDL2 -lSDL2_image

BasicGuard.o:	_Game/enemies/BasicGuard.cpp _Game/enemies/Enemy.h 
	g++ -c _Game/enemies/BasicGuard.cpp -w -lSDL2 -lSDL2_image

AverageGuard.o:	_Game/enemies/AverageGuard.cpp _Game/enemies/Enemy.h 
	g++ -c _Game/enemies/AverageGuard.cpp -w -lSDL2 -lSDL2_image

ProGuard.o:	_Game/enemies/ProGuard.cpp _Game/enemies/Enemy.h 
	g++ -c _Game/enemies/ProGuard.cpp -w -lSDL2 -lSDL2_image
	
Turret.o:	_Game/enemies/Turret.cpp _Game/enemies/Turret.h 
	g++ -c _Game/enemies/Turret.cpp -w -lSDL2 -lSDL2_image

game_loop.o:	_Game/scenes/game_loop.cpp _Game/scenes/game_loop.h _Game/Level.h 
	g++ -c _Game/scenes/game_loop.cpp -w -lSDL2 -lSDL2_image

main_menu.o:	_Game/scenes/main_menu.cpp _Game/scenes/main_menu.h _Game/NetworkClient.h
	g++ -c _Game/scenes/main_menu.cpp -w -lSDL2 -lSDL2_image -lSDL2_net

game_over.o:	_Game/scenes/game_over.cpp _Game/scenes/game_over.h
	g++ -c _Game/scenes/game_over.cpp -w -lSDL2 -lSDL2_image

game_won.o:	_Game/scenes/game_won.cpp _Game/scenes/game_won.h
	g++ -c _Game/scenes/game_won.cpp -w -lSDL2 -lSDL2_image

clean:
	rm -f *.o bin/game bin/server
