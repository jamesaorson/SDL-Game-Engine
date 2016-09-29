#ifndef PLAYER_TEAM_ENTITY_H
#define PLAYER_TEAM_ENTITY_H

#include "SolidEntity.h"

class PlayerEntity : public SolidEntity {
	public:
		//Constructors
		PlayerEntity();
		PlayerEntity(std::string path);
		PlayerEntity(std::string path, SDL_Rect* box);

		//Destructor
		~PlayerEntity();

		//Member functions
		void handleEvent(SDL_Event* e);
};

#endif //PLAYER_TEAM_ENTITY_H