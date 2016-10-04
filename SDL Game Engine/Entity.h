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
		Vector2D getVelVector();
		int getXVel();
		int getYVel();
		int getID();
	
		//Mutators
		virtual void setPosVector(int x, int y);
		virtual void setXPos(int x);
		virtual void setYPos(int y);

		//Member functions
		virtual void move();

	protected:
		Vector2D position;
		Vector2D velocity;
		int ID;
};

#endif //ENTITY_H