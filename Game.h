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

class Game
{
private:
  int w, h;
  InputManager  * im   = nullptr;
  int fps;
  SDL_Window    * wndw = nullptr;
  SDL_Renderer  * rend = nullptr;
  Scene         * s    = nullptr;

  std::chrono::system_clock::time_point lastTick;
  bool shouldQuit = false;

private:
  void init ();

public:
  Game (int w, int h, InputManager * im, int fps = 60)
    : w{w}, h{h}, im{im}, fps{fps}
  {
    lastTick = std::chrono::system_clock::now();
    init ();
  }

  bool tick    ();
  void dispose ();

  inline int getWidth  ()   { return w; }
  inline int getHeight ()   { return h; }
  void setScene (Scene * s) { this->s = s; }

  SDL_Renderer* getRenderer() const;

  template <class S>
  S * setScene ()
  {
    S * s = new S (w, h, this);
    setScene(s);
    return s;
  }
};

#endif // GAME_H
