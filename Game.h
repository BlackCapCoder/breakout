#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <thread>
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <vector>
#include <ctime>

#include "GameObject.h"
#include "ResourceManager.h"


class Game
{
private:
  const int w;
  const int h;
  InputManager    & im;
  SDL_Window      * wndw;
  SDL_Renderer    * rend;
  ResourceManager rm;

  mutable std::chrono::system_clock::time_point lastTick;
  mutable bool shouldQuit = false;
  mutable Scene * s = nullptr;

public:
  Game ( const int w
       , const int h
       , InputManager & im
       )
    : w  { w  }
    , h  { h  }
    , im { im }
    , wndw { SDL_CreateWindow
        ( "Breakout"
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

  ~Game();
  bool tick ();
  void loop ();

  inline void quit () { shouldQuit = true; }

  template <class S>
  void setScene ()
  {
    if (s != nullptr) delete s;
    s = new S {w, h, rm};
    s->init (rm, rend);
  }
};

#endif // GAME_H
