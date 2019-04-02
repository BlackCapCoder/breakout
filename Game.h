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


template <class S>
class Game
{
private:
  const int w;
  const int h;
  InputManager    & im;
  SDL_Window      * wndw;
  SDL_Renderer    * rend;
  ResourceManager rm;
  S s;

public:
  Game <S>
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
    , s  { S{w,h,rm,rend} }
  {
  }

  ~Game<S> ()
  {
    SDL_DestroyRenderer (rend);
    SDL_DestroyWindow   (wndw);
  }

  void loop ()
  {
    bool quit = false;
    std::chrono::system_clock::time_point lastTick = std::chrono::system_clock::now ();

    do {
      const auto tick  = std::chrono::system_clock::now ();
      const auto delta = std::chrono::duration<double, std::milli> (tick - lastTick) . count ();

      im.tick ();
      quit = s.tick (delta, rend, im, nullptr) || im.isDown (Quit);
      if (!quit) SDL_RenderPresent (rend);

      lastTick = tick;
    } while (!quit);
  }
};

void initGame ()
{
  SDL_Init (SDL_INIT_VIDEO);
  IMG_Init (IMG_INIT_PNG);
  TTF_Init ();

  std::srand (std::time (nullptr));
}


#endif // ifndef GAME_H
