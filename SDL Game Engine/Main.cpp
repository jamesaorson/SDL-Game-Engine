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
#include "EnemyEntity.h"

/* Usually bad practice to use global variables, but not letting these be global
 * over-complicates the use of functions and implementation of classes in this case.
 * Change to a non-global variable structure if in a team project situation
 * where not everyone will have access to this source code or if implementing
 * these files with other files. If use of this game engine is completely
 * self-contained within this engine, you will find this global structure
 * to be effective and very friendly for use of the engine.
 */

/* Enum of valid states the game can be in. This is the easiest/simplest
 * way of tracking the current state of the game. Any additional states
 * that may be found as necessary can simply be added to this.
 */
enum GameStates {
	MENU_STATE,
	PLAY_STATE
};

/* These variables have been declared simply for demonstration purposes.
 * These variables will be customized to whatever game this engine is
 * used to make. This engine currently does not support scaling,
 * meaning textures must be made for a chosen screen resolution/ratio.
 */
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
//This basic camera may be best replaced later with an OpenGL Camera
SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
float fps;

/* Will be initialized later, but these are necessary for using
 * SDL's rendering functions and for the game graphics as a whole.
 */
SDL_Window* gameWindow = nullptr;
SDL_Renderer* gameRenderer = nullptr;
TTF_Font* gameFont = nullptr;

/* This section declares all of the base individual textures
 * and objects which will be initialized at start of main.
 */
VisibleEntity background, menuBackground;
Texture startButtonSpriteSheet;
Button startButton;
//Array of rectangles which will be used to clip the spritesheet.
SDL_Rect buttonSprites[TOTAL_BUTTON_STATES];
PlayerEntity player;
EnemyEntity enemy;

int main(int argc, char* args[]) {

	//If initialization goes wrong, prints error message and ends program.
	if (!init()) {
		printf("Game was unable to initialize correctly, check previous error codes.\n");
	} else {
		//If content loads wrong, prints error message and ends program.
		if (!loadContent()){
			printf("Game was unable to load media, check previous error codes.\n");
		} else {
			bool quit = false;
			//Will hold current polled event.
			SDL_Event e;

			int countedFrames = 0;
			Timer fpsTimer, cappedRateTimer;
			fpsTimer.start();

			std::stringstream stream;

			//Sets enemy near middle of level.
			enemy.setPosVector(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2);

			while (!quit) {
				//Enters play state only after start button has been released
				if (gameState == MENU_STATE && currentButtonState == BUTTON_UP && previousButtonState == BUTTON_DOWN) {
					gameState = PLAY_STATE;
				}

				//Start timer to be certain we keep framerate fromm exceeding 60 fps.
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

				/* All logic of play state is set in it's own if statement.
				 * By using a gameState variable, it allows the game's logic
				 * to be more easily partitioned into understandable
				 * sets of instructions.
				 */
				if (gameState == PLAY_STATE) {
					player.move();
					//enemy.move();
					
					/* This is commented out, for I moved the collision logic inside  of
					 * the player.move() function to solve an error explained in function.
					 */
					/*if (checkCollisions(player.getBoundingBox(), enemy.getBoundingBox())) {
						player.setPosVector(player.getXPos() - player.getXVel(), player.getYPos() - player.getYVel());
						enemy.setPosVector(enemy.getXPos() - enemy.getXVel(), enemy.getYPos() - enemy.getYVel());
					}*/

					/* Translates camera relative to the position of the player,
					 * so the center of the player is the camera focus.
					 */
					camera.x = (player.getXPos() + player.getTexture()->getWidth() / 2) - SCREEN_WIDTH / 2;
					camera.y = (player.getYPos() + player.getTexture()->getHeight() / 2) - SCREEN_HEIGHT / 2;

					//Makes sure camera doesn't show past the bounds of thee level.
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
				
				//Disregards excessively high framerates sometimes calculated at initialization.
				if (fps > 2000000) {
					fps = 0;
				}

				stream << "SDL Game Engine - FPS: " << (int)fps;
				SDL_SetWindowTitle(gameWindow, stream.str().c_str());
				stream.clear();
				stream.str(std::string());

				//Clears back buffer and sets it to white to prepare for new drawing.
				SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gameRenderer);
		
				//Different rendering logic is separated by state of game.
				if (gameState == MENU_STATE) {
					menuBackground.render(0, 0);
					startButton.render(startButton.getXPos(), startButton.getYPos(), &buttonSprites[currentButtonState]);	
				} else if (gameState == PLAY_STATE) {
					/* Everything in the PLAY_STATE must be rendered to the back buffer 
					 * relative to the camera to actually move with it.
					 */
					background.render(0, 0, &camera);
					player.render(player.getXPos() - camera.x, player.getYPos() - camera.y);
					enemy.render(enemy.getXPos() - camera.x, enemy.getYPos() - camera.y);
				}

				//Flash the back buffer onto the screen.
				SDL_RenderPresent(gameRenderer);
				++countedFrames;

				//If frame finished early, wait in order to maintatin framerate.
				int frameTicks = cappedRateTimer.getTicks();

				if (frameTicks < TICKS_PER_FRAME) {
					SDL_Delay(TICKS_PER_FRAME - frameTicks);
				}
			}
		}
	}
	return 0;
}

/* Initializes all of the SDL elements of the game. If
 * something goes wrong here, there is an issue with dll
 * files or the overall installation of SDL. A lot of this
 * initialization logic should stay untouched in all circumstances.
 */
bool init() {
	bool success = true;
	gameState = MENU_STATE;

	//Initializes base SDl.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL didn't start. SDL Error: %s\n", SDL_GetError());

		success = false;
	} else {
		//Set texture filtering.
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled\n");
		}

		//Initializes game window.
		gameWindow = SDL_CreateWindow("RPG v1.0 FPS: ", SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (gameWindow == nullptr) {
			printf("Window could not be initialized. SDL Error: %s\n", SDL_GetError());

			success = false;
		} else {
			//Initializes gameRenderer and gives it vSync.
			gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		
			if (gameRenderer == nullptr) {
				printf("Renderer could not be initialized. SDL Error: %s\n", SDL_GetError());

				success = false;
			} else {
				//Initializes back buffer color to white.
				SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initializes SDL_image.
				if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());

					success = false;
				}

				//Initializes SDL_ttf.
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize. SDL_ttf Error: %s\n", TTF_GetError());
					
					success = false;
				}

				//Initializes SDL_mixer.
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

/*  
 */
bool loadContent() {
	bool success = true;

	if (!player.loadTextureFromPath("Content/Textures/blackBox.png")) {
		printf("Failed to load player texture!\n");
		success = false;
	}
	if (!enemy.loadTextureFromPath("Content/Textures/redBox.png")) {
		printf("Failed to load enemy texture!\n");
		success = false;
	}
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