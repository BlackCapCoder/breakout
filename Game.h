#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <chrono>

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
  mutable Scene * s = nullptr;

public:
  Game ( const std::string title
       , const int w
       , const int h
       , InputManager & im
       );

  ~Game();

  bool tick ();
  void loop ();


  template <class S>
  void setScene ()
  {
    if (s != nullptr) delete s;
    s = new S {w, h, rm};
    s->init (rm, rend);
  }

  static void init ()
  {
    SDL_Init (SDL_INIT_VIDEO);
    IMG_Init (IMG_INIT_PNG);
    TTF_Init ();

    std::srand (std::time(nullptr));
  }
};

#endif // GAME_H
