#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include "Game.h"

Game::Game() {
  isRunning = false; // Start

  std::cout << "game constructor called" << std::endl;
  
}

Game::~Game() {
  std::cout << "game destructor called" << std::endl;
  
}

void Game::Initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr << "Error initialising SDL" << std::endl;
    return;
  }
  
  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);
  
  windowWidth  = 800;
  windowHeight = 600;
  window = SDL_CreateWindow(
			NULL,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			windowWidth,
			windowHeight,
			SDL_WINDOW_BORDERLESS
  );
  
  if (!window) {
    std::cerr << "Error creating SDL window" << std::endl;
    return;
  }
  
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    std::cerr << "Error creating SDL renderer" << std::endl;
    return;
  }
  
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  isRunning = true; // End
}
 
void Game::ProcessInput() {
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent)) {
    switch(sdlEvent.type) {
      case SDL_QUIT:
	isRunning = false;
	break;
      case SDL_KEYDOWN:
	if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
	  isRunning = false;
	}
	break;
    }
  }
}

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup() {
  playerPosition = glm::vec2(10.0, 20.0);
  playerVelocity = glm::vec2(0.5, 0.0);
}

void Game::Update() {
  // NOTE: We put delay in Update(); so if we are too fast we waste some time until we reach targeted MILLISEC_PER_FRAME
  int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
  if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
    SDL_Delay(timeToWait);
  }
  
  millisecsPreviousFrame = SDL_GetTicks();
  
  playerPosition.x += playerVelocity.x;
  playerPosition.y += playerVelocity.y;
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
  SDL_RenderClear(renderer);

  SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  SDL_Rect dstRect = {
    static_cast<int>(playerPosition.x),
    static_cast<int>(playerPosition.y),    
    32,
    32
  };
  
  SDL_RenderCopy(renderer, texture, NULL, &dstRect);
  SDL_DestroyTexture(texture);
  
  SDL_RenderPresent(renderer);
}

void Game::Run() {
  Setup();
  while (isRunning) {
    ProcessInput();
    Update();
    Render();
  }
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
