#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>
#include <stdio.h>
#include "Definitions.h"
#include "Vector2D.h"

extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int BUTTON_WIDTH;
extern const int BUTTON_HEIGHT;
extern int currentButtonState;
extern int previousButtonState;
extern SDL_Rect camera;
extern SDL_Rect rect;

class Entity {
	public:
		//Constructors
		Entity();

		//Destructor
		virtual ~Entity();
		//Accessors
		Vector2D getPosVector();
		int getXPos();
		int getYPos();
		int getID();
	
		//Mutators
		void setPosVector(int x, int y);
		void setXPos(int x);
		void setYPos(int y);

	protected:
		Vector2D position;
		int ID;
};

#endif //ENTITY_H