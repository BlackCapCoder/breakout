#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "QuadTree.h"
#include "InputManager.h"
#include "Scene.h"


template <class S, class R>
class GameObject
{
  public:
    virtual R logic
      ( double tick // Milliseconds since last tick
      , InputManager *
      , S *
      ) = 0;

    virtual void render
      ( SDL_Renderer * r
      ) {}
};


#endif // GAMEOBJECT_H
