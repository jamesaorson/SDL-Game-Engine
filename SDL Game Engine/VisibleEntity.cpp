#include "VisibleEntity.h"

//Constructors
VisibleEntity::VisibleEntity() : Entity() {
}

VisibleEntity::VisibleEntity(std::string path) {
	texture.loadTextureFromPath(path);
}

VisibleEntity::VisibleEntity(std::string path, SDL_Color fontColor) {
	texture.loadTextureFromFont(path, fontColor);
}

//Destructor
VisibleEntity::~VisibleEntity() {
	texture.destroy();
}

//Accessors
Texture* VisibleEntity::getTexture() {
	return &texture;
}

//Member functions
void VisibleEntity::move() {
	position.x += velocity.x;
	position.y += velocity.y;

	if ((position.x < 0) || (position.x + texture.getWidth() > LEVEL_WIDTH)) {
		position.x -= velocity.x;
	}
	if ((position.y < 0) || (position.y + texture.getHeight() > LEVEL_WIDTH)) {
		position.y -= velocity.y;
	}
}

bool VisibleEntity::loadTextureFromPath(std::string path) {
	return texture.loadTextureFromPath(path);
}

#ifdef _SDL_TTF_H
bool VisibleEntity::loadTextureFromFont(std::string path, SDL_Color fontColor) {
	return texture.loadTextureFromFont(path, fontColor);
}
#endif

void VisibleEntity::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	texture.render(x, y, clip, angle, center, flip);
}