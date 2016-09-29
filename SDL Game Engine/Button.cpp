#include "Button.h"

//Constructors
Button::Button() : VisibleEntity() {}

Button::Button(std::string path) : VisibleEntity(path) {}

//Destructor
Button::~Button() {

}
//Member functions
void Button::handleEvent(SDL_Event*e) {
	previousButtonState = currentButtonState;
	
	Vector2D mousePos;
	SDL_GetMouseState(&mousePos.x, &mousePos.y);

	bool inside = true;

	if (mousePos.x < position.x) {
		inside = false;
	} else if (mousePos.x > position.x + BUTTON_WIDTH) {
		inside = false;
	} else if (mousePos.y < position.y) {
		inside = false;
	} else if (mousePos.y > position.y + BUTTON_HEIGHT) {
		inside = false;
	}

	if (inside) {
		if (e->type == SDL_MOUSEBUTTONDOWN) {
			currentButtonState = BUTTON_DOWN;
		}
		if (e->type == SDL_MOUSEBUTTONUP) {
			currentButtonState = BUTTON_UP;
		}
	} else if (e->type == SDL_MOUSEBUTTONUP) {
		currentButtonState = BUTTON_UP;
		previousButtonState = BUTTON_UP;
	}
}