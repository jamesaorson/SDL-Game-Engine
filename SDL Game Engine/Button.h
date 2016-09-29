#ifndef BUTTON_H
#define BUTTON_H

#include "VisibleEntity.h"

enum ButtonStates {
	BUTTON_UP = 0,
	BUTTON_DOWN = 1,
	TOTAL_BUTTON_STATES = 2
};

class Button : public VisibleEntity {
	public:
		//Constructor
		Button();
		Button(std::string path);

		//Destructor
		~Button();

		//Member functions
		void handleEvent(SDL_Event* e);
};

#endif //BUTTON_H