#ifndef VISIBLE_ENTITY_H
#define VISIBLE_ENTITY_H

#include "Entity.h"
#include "Texture.h"
#include "Vector2D.h"

class VisibleEntity : public Entity {
	public:
		//Constructors
		VisibleEntity();
		VisibleEntity(std::string path);
		VisibleEntity(std::string path, SDL_Color fontColor);

		//Destructor
		virtual ~VisibleEntity();

		//Accessors
		Texture* getTexture();

		//Member functions
		void move();
		virtual bool loadTextureFromPath(std::string path);
		#ifdef _SDL_TTF_H
		bool loadTextureFromFont(std::string path, SDL_Color fontColor);
		#endif
		void render(int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

	protected:
		Texture texture;
};

#endif //VISIBLE_ENTITY_H/