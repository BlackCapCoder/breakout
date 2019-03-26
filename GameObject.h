#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "QuadTree.h"
#include "InputManager.h"


class GameObject {
  public:
    virtual Rect getBounds ()      = 0;
    virtual void logic  (int tick, InputManager * im) = 0;
    virtual void render (SDL_Renderer * r) = 0;
};


#endif // GAMEOBJECT_H
