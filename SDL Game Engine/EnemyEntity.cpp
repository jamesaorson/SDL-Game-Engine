#include "EnemyEntity.h"

//Constructors
EnemyEntity::EnemyEntity() : SolidEntity() {
	velocity.x = 0;
	velocity.y = 0;
}

EnemyEntity::EnemyEntity(std::string path) : SolidEntity(path) {
	velocity.x = 0;
	velocity.y = 0;
}

EnemyEntity::EnemyEntity(std::string path, SDL_Rect* box) : SolidEntity(path, box) {
	velocity.x = 0;
	velocity.y = 0;
}

//Destructor
EnemyEntity::~EnemyEntity() {}