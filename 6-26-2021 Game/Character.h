#pragma once
#ifndef CHARACTER
#define CHARACTER
#include "Image_Texture.h"
#include <vector>

enum Walk_Direction 
{
	WALK_LEFT = SDL_FLIP_NONE, WALK_RIGHT = SDL_FLIP_HORIZONTAL
};

namespace {
	const int WINDOW_HEIGHT_C = 1080;
	const int WINDOW_WIDTH_C = 1920;
	const int LEVEL_HEIGHT_C = 1080;
	const int LEVEL_WIDTH_C = 7680;
	const int X_SPEED = 5;
	const std::string SPRITE_PATH = "Test.png";
};

class Character
{
public:
	Character(SDL_Renderer*&, SDL_Window*&, int = 0);
	
	bool render(SDL_Renderer*&, int = 0, int = 0, int = 0);
	void move();
	void handle_input(SDL_Event&);
	
	void set_x_velocity(int x) { x_velocity += x; }
	void set_y_velocity(int y) { y_velocity += y; }
	void set_spawn(int, int);
	
	int get_x() { return pos_x; }
	int get_y() { return pos_y; }
	int get_height() { return sprite.get_height(); }
	int get_width() { return sprite.get_width(); }
	int get_total_frames() { return total_frames; }
	bool is_walking() {return walking;}
	
private:
	int x_velocity = 0;
	int y_velocity = 0;
	int pos_x = 0;
	int pos_y = 0;
	int total_frames;
	int current_frame = 0;
	
	bool walking = false;
	Image_Texture sprite;
	std::vector<SDL_Rect> frames; 
	Walk_Direction walk_dir = WALK_RIGHT;
};

#endif
