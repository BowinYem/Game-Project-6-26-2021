#pragma once
#ifndef IMAGE_TEXTURE
#define IMAGE_TEXTURE
#include <SDL.h>
#include <SDL_image.h>
#include <String>
#include <iostream>

class Image_Texture
{
public:
	
	~Image_Texture();
	bool load(SDL_Renderer*&, SDL_Window*&,  const std::string&);
	bool render(SDL_Renderer*&, SDL_Rect* = nullptr, int = 0, int = 0, int = 0, int = 0, SDL_RendererFlip = SDL_FLIP_NONE);
	bool make_transparent(SDL_Window*&);
	int get_height() { return height; }
	int get_width() { return width;  }

private:

	bool lock();
	bool unlock();

	static SDL_Color color_key;
	SDL_Texture* texture; 
	int height;
	int width;
	void* pixels;
	int pitch;
};

#endif