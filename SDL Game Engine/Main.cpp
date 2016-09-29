#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <sstream>
#include "vld.h"
#include "MainFunctions.h"
#include "Definitions.h"
#include "Texture.h"
#include "Timer.h"
#include "Button.h"
#include "Entity.h"
#include "VisibleEntity.h"
#include "SolidEntity.h"
#include "PlayerEntity.h"
//#include "EnemyEntity.h"

//Usually bad practice to use global variables, but not letting these be global 
//makes the entirety of an SDL game pretty cancerous. It is more important not to 
//use globals if in a team project situation, so just be certain to use specific names

enum GameStates {
	MENU_STATE,
	PLAY_STATE
};

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BUTTON_WIDTH = SCREEN_WIDTH / 2;
const int BUTTON_HEIGHT = SCREEN_HEIGHT / 2;
const int FRAME_RATE_CAP = 60;
const int TICKS_PER_FRAME = 1000 / FRAME_RATE_CAP;
GameStates gameState;
int currentButtonState, previousButtonState;
SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
SDL_Rect rect = camera;
float fps;

//Will be initialized later
SDL_Window* gameWindow = nullptr;
SDL_Renderer* gameRenderer = nullptr;
TTF_Font* gameFont = nullptr;

VisibleEntity background, menuBackground;
Texture startButtonSpriteSheet;
Button startButton;
SDL_Rect buttonSprites[TOTAL_BUTTON_STATES];
PlayerEntity player;
//EnemyEntity enemy;

int main(int argc, char* args[]) {

	if (!init()) {
		printf("Game was unable to initialize correctly, check previous error codes.\n");
	} else {
		if (!loadContent()){
			printf("Game was unable to load media, check previous error codes.\n");
		} else {
			bool quit = false;
			SDL_Event e;

			int countedFrames = 0;
			Timer fpsTimer, cappedRateTimer;
			fpsTimer.start();

			std::stringstream stream;


			//enemy.setPos(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2);

			while (!quit) {
				if (gameState == MENU_STATE && currentButtonState == BUTTON_UP && previousButtonState == BUTTON_DOWN) {
					gameState = PLAY_STATE;
				}

				cappedRateTimer.start();
				
				while ((SDL_PollEvent(&e)) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
					if (e.key.keysym.sym == SDLK_ESCAPE && e.key.repeat == 0) {
						quit = true;
					}
					if (gameState == MENU_STATE) {
						startButton.handleEvent(&e);
					} else if (gameState == PLAY_STATE) {
						player.handleEvent(&e);
					}
				}

				if (gameState == PLAY_STATE) {
					player.move();
					//enemy.move();

					camera.x = (player.getXPos() + player.getTexture()->getWidth() / 2) - SCREEN_WIDTH / 2;
					camera.y = (player.getYPos() + player.getTexture()->getHeight() / 2) - SCREEN_HEIGHT / 2;

					if (camera.x < 0) {
						camera.x = 0;
					}
					if (camera.y < 0) {
						camera.y = 0;
					}
					if (camera.x > LEVEL_WIDTH - camera.w) {
						camera.x = LEVEL_WIDTH - camera.w;
					}
					if (camera.y > LEVEL_HEIGHT - camera.h) {
						camera.y = LEVEL_HEIGHT - camera.h;
					}
				}

				//Calculate and correct fps
				fps = countedFrames / (fpsTimer.getTicks() / 1000.f);
				
				if (fps > 2000000) {
					fps = 0;
				}

				stream << "SDL Game Engine - FPS: " << (int)fps;
				SDL_SetWindowTitle(gameWindow, stream.str().c_str());
				stream.clear();
				stream.str(std::string());

				//Clear screen
				SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gameRenderer);
		
				if (gameState == MENU_STATE) {
					menuBackground.render(0, 0);
					startButton.render(startButton.getXPos(), startButton.getYPos(), &buttonSprites[currentButtonState]);	
				} else if (gameState == PLAY_STATE) {
					background.render(0, 0, &camera);
					player.render(player.getXPos() - camera.x, player.getYPos() - camera.y);
					//enemy.render(enemy.getXPos() - camera.x, enemy.getYPos() - camera.y);
				}

				//Update screen
				SDL_RenderPresent(gameRenderer);
				++countedFrames;

				//If frame finished early
				int frameTicks = cappedRateTimer.getTicks();

				if (frameTicks < TICKS_PER_FRAME) {
					SDL_Delay(TICKS_PER_FRAME - frameTicks);
				}
			}
		}
	}

	return 0;
}

bool init() {

	//Keeps track of whether or not program initialized correctly
	bool success = true;
	gameState = MENU_STATE;

	//Initializes base SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL didn't start. SDL Error: %s\n", SDL_GetError());

		success = false;
	} else {
		//Set texture filtering
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled\n");
		}

		//Initializes window
		gameWindow = SDL_CreateWindow("RPG v1.0 FPS: ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gameWindow == nullptr) {
			printf("Window could not be initialized. SDL Error: %s\n", SDL_GetError());

			success = false;
		} else {
			//Initializes gameRenderer and gives it vSync+
			gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gameRenderer == nullptr) {
				printf("Renderer could not be initialized. SDL Error: %s\n", SDL_GetError());

				success = false;
			} else {
				//Initializes renderer color
				SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initializes SDL_image
				if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());

					success = false;
				}

				//Initializes SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize. SDL_ttf Error: %s\n", TTF_GetError());
					
					success = false;
				}

				//Initializes SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize. SDL_mixer Error: %s\n", Mix_GetError());
					
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadContent() {
	bool success = true;

	/*if (!player.loadTextureFromPath("Content/Textures/<filename>")) {
		printf("Failed to load player texture!\n");
		success = false;
	}*/
	/*if (!enemy.loadTextureFromPath("Content/Textures/<filename>")) {
		printf("Failed to load enemy texture!\n");
		success = false;
	}*/
	if (!background.loadTextureFromPath("Content/Textures/bg.png")) {
		printf("Failed to load background texture!\n");
		success = false;
	}
	if (!menuBackground.loadTextureFromPath("Content/Textures/menubg.png")) {
		printf("Failed to load menu background texture!\n");
		success = false;
	}
	if (!startButtonSpriteSheet.loadTextureFromPath("Content/Textures/startButton.png")) {
		printf("Failed to load start button texture!\n");
		success = false;
	} else {
		for (int i = 0; i < TOTAL_BUTTON_STATES; ++i) {
			buttonSprites[i].x = i * BUTTON_WIDTH;
			buttonSprites[i].y = 0;
			buttonSprites[i].h = BUTTON_HEIGHT;
			buttonSprites[i].w = BUTTON_WIDTH;
		}

		startButton.getTexture()->setTexture(startButtonSpriteSheet.getTexture());
		startButton.setPosVector(LEVEL_WIDTH / 8, LEVEL_HEIGHT / 8);
	}

	return success;
}

void close() {
	SDL_DestroyRenderer(gameRenderer);
	gameRenderer = nullptr;

	SDL_DestroyWindow(gameWindow);
	gameWindow = nullptr;

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool checkCollisions(SDL_Rect* a, SDL_Rect* b) {
	SDL_Rect result = { 0, 0, 0, 0 };

	if (SDL_IntersectRect(a, b, &result) == SDL_TRUE) {
		return true;
	}

	return false;
}