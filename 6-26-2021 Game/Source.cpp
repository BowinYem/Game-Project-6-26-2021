#include "Image_Texture.h"
#include "Character.h"
#include <memory>


namespace 
{
	SDL_Window* game_window = nullptr;
	SDL_Renderer* game_renderer = nullptr;
	Character* test_character = nullptr;

	const int WALK_ANIMATION_SPEED = 30;
	const int WINDOW_HEIGHT = 1080;
	const int WINDOW_WIDTH = 1920;
	const int LEVEL_HEIGHT = 1080;
	const int LEVEL_WIDTH = 7680;
	const int PLAYER_SPAWN_X = 100;
	const int PLAYER_SPAWN_Y = 600;
	const std::string GAME_TITLE = "6-26-2021 PROJECT";
	const std::string BACKGROUND_PATH = "Test_BG.png";

	SDL_Rect camera{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

};

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "ERROR INITIALIZING SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	auto img_flag = IMG_INIT_PNG;
	if (!IMG_Init(IMG_INIT_PNG) & img_flag)
	{
		std::cout << "ERROR INITIALZING IMG SUBSYSTEM: " << SDL_GetError() << std::endl;
		return false;
	}

	game_window = SDL_CreateWindow(GAME_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN);
	if (!game_window)
	{
		std::cout << "ERROR CREATING WINDOW: " << SDL_GetError() << std::endl;
		return false;
	}

	game_renderer = SDL_CreateRenderer(game_window, -1, SDL_RENDERER_ACCELERATED && SDL_RENDERER_PRESENTVSYNC);
	if (!game_renderer)
	{
		std::cout << "ERROR CREATING RENDERER: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SetRenderDrawColor(game_renderer, 255, 255, 255, 255);

	return true;
}

void quit()
{
	delete test_character;
	SDL_DestroyWindow(game_window);
	game_window = nullptr;

	SDL_DestroyRenderer(game_renderer);
	game_renderer = nullptr;

	IMG_Quit();
	SDL_Quit();
}

void move_camera()
{
	camera.x = (test_character->get_x() + test_character->get_width() / 2) - (WINDOW_WIDTH / 2);

	if (camera.x < 0)
	{
		camera.x = 0;
	}

	if (camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
}

int main(int argc, char* argv[])
{
	if (!init())
		return -1;

	bool game_loop = true;
	
	SDL_Event e;

	int current_frame = 0;
	test_character = new Character(game_renderer, game_window, 7);
	test_character->set_spawn(PLAYER_SPAWN_X, PLAYER_SPAWN_Y);

	Image_Texture background;
	background.load(game_renderer, game_window, BACKGROUND_PATH.c_str());	

	while (game_loop)
	{
		SDL_RenderClear(game_renderer);

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				game_loop = false;
			
			test_character->handle_input(e);
		}

		test_character->move();
		move_camera();
		background.render(game_renderer, &camera);
	
		if(test_character->is_walking())
		{
			//Render walking animation
			test_character->render(game_renderer, camera.x, camera.y, current_frame / WALK_ANIMATION_SPEED);
			++current_frame;
			
			if((current_frame / WALK_ANIMATION_SPEED) >= test_character->get_total_frames())
				current_frame = 0;
		}
		else
		{
			//Render neutral position
			test_character->render(game_renderer, camera.x, camera.y);
		}
		
		//test_character->render(game_renderer, camera.x, camera.y);
		SDL_RenderPresent(game_renderer);
	}

	quit();
	return 0;
}

