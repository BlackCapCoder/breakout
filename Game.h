#ifndef GAME_H
#define GAME_H


#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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
    Mix_CloseAudio();
  }

  static void initGame ()
  {
    SDL_Init (SDL_INIT_VIDEO | IMG_INIT_PNG | SDL_INIT_AUDIO);
    TTF_Init ();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

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
      // std::cout << std::chrono::duration<double, std::milli> (tick - lastTick) . count () << std::endl;

      lastTick = tick;
    } while (!quit);
  }

};


#endif // ifndef GAME_H
