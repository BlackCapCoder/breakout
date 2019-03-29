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

#include "QuadTree.h"
#include "InputManager.h"
#include "GameObject.h"
#include "CachedVector.h"


class Game {
  private:
    SDL_Window    * wndw = nullptr;
    SDL_Renderer  * rend = nullptr;
    Scene         * s    = nullptr;
    InputManager  * im   = nullptr;
    int fps, w, h;

    std::chrono::system_clock::time_point lastTick;
    bool shouldQuit = false;


    void init ()
    {
      SDL_Init (SDL_INIT_VIDEO);

      if (SDL_CreateWindowAndRenderer
          ( w
          , h
          , SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
          , &wndw
          , &rend
          )
        ) exit (EXIT_FAILURE);

      if (!IMG_Init(IMG_INIT_PNG))
        exit (EXIT_FAILURE);

      std::srand(std::time(nullptr));
    }

  public:
    Game (int w, int h, InputManager * im, int fps = 60)
    {
      this->w  = w;
      this->h  = h;
      this->im = im;
      this->lastTick = std::chrono::system_clock::now();
      this->fps = fps;

      init ();
    }

    bool tick ()
    {
      auto tick  = std::chrono::system_clock::now();
      auto delta = std::chrono::duration<double, std::milli>(tick - lastTick).count();

      if (1000 / fps <= delta) {
        im->tick();

        SDL_SetRenderDrawColor (rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);
        (void) s->tick(delta, rend, im, nullptr);
        SDL_RenderPresent (rend);

        lastTick = tick;
        if (im->isDown(Quit)) shouldQuit = true;
      } else {
        std::this_thread::sleep_for
          ( std::chrono::duration<double, std::milli>
            (1000 / fps - delta)
          );
      }

      return !this->shouldQuit;
    }

    void dispose () {
      im->dispose         ();
      SDL_DestroyRenderer (rend);
      SDL_DestroyWindow   (wndw);
    }

    inline int getWidth  ()   { return w; }
    inline int getHeight ()   { return h; }
    void setScene (Scene * s) { this->s = s; }

    template <class S>
    S * setScene ()
    {
      S * s = new S (w, h);
      setScene(s);
      return s;
    }

};

#endif // GAME_H
