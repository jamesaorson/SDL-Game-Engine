#include "Texture.h"

//Constructor
Texture::Texture() {
	texture = nullptr;
	width = DEFAULT_WIDTH;
	height = DEFAULT_HEIGHT;
}

//Destructor
Texture::~Texture() {
	destroy();
}

//Accessors
SDL_Texture* Texture::getTexture() {
	return texture;
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}

//Mutator
void Texture::setTexture(SDL_Texture* tex) {
	if (texture != nullptr) {
		destroy();
	}

	texture = tex;
}

//Member functions
bool Texture::loadTextureFromPath(std::string path) {
	destroy();

	SDL_Texture* newTexture = nullptr;

	//Load image
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == nullptr) {
		printf("Unable to load image %s. SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	} else {
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gameRenderer, loadedSurface);

		if (newTexture == nullptr) {
			printf("Unable to create texture from %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
		} else {
			//Get image dimensions
			width = loadedSurface->w;
			height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	texture = newTexture;
	return texture != nullptr;
}

#ifdef _SDL_TTF_H
bool Texture::loadTextureFromFont(std::string path, SDL_Color fontColor) {
	destroy();

	//Render text surface
	SDL_Surface* fontSurface = TTF_RenderText_Solid(gameFont, path.c_str(), fontColor);

	if (fontSurface != nullptr) {
		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(gameRenderer, fontSurface);

		if (texture == nullptr) {
			printf("Unable to create texture from rendered text. SDL Error: %s\n", SDL_GetError());
		} else {
			//Get image dimensions
			width = fontSurface->w;
			height = fontSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(fontSurface);
	} else {
		printf("Unable to render text surface. SDL_ttf Error: %s\n", TTF_GetError());
	}

	//Return success
	return texture != nullptr;
}
#endif

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	//Set rendering space and render to screen
	SDL_Rect renderRect = { x, y, width, height };

	//Set clip rendering dimensions
	if (clip != nullptr) {
		renderRect.w = clip->w;
		renderRect.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gameRenderer, texture, clip, &renderRect, angle, center, flip);
}

void Texture::destroy() {
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);

		texture = nullptr;
		width = DEFAULT_WIDTH;
		height = DEFAULT_HEIGHT;
	}
}