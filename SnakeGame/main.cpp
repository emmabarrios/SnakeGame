#include <SDL.H>
#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>

int main(int argc, char* argv[]) {

	const int FPS = 15;
	const int MILLISECS_PER_FRAME = 1000 / FPS;
	int millisecsPreviousFrame = 0;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 1, 0);
	SDL_Event sdlEvent;

	enum Direction {
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	bool isRunning = true;
	Direction dir = UP;

	SDL_Rect head{ 500, 500, 10, 10 };
	int size = 1;

	// Snake body container
	std::deque<SDL_Rect> rq;

	// Apple container
	std::vector<SDL_Rect> apples;


	for (int i = 0; i < 100; i++) {
		SDL_Rect apple{ rand() % 100 * 10, rand() % 100 * 10, 10, 10 };
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
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) { isRunning = false; }
				if (sdlEvent.key.keysym.sym == SDLK_UP) { dir = UP; }
				if (sdlEvent.key.keysym.sym == SDLK_DOWN) { dir = DOWN; }
				if (sdlEvent.key.keysym.sym == SDLK_LEFT) { dir = LEFT; }
				if (sdlEvent.key.keysym.sym == SDLK_RIGHT) { dir = RIGHT; }
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

		// Collision detection with apple
		std::for_each(apples.begin(), apples.end(), [&](auto& apple) {
			if (head.x == apple.x && head.y == apple.y) {
				//size += 10;  change from 10 to 1
				size += 1;
				apple.x = -10;
				apple.y = -10;
			}
			});

		// Collision detection with snake body
		std::for_each(rq.begin(), rq.end(), [&](auto& snake_segment) {
			if (head.x == snake_segment.x && head.y == snake_segment.y) {
				size = 1;
			}
			});

		// Add newest head to the snake
		rq.push_front(head);
		while (rq.size() > size) {
			rq.pop_back();
		}

		// Clear Window
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);


		// Draw Body
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		std::for_each(rq.begin(), rq.end(), [&](auto& snake_segment) {
			SDL_RenderFillRect(renderer, &snake_segment);
			});

		// Draw apples
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
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

