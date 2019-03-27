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
    SDL_Window    * wndw = nullptr;
    SDL_Renderer  * rend = nullptr;
    std::vector<GameObject*> objs{};
    int fps;

    std::chrono::system_clock::time_point lastTick;
    bool shouldQuit = false;


    void logic (double tick) {
      for (int i = objs.size()-1; i >= 0; i--) {
        auto obj = objs[i];
        LogicResult r = obj->logic (tick, this);

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
    int w, h;
    QuadTree     * qt = nullptr;
    InputManager * im = nullptr;

    Game (int w, int h, InputManager * im, int fps = 60) {
      this->w  = w;
      this->h  = h;
      this->im = im;
      this->qt = new QuadTree({0, 0, (double) w, (double) h}, 100, 4);
      this->lastTick = std::chrono::system_clock::now();
      this->fps = fps;

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
    }

    bool tick () {
      im->tick();

      auto tick  = std::chrono::system_clock::now();
      auto delta = std::chrono::duration<double, std::milli>(tick - lastTick).count();

      if (1000 / fps <= delta) {
        logic  (delta);
        render ();

        lastTick = tick;

        if (im->isDown(Quit))
          this->shouldQuit = true;
    } else {
      std::this_thread::sleep_for
        ( std::chrono::duration<double, std::milli>
          (1000 / fps - delta)
        );
    }

      return !this->shouldQuit;
    }

    void dispose () {
      this->im->dispose();
      SDL_DestroyWindow (wndw);
    }


    void addObject (GameObject * obj)
    {
      if (obj->getBounds() != nullptr) this->qt->insert(obj);
      this->objs.push_back(obj);
    }

    bool removeObject (GameObject * obj)
    {
      for (int i = objs.size()-1; i >= 0; i--) {
        if (objs[i] != obj) continue;
        objs.erase(objs.begin() + i);
        qt->remove(obj);
        return true;
      }

      return false;
    }

};

#endif // GAME_H
