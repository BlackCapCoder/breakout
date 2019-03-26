#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <thread>
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <vector>

#include "QuadTree.h"
#include "InputManager.h"
#include "GameObject.h"



class Game {
  private:
    int w, h;
    SDL_Window    * wndw = nullptr;
    SDL_Renderer  * rend = nullptr;
    QuadTree      * qt   = nullptr;
    InputManager  * im   = nullptr;
    std::vector<GameObject*> objs{};

    std::chrono::system_clock::time_point lastTick;
    const int fps = 60;
    bool shouldQuit = false;


    void logic (double tick) {
      for (int i = objs.size()-1; i >= 0; i--) {
        auto obj = objs[i];
        LogicResult r = obj->logic (tick, im, qt);

        if (r & Remove) {
          qt->remove(obj);
          objs.erase(objs.begin() + i);
          continue;
        }

        if (r & BoundsChanged)
          qt->update (obj);
      }
    }

    void render () {
      SDL_SetRenderDrawColor (rend, 0, 0, 0, 255);
      SDL_RenderClear(rend);
      for (auto obj : objs) obj->render (rend);
      SDL_RenderPresent (rend);
    }


  public:
    Game (int w, int h, InputManager * im) {
      this->w  = w;
      this->h  = h;
      this->im = im;
      this->qt = new QuadTree({0, 0, (double) w, (double) h}, 100, 4);
      this->lastTick = std::chrono::system_clock::now();

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

    bool tick () {
      im->Tick();

      auto tick  = std::chrono::system_clock::now();
      auto delta = std::chrono::duration<double, std::milli>(tick - lastTick);

      logic (delta.count());

      if (1000 / fps >= delta.count())
        render ();

      lastTick = tick;

      if (im->isDown(Quit))
        this->shouldQuit = true;

      return !this->shouldQuit;
    }

    void dispose () {
      this->im->dispose();
      SDL_DestroyWindow (wndw);
    }


    void addObject (GameObject * obj) {
      if (obj->getBounds() != nullptr) this->qt->insert(obj);
      this->objs.push_back(obj);
    }

};

#endif // GAME_H
