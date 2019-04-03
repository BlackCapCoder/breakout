#ifndef GAME_H
#define GAME_H


#include <iostream>
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
  const int         w;
  const int         h;
  InputManager    & im;
  SDL_Window      & wndw;
  SDL_Renderer    & rend;
  ResourceManager rm;

public:
  Game
    ( const char * title
    , const int    w
    , const int    h
    , InputManager & im
    )
    : w  { w  }
    , h  { h  }
    , im { im }
    , wndw { *SDL_CreateWindow
        ( title
        , SDL_WINDOWPOS_CENTERED
        , SDL_WINDOWPOS_CENTERED
        , w
        , h
        , SDL_WINDOW_SHOWN
        | SDL_WINDOW_OPENGL
        ) }
    , rend { *SDL_CreateRenderer
        ( &wndw
        , -1
        , SDL_RENDERER_ACCELERATED
        | SDL_RENDERER_PRESENTVSYNC
        ) }
    , rm { ResourceManager {rend} }
  {
  }

  ~Game ()
  {
    SDL_DestroyRenderer (&rend);
    SDL_DestroyWindow   (&wndw);
  }

  static void initGame ()
  {
    SDL_Init (SDL_INIT_VIDEO);
    IMG_Init (IMG_INIT_PNG);
    TTF_Init ();

    std::srand (std::time (nullptr));
  }

  template <class S>
  void loop ()
  {
    LogicArgsS      largs {0, im};
    const TickArgsS targs {largs, rend};

    bool quit     = false;
    auto lastTick = std::chrono::system_clock::now ();
    S s { InitArgs{w, h, rm, rend} };

    do {
      const auto tick = std::chrono::system_clock::now ();
      largs.setDt (std::chrono::duration<double, std::milli> (tick - lastTick) . count ());

      im.tick ();
      quit = s.tick (targs) || im.isDown (Quit);
      SDL_RenderPresent (&rend);

      lastTick = tick;
    } while (!quit);
  }

};


#endif // ifndef GAME_H
