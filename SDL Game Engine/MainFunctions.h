#ifndef MAIN_FUNCTIONS_H
#define MAIN_FUNCTIONS_H

#include "SDL.h"

bool init();
bool loadContent();
void close();
bool checkCollisions(SDL_Rect* a, SDL_Rect* b);

#endif //MAIN_FUNCTIONS_H