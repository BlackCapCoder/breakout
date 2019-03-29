#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include "InputManager.h"


struct Scene
{
  private:
  public:
    virtual void init (int w, int h) {}

    virtual void tick
      ( double dt
      , SDL_Renderer *
      , InputManager *
      ) = 0;
};

#endif // ifndef SCENE_H
