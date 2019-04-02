#include "Game.h"


Game::Game
  ( const std::string title
  , const int w
  , const int h
  , InputManager & im
  )
  : w  { w  }
  , h  { h  }
  , im { im }
  , wndw { SDL_CreateWindow
      ( title.c_str()
      , SDL_WINDOWPOS_CENTERED
      , SDL_WINDOWPOS_CENTERED
      , w
      , h
      , SDL_WINDOW_SHOWN
      | SDL_WINDOW_OPENGL
      ) }
  , rend { SDL_CreateRenderer
      ( wndw
      , -1
      , SDL_RENDERER_ACCELERATED
      | SDL_RENDERER_PRESENTVSYNC
      ) }
  , rm { ResourceManager{rend} }
  , lastTick { std::chrono::system_clock::now() }
{
}

Game::~Game()
{
  delete s;
  SDL_DestroyRenderer (rend);
  SDL_DestroyWindow   (wndw);
}

bool Game::tick ()
{
  const auto tick  = std::chrono::system_clock::now();
  const auto delta = std::chrono::duration<double, std::milli>(tick - lastTick).count();

  im.tick();
  Scene * newScene = s->tick(delta, rend, im, nullptr);

  if (newScene == nullptr) {
    SDL_RenderPresent (rend);
  } else {
    s = newScene;
    s->init (rm, rend);
  }

  lastTick = tick;

  return !im.isDown(Quit);
}

void Game::loop () { while (tick ()); }
