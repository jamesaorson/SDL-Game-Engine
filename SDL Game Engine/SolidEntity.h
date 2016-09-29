#ifndef SOLID_ENTITY_H
#define SOLID_ENTITY_H

#include "VisibleEntity.h"
#include "Vector2D.h"

class SolidEntity : public VisibleEntity {
	public:
		//Constructors
		SolidEntity();
		SolidEntity(std::string path);
		SolidEntity(std::string path, SDL_Rect* box);
		
		//Destructor
		virtual ~SolidEntity();

		//Accessors
		SDL_Rect* getBoundingBox();

		//Member functions
		void move();
		bool loadTextureFromPath(std::string path);

	protected:
		SDL_Rect boundingBox;
};

#endif //SOLID_ENTITY_H