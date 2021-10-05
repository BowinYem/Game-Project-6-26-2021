#include "Character.h"

Character::Character(SDL_Renderer*& renderer, SDL_Window*& window, int n) : total_frames(n) 
{
	sprite.load(renderer, window, SPRITE_PATH.c_str());
	sprite.make_transparent(window);
	
	int frame_width = sprite.get_width() / total_frames;
	int frame_height = sprite.get_height();
	
	//load frames container
	for(int i = 0; i < total_frames; ++i)
	{
		frames.push_back(SDL_Rect{ i * frame_width, 0, frame_width, frame_height });
	}
}

bool Character::render(SDL_Renderer*& renderer, int x_offset, int y_offset, int current_frame)
{
	//FIND A WAY SO WE DON'T HAVE TO TYPE CAST WALK_DIR
	if (!sprite.render(renderer, &frames[current_frame], pos_x, pos_y, x_offset, y_offset, (SDL_RendererFlip) walk_dir));
		return false;
}

void Character::move()
{
	pos_x += x_velocity;
	pos_y += y_velocity;

	if (pos_x < 0 || pos_x + sprite.get_width() > LEVEL_WIDTH_C)
	{
		pos_x -= x_velocity;
	}
	if (pos_y < 0 || pos_y + sprite.get_height() > LEVEL_HEIGHT_C)
	{
		pos_y -= y_velocity;
	}
}

void Character::handle_input(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			walking = true;
			walk_dir = WALK_LEFT;
			set_x_velocity(-X_SPEED);
			break;

		case SDLK_RIGHT:
			walking = true;
			walk_dir = WALK_RIGHT;
			set_x_velocity(X_SPEED);
			break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			walking = false;
			current_frame = 0;
			set_x_velocity(X_SPEED);
			break;

		case SDLK_RIGHT:
			walking = false;
			current_frame = 0;
			set_x_velocity(-X_SPEED);
			break;
		}
	}
}

void Character::set_spawn(int x, int y)
{
	pos_x = x;
	pos_y = y;
}