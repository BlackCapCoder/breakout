#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>

#include "QuadTree.h"


class Game {
  private:
    int w, h;
    SDL_Window   * wndw = nullptr;
    SDL_Renderer * rend = nullptr;
    QuadTree     * qt   = nullptr;
    std::chrono::system_clock::time_point lastTick;
    const int fps = 60;
    bool shouldQuit = false;

  public:
    Game (int w, int h) {
      this->w = w;
      this->h = h;
      this->lastTick = std::chrono::system_clock::now();
      qt = new QuadTree({0, 0, (double) w, (double) h}, 100, 4);

      SDL_Init (SDL_INIT_VIDEO);

      if (SDL_CreateWindowAndRenderer
          ( w
          , h
          , SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
          , &wndw
          , &rend
          )
        ) exit(EXIT_FAILURE);

      if (!IMG_Init(IMG_INIT_PNG))
        exit(EXIT_FAILURE);
    }


    void logic (int tick) {

    }

    void render () {

    }


    bool tick () {
      auto tick = std::chrono::system_clock::now();
      auto delta = std::chrono::duration<double, std::milli>(tick - lastTick);

      logic (delta.count());

      if (1000 / fps >= delta.count())
        render ();

      lastTick = tick;

      return !this->shouldQuit;
    }

    void dispose () {
      SDL_DestroyWindow (wndw);
    }

};
