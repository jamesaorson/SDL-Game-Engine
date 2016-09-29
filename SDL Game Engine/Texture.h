#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include "Definitions.h"

extern SDL_Renderer* gameRenderer;
extern TTF_Font* gameFont;

class Texture {
	public:
		//Constructor
		Texture();

		//Destructor
		~Texture();

		//Accessors
		SDL_Texture* getTexture();
		int getWidth();
		int getHeight();

		//Mutator
		void setTexture(SDL_Texture* tex);

		//Member functions
		bool loadTextureFromPath(std::string path);
		#ifdef _SDL_TTF_H
		bool loadTextureFromFont(std::string path, SDL_Color fontColor);
		#endif	
		void render(int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void destroy();

	private:
		SDL_Texture* texture;
		int width, height;
};

#endif //TEXTURE_H