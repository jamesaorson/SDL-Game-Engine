#include "SolidEntity.h"
#include "Vector2D.h"

//Constructors
SolidEntity::SolidEntity() : VisibleEntity() {
	boundingBox.w = DEFAULT_WIDTH;
	boundingBox.h = DEFAULT_HEIGHT;
	boundingBox.x = DEFAULT_X;
	boundingBox.y = DEFAULT_Y;
}

SolidEntity::SolidEntity(std::string path) : VisibleEntity(path) {
	boundingBox.w = texture.getWidth();
	boundingBox.h = texture.getHeight();
	boundingBox.x = position.x;
	boundingBox.y = position.y;
}

SolidEntity::SolidEntity(std::string path, SDL_Rect* box) : VisibleEntity(path) {
	boundingBox = *box;
}

//Destructor
SolidEntity::~SolidEntity() {}

//Accessors
SDL_Rect* SolidEntity::getBoundingBox() {
	return &boundingBox;
}

//Member functions
void SolidEntity::move() {
	position.x += velocity.x;
	position.y += velocity.y;

	if ((position.x < 0) || (position.x + texture.getWidth() > LEVEL_WIDTH)) {
		position.x -= velocity.x;
	}
	if ((position.y < 0) || (position.y + texture.getHeight() > LEVEL_WIDTH)) {
		position.y -= velocity.y;
	}

	boundingBox.x = position.x;
	boundingBox.y = position.y;
}

bool SolidEntity::loadTextureFromPath(std::string path) {
	bool success = VisibleEntity::loadTextureFromPath(path);

	boundingBox.w = texture.getWidth();
	boundingBox.h = texture.getHeight();

	return success;
}