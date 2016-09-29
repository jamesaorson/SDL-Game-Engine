#include "PlayerEntity.h"

//Constructors
PlayerEntity::PlayerEntity() : SolidEntity() {
	velocity.x = 0;
	velocity.y = 0;
}

PlayerEntity::PlayerEntity(std::string path) : SolidEntity(path) {
	velocity.x = 0;
	velocity.y = 0;
}

PlayerEntity::PlayerEntity(std::string path, SDL_Rect* box) : SolidEntity(path, box) {
	velocity.x = 0;
	velocity.y = 0;
}

//Destructor
PlayerEntity::~PlayerEntity() {}

//Member functions
void PlayerEntity::handleEvent(SDL_Event* e) {
	if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
		switch (e->key.keysym.sym) {
			case SDLK_UP:
				velocity.y -= 5;
				break;
			case SDLK_DOWN:
				velocity.y += 5;
				break;
			case SDLK_LEFT:
				velocity.x -= 5;
				break;
			case SDLK_RIGHT:
				velocity.x += 5;
				break;
		}
	} else if (e->type == SDL_KEYUP && e->key.repeat == 0) {
		switch (e->key.keysym.sym) {
			case SDLK_UP:
				velocity.y += 5;
				break;
			case SDLK_DOWN:
				velocity.y -= 5;
				break;
			case SDLK_LEFT:
				velocity.x += 5;
				break;
			case SDLK_RIGHT:
				velocity.x -= 5;
				break;
		}
	}
}