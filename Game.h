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
#include <thread>

#include "GameObject.h"
#include "ResourceManager.h"


class Game
{
private:
  const int         w;
  const int         h;
  const bool lazyRedraw;
  InputManager    & im;
  SDL_Window      & wndw;
  SDL_Renderer    & rend;
  ResourceManager rm;

public:
  Game
    ( const char *   title
    , const int      w
    , const int      h
    , InputManager & im
    , const bool     lazy
    )
    : w  { w  }
    , h  { h  }
    , im { im }
    , lazyRedraw { lazy }
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
    bool dirty = false;
    LogicArgsS      largs {0, im};
    const TickArgsS targs {largs, rend, dirty};

    bool quit     = false;
    auto lastTick = std::chrono::system_clock::now ();
    S s { InitArgs{w, h, rm, rend} };

    using Dur = std::chrono::duration<double, std::milli>;

    do {
      const auto tick  = std::chrono::system_clock::now ();
      const auto delta = Dur{tick - lastTick}.count();
      largs.setDt (delta);

      im.tick ();
      quit = s.tick (targs) || im.isDown (Quit);

      if (dirty || !lazyRedraw) {
        SDL_RenderPresent (&rend);
        dirty = false;
      } else
        std::this_thread::sleep_for (Dur {1000/60 - delta});

      lastTick = tick;
    } while (!quit);
  }

};


#endif // ifndef GAME_H
