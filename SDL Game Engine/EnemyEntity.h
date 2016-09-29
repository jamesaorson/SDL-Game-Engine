#ifndef ENEMY_ENTITY_H
#define ENEMY_ENTITY_H

#include "SolidEntity.h"
#include "Vector2D.h"

class EnemyEntity : public SolidEntity {
	public:
		//Constructors
		EnemyEntity();
		EnemyEntity(std::string path);
		EnemyEntity(std::string path, SDL_Rect* box);

		//Destructor
		~EnemyEntity();

	private:
		Vector2D velocity;
};

#endif //ENEMY_ENTITY_H