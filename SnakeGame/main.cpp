#include <SDL.H>
#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>



enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

void handleDirectionChange(SDL_Keycode key, Direction& dir, Direction& previousDir) {
	switch (key) {
	case SDLK_UP:
		if (previousDir != DOWN) { dir = UP; previousDir = dir; }
		break;
	case SDLK_DOWN:
		if (previousDir != UP) { dir = DOWN; previousDir = dir; }
		break;
	case SDLK_LEFT:
		if (previousDir != RIGHT) { dir = LEFT; previousDir = dir; }
		break;
	case SDLK_RIGHT:
		if (previousDir != LEFT) { dir = RIGHT; previousDir = dir; }
		break;
	}
}


int main(int argc, char* argv[]) {
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const int APPLE_SIZE = 10;
	const int NUM_APPLES = 100;
	const int FPS = 15;
	const int MILLISECS_PER_FRAME = 1000 / FPS;
	int millisecsPreviousFrame = 0;

	SDL_Init(SDL_INIT_EVERYTHING);

	//SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
	SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 1, 0);
	SDL_Event sdlEvent;

	bool isRunning = true;

	Direction dir = UP;
	Direction previousDir = dir;

	SDL_Rect head{ 500, 500, 10, 10 };
	int size = 1;

	// Snake body container
	std::deque<SDL_Rect> snakeBody;

	// Apple container
	std::vector<SDL_Rect> apples;


	for (int i = 0; i < NUM_APPLES; i++) {
		SDL_Rect apple{ rand() % (SCREEN_WIDTH / APPLE_SIZE) * APPLE_SIZE,
						rand() % (SCREEN_HEIGHT / APPLE_SIZE) * APPLE_SIZE,
						APPLE_SIZE, APPLE_SIZE};
		apples.push_back(apple);
	}


	while (isRunning) {

		int millisecsCurrentFrame = SDL_GetTicks();
		if (millisecsCurrentFrame - millisecsPreviousFrame < MILLISECS_PER_FRAME) {
			SDL_Delay(MILLISECS_PER_FRAME - (millisecsCurrentFrame - millisecsPreviousFrame));
			continue;
		}
		millisecsPreviousFrame = millisecsCurrentFrame;

		// Handle input
		while (SDL_PollEvent(&sdlEvent)) {
			switch (sdlEvent.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) { 
					isRunning = false; 
				}
				else {
					handleDirectionChange(sdlEvent.key.keysym.sym, dir, previousDir);
				}
				break;
			}
		}

		// Move
		switch (dir) {
		case DOWN:
			head.y += 10;
			break;
		case UP:
			head.y -= 10;
			break;
		case LEFT:
			head.x -= 10;
			break;
		case RIGHT:
			head.x += 10;
			break;
		}

		// Wrap around the screen
		if (head.x < 0) head.x = SCREEN_WIDTH - APPLE_SIZE;
		if (head.x >= SCREEN_WIDTH) head.x = 0;
		if (head.y < 0) head.y = SCREEN_HEIGHT - APPLE_SIZE;
		if (head.y >= SCREEN_HEIGHT) head.y = 0;

		// Collision detection with apple
		std::for_each(apples.begin(), apples.end(), [&](auto& apple) {
			if (head.x == apple.x && head.y == apple.y) {
				size += 1;

				// Move the apple outside the screen
				apple.x = -10;
				apple.y = -10;
			}
			});

		// Collision detection with snake body
		std::for_each(snakeBody.begin(), snakeBody.end(), [&](auto& snake_segment) {
			if (head.x == snake_segment.x && head.y == snake_segment.y) {
				size = 1;
			}
			});

		// Add newest head to the snake
		snakeBody.push_front(head);
		while (snakeBody.size() > size) {
			snakeBody.pop_back();
		}

		// Clear Window
		SDL_SetRenderDrawColor(renderer, 198, 215, 107, 255);
		SDL_RenderClear(renderer);


		// Draw Body
		SDL_SetRenderDrawColor(renderer, 117, 129, 51, 255);
		std::for_each(snakeBody.begin(), snakeBody.end(), [&](auto& snake_segment) {
			SDL_RenderFillRect(renderer, &snake_segment);
			});

		// Draw apples
		SDL_SetRenderDrawColor(renderer, 192, 118, 81, 255);
		std::for_each(apples.begin(), apples.end(), [&](auto& apple) {
			SDL_RenderFillRect(renderer, &apple);
			});

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

