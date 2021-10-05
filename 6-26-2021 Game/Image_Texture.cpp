#include "Image_Texture.h"

SDL_Color Image_Texture::color_key = { 0, 255, 255 };

Image_Texture::~Image_Texture()
{
	SDL_DestroyTexture(texture);
	texture = nullptr;
}

bool Image_Texture::load(SDL_Renderer*& renderer, SDL_Window*& window, const std::string& path)
{
	SDL_Surface* load_surface = nullptr;
	SDL_Surface* convert_surface = nullptr;
	SDL_Texture* load_texture = nullptr;

	load_surface = IMG_Load(path.c_str());
	if (!load_surface)
	{
		std::cout << "ERROR CREATING LOAD SURFACE: " << SDL_GetError() << std::endl;
		return false;
	}

	Uint32 window_format = SDL_GetWindowPixelFormat(window);
	convert_surface = SDL_ConvertSurfaceFormat(load_surface, window_format, 0);
	if (!convert_surface)
	{
		std::cout << "ERROR CONVERTING LOAD SURFACE: " << SDL_GetError() << std::endl;
		return false;
	}
	
	load_texture = SDL_CreateTexture(renderer, window_format, SDL_TEXTUREACCESS_STREAMING, convert_surface->w, convert_surface->h);
	if (!load_texture)
	{
		std::cout << "ERROR CREATING BLANK TEXTURE: " << SDL_GetError() << std::endl;
		return false;
	}

	if (SDL_LockTexture(load_texture, NULL, &pixels, &pitch) < 0)
	{
		std::cout << "ERROR LOCKING TEXTURE: " << SDL_GetError() << std::endl;
		return false;
	}

	std::memcpy(pixels, convert_surface->pixels, convert_surface->h * convert_surface->pitch);

	SDL_UnlockTexture(texture);
	pixels = nullptr;
	pitch = 0;

	height = convert_surface->h;
	width = convert_surface->w;

	SDL_FreeSurface(load_surface);
	SDL_FreeSurface(convert_surface);

	texture = load_texture;

	return texture == nullptr;
}

bool Image_Texture::render(SDL_Renderer*& renderer, SDL_Rect* clip, int x, int y, 
	int x_offset, int y_offset, SDL_RendererFlip flip)
{
	SDL_Rect src;
	SDL_Rect dest;

	if (!clip)
	{ 
		dest = { x - x_offset, y - y_offset, width, height };
		src = dest;
	}
	else
	{
		src = *clip;
		dest = { x - x_offset, y - y_offset, src.w, src.h };
	}
		
	if (SDL_RenderCopyEx(renderer, texture, &src, &dest, 0, nullptr, flip) < 0)
	{
		std::cout << "ERROR COPYING IMAGE TEXTURE TO RENDERER: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

bool Image_Texture::lock()
{
	if (pixels)
	{
		//ALREADY LOCKED
		return false;
	}
	else
	{
		if (SDL_LockTexture(texture, NULL, &pixels, &pitch) < 0)
		{
			std::cout << "ERROR LOCKING TEXTURE: " << SDL_GetError() << std::endl;
			return false;
		}
		else return true;
	}
}

bool Image_Texture::unlock()
{
	if (!pixels)
	{
		//ALREADY UNLOCKED
		return false;
	}
	else
	{
		SDL_UnlockTexture(texture);
		pixels = nullptr;
		pitch = 0;
		return true;
	}
}

bool Image_Texture::make_transparent(SDL_Window*& window)
{

	if (lock())
	{
		Uint32 window_format = SDL_GetWindowPixelFormat(window);
		SDL_PixelFormat* pixel_format = SDL_AllocFormat(window_format);

		int pixel_count = (pitch / 4) * height;
		Uint32 transparent_color = SDL_MapRGBA(pixel_format, 0xFF, 0xFF, 0xFF, 0);
		Uint32 key = SDL_MapRGB(pixel_format, color_key.r, color_key.g, color_key.b);
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		for (Uint32* i = (Uint32*)pixels; i != (Uint32*)pixels + pixel_count; ++i)
		{
			if (*i == key)
				*i = transparent_color;
		}
		unlock();
		return true;
	}
	else return false;
}