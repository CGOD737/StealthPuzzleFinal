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

class Enemy{
	public:

        double globalX;
        double globalY;
        double detectionRadius = 480.0;
        int state = 0;

        struct pathTile{
                //index into tileset
            int x;
            int y;
                //values needed for PathFinding alg
            int g;
            int h;
            int f;
                //parent's index in tileset
            int parentX;
            int parentY;
        };

		virtual void Init(GameStateManager* game, unsigned int seed, int startX, int startY, Tile** levelMap, int levelWidth, int levelHeight) = 0;
		virtual void Cleanup() = 0;
		virtual void Draw(GameStateManager* game, SDL_Rect* dest) = 0;
		virtual void Move(GameStateManager* game, double timeStep) {

			switch(direction) {
        		case 0: //moving right
        		globalX += speed * timeStep;
        		distanceCounter += speed * timeStep;
        		if(distanceCounter >= 300.0) {
        			globalX -= distanceCounter - 300.0;
        			globalY += distanceCounter - 300.0;
        			distanceCounter = distanceCounter - 300.0;
        			direction = 1;
        		}
        		break;
        		case 1: //moving down
        		globalY += speed * timeStep;
        		distanceCounter += speed * timeStep;
        		if(distanceCounter >= 300.0) {
        			globalY -= distanceCounter - 300.0;
        			globalX -= distanceCounter - 300.0;
        			distanceCounter = distanceCounter - 300.0;
        			direction = 2;
        		}
        		break;
        		case 2: // moving left 
        		globalX -= speed * timeStep;
        		distanceCounter += speed * timeStep;
        		if(distanceCounter >= 300.0) {
        			globalX += distanceCounter - 300.0;
        			globalY -= distanceCounter - 300.0;
        			distanceCounter -= 300.0;
        			direction = 3;
        		}
        		break;
        		case 3: // moving up 
        		globalY -= speed * timeStep;
        		distanceCounter += speed * timeStep;
        		if(distanceCounter >= 300.0) {
        			globalY += distanceCounter - 300.0;
        			globalX += distanceCounter - 300.0;
        			distanceCounter -= 300.0;
        			direction = 0;
        		}
        		break;
			} // end switch 
		}

        virtual void RandomMovePathFind(double timeStep, Tile** levelMap, int levelWidth, int levelHeight) {

            if (movePath.empty()) {
                int randX, randY;

                randX = rand() % levelWidth;
                randY = rand() % levelHeight;

                findPath(randX, randY, levelMap, levelWidth, levelHeight, &movePath);

            } else {

                pathTile* curTile = &movePath.top();

                double curTileGlobalX, curTileGlobalY;

                curTileGlobalX = curTile->x * 80;
                curTileGlobalY = curTile->y * 80;

                double frameSpeedX = speed * timeStep;
                double frameSpeedY = speed * timeStep;

                if (globalX == curTileGlobalX && globalY == curTileGlobalY) {
                    
                    movePath.pop();
                    return;
                    
                }



                if (globalX < curTileGlobalX) {

                    if (frameSpeedX < curTileGlobalX - globalX) {
                        globalX += frameSpeedX;
                        frameSpeedX = 0;
                    } else {
                        globalX = curTileGlobalX;
                        frameSpeedX -= abs(curTileGlobalX - globalX);
                    }

                    direction = 3;

                } else if (globalX > curTileGlobalX) {

                    if (frameSpeedX > curTileGlobalX - globalX) {
                        globalX -= frameSpeedX;
                        frameSpeedX = 0;
                    } else {
                        globalX = curTileGlobalX;
                        frameSpeedX -= abs(curTileGlobalX - globalX);
                    }

                    direction = 2;

                }

                if (globalY < curTileGlobalY) {

                    if (frameSpeedY < curTileGlobalY - globalY) {
                        globalY += frameSpeedY;
                        frameSpeedY = 0;
                    } else {
                        globalY = curTileGlobalY;
                        frameSpeedY = abs(curTileGlobalY - globalY);
                    }

                    direction = 0;

                } else if (globalY > curTileGlobalY) {

                    if (frameSpeedY > curTileGlobalY - globalY) {
                        globalY -= frameSpeedY;
                        frameSpeedY = 0;
                    } else {
                        globalY = curTileGlobalY;
                        frameSpeedY = abs(curTileGlobalY - globalY);
                    }

                    direction = 1;

                }

                //Disables temporarily for testing
                // if (frameSpeedX > 0 || frameSpeedY > 0) {
                //     movePath.pop();
                //     RandomMovePathFindContinue(frameSpeedX, frameSpeedY, levelMap, levelWidth, levelHeight);
                // }

            }

        }

        virtual void RandomMovePathFindContinue (double iframeSpeedX, double iframeSpeedY, Tile** levelMap, int levelWidth, int levelHeight) { 

            if (movePath.empty()) {
                int randX, randY;

                randX = rand() % levelWidth;
                randY = rand() % levelHeight;

                findPath(randX, randY, levelMap, levelWidth, levelHeight, &movePath);

            } else {

                pathTile* curTile = &movePath.top();

                double curTileGlobalX, curTileGlobalY;

                curTileGlobalX = curTile->x * 80;
                curTileGlobalY = curTile->y * 80;

                double frameSpeedX = iframeSpeedX;
                double frameSpeedY = iframeSpeedY;

                if (globalX == curTileGlobalX && globalY == curTileGlobalY) {
                    
                    movePath.pop();
                    return;
                    
                }



                if (globalX < curTileGlobalX) {

                    if (frameSpeedX < curTileGlobalX - globalX) {
                        globalX += frameSpeedX;
                        frameSpeedX = 0;
                    } else {
                        globalX = curTileGlobalX;
                        frameSpeedX -= abs(curTileGlobalX - globalX);
                    }

                    direction = 3;

                } else if (globalX > curTileGlobalX) {

                    if (frameSpeedX > curTileGlobalX - globalX) {
                        globalX -= frameSpeedX;
                        frameSpeedX = 0;
                    } else {
                        globalX = curTileGlobalX;
                        frameSpeedX -= abs(curTileGlobalX - globalX);
                    }

                    direction = 2;

                }

                if (globalY < curTileGlobalY) {

                    if (frameSpeedY < curTileGlobalY - globalY) {
                        globalY += frameSpeedY;
                        frameSpeedY = 0;
                    } else {
                        globalY = curTileGlobalY;
                        frameSpeedY = abs(curTileGlobalY - globalY);
                    }

                    direction = 0;

                } else if (globalY > curTileGlobalY) {

                    if (frameSpeedY > curTileGlobalY - globalY) {
                        globalY -= frameSpeedY;
                        frameSpeedY = 0;
                    } else {
                        globalY = curTileGlobalY;
                        frameSpeedY = abs(curTileGlobalY - globalY);
                    }

                    direction = 1;

                }

                if (frameSpeedX > 0 || frameSpeedY > 0) {
                    movePath.pop();
                    RandomMovePathFindContinue(frameSpeedX, frameSpeedY, levelMap, levelWidth, levelHeight);
                }

            }

        }

        virtual void ChaseAgent (double timeStep, Tile** levelMap, int levelWidth, int levelHeight, double agentX, double agentY) {

        	if (chasePath.empty()) {
                int randX, randY;

                randX = rand() % levelWidth;
                randY = rand() % levelHeight;

                findPath(randX, randY, levelMap, levelWidth, levelHeight, &chasePath);

            } else {

                pathTile* curTile = &chasePath.top();

                double curTileGlobalX, curTileGlobalY;

                curTileGlobalX = curTile->x * 80;
                curTileGlobalY = curTile->y * 80;

                double frameSpeedX = speed * timeStep;
                double frameSpeedY = speed * timeStep;

                if (globalX == curTileGlobalX && globalY == curTileGlobalY) {
                    
                    chasePath.pop();
                    return;
                    
                }



                if (globalX < curTileGlobalX) {

                    if (frameSpeedX < curTileGlobalX - globalX) {
                        globalX += frameSpeedX;
                        frameSpeedX = 0;
                    } else {
                        globalX = curTileGlobalX;
                        frameSpeedX -= abs(curTileGlobalX - globalX);
                    }

                    direction = 3;

                } else if (globalX > curTileGlobalX) {

                    if (frameSpeedX > curTileGlobalX - globalX) {
                        globalX -= frameSpeedX;
                        frameSpeedX = 0;
                    } else {
                        globalX = curTileGlobalX;
                        frameSpeedX -= abs(curTileGlobalX - globalX);
                    }

                    direction = 2;

                }

                if (globalY < curTileGlobalY) {

                    if (frameSpeedY < curTileGlobalY - globalY) {
                        globalY += frameSpeedY;
                        frameSpeedY = 0;
                    } else {
                        globalY = curTileGlobalY;
                        frameSpeedY = abs(curTileGlobalY - globalY);
                    }

                    direction = 0;

                } else if (globalY > curTileGlobalY) {

                    if (frameSpeedY > curTileGlobalY - globalY) {
                        globalY -= frameSpeedY;
                        frameSpeedY = 0;
                    } else {
                        globalY = curTileGlobalY;
                        frameSpeedY = abs(curTileGlobalY - globalY);
                    }

                    direction = 1;

                }

                //Disables temporarily for testing
                // if (frameSpeedX > 0 || frameSpeedY > 0) {
                //     chasePath.pop();
                //     ChaseAgentContinue(frameSpeedX, frameSpeedY, levelMap, levelWidth, levelHeight);
                // }

            }

        }

        virtual void ChaseAgentContinue(double frameSpeedX, double frameSpeedY, Tile** levelMap, int levelWidth, int levelHeight) {

            if (chasePath.empty()) {
                int randX, randY;

                randX = rand() % levelWidth;
                randY = rand() % levelHeight;

                findPath(randX, randY, levelMap, levelWidth, levelHeight, &chasePath);

            } else {

                pathTile* curTile = &chasePath.top();

                double curTileGlobalX, curTileGlobalY;

                curTileGlobalX = curTile->x * 80;
                curTileGlobalY = curTile->y * 80;

                if (globalX == curTileGlobalX && globalY == curTileGlobalY) {
                    
                   chasePath.pop();
                    return;
                    
                }



                if (globalX < curTileGlobalX) {

                    if (frameSpeedX < curTileGlobalX - globalX) {
                        globalX += frameSpeedX;
                        frameSpeedX = 0;
                    } else {
                        globalX = curTileGlobalX;
                        frameSpeedX -= abs(curTileGlobalX - globalX);
                    }

                    direction = 3;

                } else if (globalX > curTileGlobalX) {

                    if (frameSpeedX > curTileGlobalX - globalX) {
                        globalX -= frameSpeedX;
                        frameSpeedX = 0;
                    } else {
                        globalX = curTileGlobalX;
                        frameSpeedX -= abs(curTileGlobalX - globalX);
                    }

                    direction = 2;

                }

                if (globalY < curTileGlobalY) {

                    if (frameSpeedY < curTileGlobalY - globalY) {
                        globalY += frameSpeedY;
                        frameSpeedY = 0;
                    } else {
                        globalY = curTileGlobalY;
                        frameSpeedY = abs(curTileGlobalY - globalY);
                    }

                    direction = 0;

                } else if (globalY > curTileGlobalY) {

                    if (frameSpeedY > curTileGlobalY - globalY) {
                        globalY -= frameSpeedY;
                        frameSpeedY = 0;
                    } else {
                        globalY = curTileGlobalY;
                        frameSpeedY = abs(curTileGlobalY - globalY);
                    }

                    direction = 1;

                }

                if (frameSpeedX > 0 || frameSpeedY > 0) {
                    chasePath.pop();
                    ChaseAgentContinue(frameSpeedX, frameSpeedY, levelMap, levelWidth, levelHeight);
                }

            }

        }

        void goToLastKnown(double timeStep, Tile** levelMap, int levelWidth, int levelHeight) {

            pathTile* curTile = &chasePath.top();

            double curTileGlobalX, curTileGlobalY;

            curTileGlobalX = curTile->x * 80;
            curTileGlobalY = curTile->y * 80;

            double frameSpeedX = speed * timeStep;
            double frameSpeedY = speed * timeStep;

            if (globalX == curTileGlobalX && globalY == curTileGlobalY) {
                
                chasePath.pop();
                return;
                
            }

            if (globalX < curTileGlobalX) {

                if (frameSpeedX < curTileGlobalX - globalX) {
                    globalX += frameSpeedX;
                    frameSpeedX = 0;
                } else {
                    globalX = curTileGlobalX;
                    frameSpeedX -= abs(curTileGlobalX - globalX);
                }

                direction = 3;

            } else if (globalX > curTileGlobalX) {

                if (frameSpeedX > curTileGlobalX - globalX) {
                    globalX -= frameSpeedX;
                    frameSpeedX = 0;
                } else {
                    globalX = curTileGlobalX;
                    frameSpeedX -= abs(curTileGlobalX - globalX);
                }

                direction = 2;

            }

            if (globalY < curTileGlobalY) {

                if (frameSpeedY < curTileGlobalY - globalY) {
                    globalY += frameSpeedY;
                    frameSpeedY = 0;
                } else {
                    globalY = curTileGlobalY;
                    frameSpeedY = abs(curTileGlobalY - globalY);
                }

                direction = 0;

            } else if (globalY > curTileGlobalY) {

                if (frameSpeedY > curTileGlobalY - globalY) {
                    globalY -= frameSpeedY;
                    frameSpeedY = 0;
                } else {
                    globalY = curTileGlobalY;
                    frameSpeedY = abs(curTileGlobalY - globalY);
                }

                direction = 1;

            }

        }

        virtual void FollowPlayer (double timeStep, double agent_x, double agent_y) {

            double frameSpeed = speed * timeStep;

            double difx = agent_x - globalX;
            double dify = agent_y - globalY;

            double angle = atan(dify/difx);

            if (difx > 0) {
                globalX += frameSpeed * cos(angle);
                globalY += frameSpeed * sin(angle);

                if (angle > M_PI / 4) {
                    direction = 0;
                } else if (angle < -M_PI/4) {
                    direction = 1;
                } else {
                    direction = 3;
                }

            } else {
                globalX -= frameSpeed * cos(angle);
                globalY -= frameSpeed * sin(angle);

                if (angle > M_PI / 4) {
                    direction = 1;
                } else if (angle < -M_PI/4) {
                    direction = 0;
                } else {
                    direction = 2;
                }

            }

        }

        virtual void RunAI (double timeStep, Tile** levelMap, int levelWidth, int levelHeight, double agentX, double agentY, int alarmX, int alarmY) {

        	double agentDistance = sqrt((globalX - agentX) * (globalX - agentX) + (globalY - agentY) * (globalY - agentY)); // agent dist from enemy

            //State 0 is for randomly patrolling across the map
            if (state == 0) {
                // if (levelMap[(int)(globalX / 80)][(int)(globalY / 80)].getCollision() == true) {
                //     printf("DOOR STUCK! DOOR STUCK!\n");
                //     return;
                // }
                RandomMovePathFind(timeStep, levelMap, levelWidth, levelHeight);

                if (lineOfSight(agentX, agentY, levelMap, levelWidth, levelHeight) == true && agentDistance <= detectionRadius && inFOV(agentX, agentY)) {

                    ClearChasePath();
                    ClearMovePath();

                    state = 1;

                }

            //State 1 is for following the player when the AI has direct line of sight
            } else if (state == 1) {
                // if (levelMap[(int)(globalX / 80)][(int)(globalY / 80)].getCollision() == true) {
                //     printf("DOOR STUCK! DOOR STUCK!\n");
                //     return;
                // }
                FollowPlayer(timeStep, agentX, agentY); // Makes the enemy face the player

                if (agentDistance > detectionRadius) {

                    ClearChasePath();
                    ClearMovePath();

                    state = 0;

                } else if (lineOfSight(agentX, agentY, levelMap, levelWidth, levelHeight) == false) {

                    ClearChasePath();
                    ClearMovePath();

                    int agentIntX = agentX / 80;
                    int agentIntY = agentY / 80;

                    findPath(agentIntX, agentIntY, levelMap, levelWidth, levelHeight, &chasePath);

                    state = 2;

                }

            //State 2 is for going to the player's last known position.
            } else if (state == 2) {
                // if (levelMap[(int)(globalX / 80)][(int)(globalY / 80)].getCollision() == true) {
                //     printf("DOOR STUCK! DOOR STUCK!\n");
                //     return;
                // }
                goToLastKnown(timeStep, levelMap, levelWidth, levelHeight);

                if (chasePath.empty()) {

                    ClearChasePath();
                    ClearMovePath();

                    state = 0;

                } else if (lineOfSight(agentX, agentY, levelMap, levelWidth, levelHeight) == true && agentDistance <= detectionRadius) {

                    ClearChasePath();
                    ClearMovePath();

                    state = 1;

                }

            //This is the KO state for Guards.
            } else if (state == 3) {
                // if (levelMap[(int)(globalX / 80)][(int)(globalY / 80)].getCollision() == true) {
                //     printf("DOOR STUCK! DOOR STUCK!\n");
                //     return;
                // }
                if (sleepTimer < 0.0) {

                    sleepTimer = 5.0;

                    ClearChasePath();
                    ClearMovePath();

                    state = 0;

                } else {

                    sleepTimer -= timeStep;

                }

            }

        }

        void ClearChasePath () {
        	while (!chasePath.empty()) {
        		chasePath.pop();
            }
        }

        void ClearMovePath () {
            while (!movePath.empty()) {
                movePath.pop();
            }
        }

        void SetChasePath (double agentX, double agentY, Tile** levelMap, int levelWidth, int levelHeight) {
            int agentIntX = agentX / 80;
            int agentIntY = agentY / 80;
            findPath(agentIntX, agentIntY, levelMap, levelWidth, levelHeight, &chasePath);
        }

        bool lineOfSight (double agentX, double agentY, Tile** levelMap, int levelWidth, int levelHeight) {

            int centerAgentX, centerAgentY, centerGlobalX, centerGlobalY;

            centerAgentX = agentX + 40.0;
            centerAgentY = agentY + 40.0;
            centerGlobalX = globalX + 40.0;
            centerGlobalY = globalY + 40.0;

            for (int i = 0; i < levelWidth; i++) {
                for (int j = 0; j < levelHeight; j++) {

                    if (levelMap[i][j].getCollision() == true) {

                        SDL_Rect test = {j * 80, i * 80, 80, 80};

                        if (SDL_IntersectRectAndLine(&test, &centerGlobalX, &centerGlobalY, &centerAgentX, &centerAgentY) == true) {
                            return false;
                        }

                    }

                }
            }

            return true;

        }

        bool inFOV (double agentX, double agentY) {

            SDL_Point playerPoint = {agentX, agentY};
            SDL_Rect FOV;

            if (direction == 0) {

                FOV.x = globalX - viewField;
                FOV.y = globalY;
                FOV.w = 2 * viewField;
                FOV.h = detectionRadius;

            } else if (direction == 1) {

                FOV.x = globalX - viewField;
                FOV.y = globalY - detectionRadius;
                FOV.w = 2 * viewField;
                FOV.h = detectionRadius;

            } else if (direction == 2) {

                FOV.x = globalX - detectionRadius;
                FOV.y = globalY - viewField;
                FOV.w = detectionRadius;
                FOV.h = 2 * viewField;

            } else if (direction == 3) {

                FOV.x = globalX;
                FOV.y = globalY - viewField;
                FOV.w = detectionRadius;
                FOV.h = 2 * viewField;

            }

            if (SDL_PointInRect(&playerPoint, &FOV) == true) {
                return true;
            } else {
                return false;
            }

        }

        void stateDraw(GameStateManager* game, SDL_Rect* dest) {

            SDL_Rect src;
            dest->x += 20;
            dest->y -= 40;
            dest->w = 40;
            dest->h = 40;

            switch(state) {
                case 1: 
                    src = {0, 0, tileSize/2, tileSize/2};
                    SDL_RenderCopy(game->gRenderer, stateTexture, &src, dest);
                break;
                case 2: 
                    src = {40, 0, tileSize/2, tileSize/2};
                    SDL_RenderCopy(game->gRenderer, stateTexture, &src, dest);
                break;
                case 3: 
                    src = {80, 0, tileSize/2, tileSize/2};
                    SDL_RenderCopy(game->gRenderer, stateTexture, &src, dest);
                break;
            } 

        }

	protected: 

		int direction;
		double distanceCounter;
		double speed;
		std::stack<pathTile> movePath;
		std::stack<pathTile> chasePath;
        SDL_Texture* enemyTexture;
        SDL_Texture* stateTexture;
        int tileSize = 80;
        double sleepTimer = 5.0;
        int viewField;

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
			}

			SDL_FreeSurface(startSurf);

			return newText;
		};

        //Returns and deletes the best open
        pathTile getBestOpen (std::vector<pathTile>* openList) {

            pathTile result, checker;
            result.f = -1;
            std::vector<pathTile>::iterator resultIndex;

            for (std::vector<pathTile>::iterator it = openList->begin(); it != openList->end(); ++it) {

                checker = *it;

                if (result.f == -1) {
                    result = checker;
                    resultIndex = it;
                } else {
                    if (checker.f < result.f) {
                        result = checker;
                        resultIndex = it;
                    }
                }
            }

            //if (isOnOpenList(result.x, result.y, openList)){
                openList->erase(resultIndex);
            //}

            return result;

        }

        //Returns true if the indicated X and Y value exist as a Tile in the list
        bool isOnCloseList (int checkX, int checkY, std::vector<pathTile>* closeList) {

            for (std::vector<pathTile>::iterator it = closeList->begin(); it != closeList->end(); ++it) {

                if (it->x == checkX && it->y == checkY){
                    return true;
                }

            }

            return false;

        }

        //Returns true if the indicated X and Y value exist as a Tile in the list
        bool isOnOpenList (int checkX, int checkY, std::vector<pathTile>* openList) {

            for (std::vector<pathTile>::iterator it = openList->begin(); it != openList->end(); ++it) {

                if (it->x == checkX && it->y == checkY){
                    return true;
                }

            }

            return false;

        }

        //Returns a pointer to the Tile that matches the X and Y
        pathTile* getMatchOpenList (int checkX, int checkY, std::vector<pathTile>* openList) {

            for (std::vector<pathTile>::iterator it = openList->begin(); it != openList->end(); ++it) {

                if (it->x == checkX && it->y == checkY){
                    return &*it;
                }

            }

        }

        void findPath(int targetX, int targetY, Tile** levelMap, int levelWidth, int levelHeight, std::stack<pathTile>* curPath) {
            
            if (levelMap[targetY][targetX].getCollision() == true) {
                return;
            }

            std::vector <pathTile> openList, closeList;

            pathTile start;
            pathTile curTile;

            int enemyX, enemyY, newX, newY;

            //Flooring the enemy's global coordinates to use as ints in an array
            enemyX = globalX / 80;
            enemyY = globalY / 80;

            // if (levelMap[(int)(globalX / 80)][(int)(globalY / 80)].getCollision() == true) {
            //     printf("DOOR STUCK! DOOR STUCK!\n");
            //     return;
            // }

            //setting the starting tile
            start.x = enemyX;
            start.y = enemyY;
            start.g = 0;
            start.h = 0;
            start.f = 0;
            start.parentX = -1;
            start.parentY = -1;

            //Add the starting tile to the openList
            openList.push_back(start);

            while (!isOnCloseList(targetX, targetY, &closeList)) {

                if (openList.empty()) {
                    return;
                }

                //Gets the best open and deletes it from the open list
                curTile = getBestOpen(&openList);

                //Adds the curTile to the closeList
                closeList.push_back(curTile);

                if (isOnCloseList(targetX, targetY, &closeList)) { // found the target
                    break;
                }

                for (int i = -1; i < 2; i++) {
                    for (int j = -1; j < 2; j++) {

                        newX = curTile.x + i;
                        newY = curTile.y + j;

                        if (newX < 0 || newX >= levelWidth || newY < 0 || newY >= levelHeight) {
                            continue;
                        }

                        //Checks to make sure the tile is walkable and not on the closeList, otherwise skips the tile
                        if (levelMap[newY][newX].getCollision() == false && isOnCloseList(newX, newY, &closeList) == false && (i != 0 || j != 0)) {

                        	if (i == -1 && j == -1) {
                                if (levelMap[newY+1][newX].getCollision() == true || levelMap[newY][newX+1].getCollision() == true) {
                                    continue;
                                }
                            } else if (i == 1 && j == -1) {
                                if (levelMap[newY+1][newX].getCollision() == true || levelMap[newY][newX-1].getCollision() == true) {
                                    continue;
                                }
                            } else if (i == -1 && j == 1) {
                                if (levelMap[newY-1][newX].getCollision() == true || levelMap[newY][newX+1].getCollision() == true) {
                                    continue;
                                }
                            } else if (i == 1 && j == 1) {
                                if (levelMap[newY-1][newX].getCollision() == true || levelMap[newY][newX-1].getCollision() == true) {
                                    continue;
                                }
                            }

                            pathTile newTile;

                            newTile.x = newX;
                            newTile.y = newY;

                            if (i == 0 || j == 0){
                                newTile.g = curTile.g + 10;
                            } else {
                                newTile.g = curTile.g + 14;
                            }

                            newTile.h = (abs(targetX - newX) + abs(targetY - newY)) * 10;

                            newTile.f = newTile.g + newTile.h;

                            newTile.parentX = curTile.x;
                            newTile.parentY = curTile.y;

                            if (isOnOpenList(newX, newY, &openList) == false) {

                                openList.push_back(newTile);

                            } else {

                                pathTile* existingTile = getMatchOpenList(newX, newY, &openList);

                                if (newTile.g < existingTile->g) {

                                    existingTile->parentX = curTile.x;
                                    existingTile->parentY = curTile.y;

                                    existingTile->g = newTile.g;
                                    existingTile->f = existingTile->g + existingTile->h;

                                }

                            }

                        }

                    }
                }

            }

            pathTile stackAdd = closeList.back();

            int lookForLeak = 0;

            while (stackAdd.parentX != -1) {
                
                curPath->push(stackAdd);
                stackAdd = *getMatchOpenList(stackAdd.parentX, stackAdd.parentY, &closeList);



            }

        }

};

class PaulBlart : public Enemy {
	public: 

		void Init(GameStateManager* game, unsigned int seed, int startX, int startY, Tile** levelMap, int levelWidth, int levelHeight);
		void Cleanup();
		void Draw(GameStateManager* game, SDL_Rect* dest);
		void Move(GameStateManager* game, double timeStep) {
			Enemy::Move(game, timeStep);
		}

	private:

	void PathFind(GameStateManager* game, char** levelMap, int destX, int destY, int mapWidth, int mapHeight);
    bool searchPathList(pathTile tileList[], pathTile look, int listSize);
    pathTile getBestOpen(pathTile tileList[], int listSize);
};

class Paula : public Enemy {
	public: 

		void Init(GameStateManager* game, unsigned int seed, int startX, int startY, Tile** levelMap, int levelWidth, int levelHeight);
		void Cleanup();
		void Draw(GameStateManager* game, SDL_Rect* dest);
		void Move(GameStateManager* game, double timeStep) {
			Enemy::Move(game, timeStep);
		}

		void FollowPlayer(GameStateManager* game, double timeStep, double agent_x, double agent_y);

	private:
};

class BasicGuard : public Enemy {

    public: 

        void Init(GameStateManager* game, unsigned int seed, int startX, int startY, Tile** levelMap, int levelWidth, int levelHeight);
        void Cleanup();
        void Draw(GameStateManager* game, SDL_Rect* dest);

};

class AverageGuard : public Enemy {

    public: 

        void Init(GameStateManager* game, unsigned int seed, int startX, int startY, Tile** levelMap, int levelWidth, int levelHeight);
        void Cleanup();
        void Draw(GameStateManager* game, SDL_Rect* dest);

};

class ProGuard : public Enemy {

    public: 

        void Init(GameStateManager* game, unsigned int seed, int startX, int startY, Tile** levelMap, int levelWidth, int levelHeight);
        void Cleanup();
        void Draw(GameStateManager* game, SDL_Rect* dest);

};


