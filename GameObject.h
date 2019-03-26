#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "QuadTree.h"
#include "InputManager.h"


class GameObject : public Collidable {
  public:

    virtual bool logic
      ( double         tick // Milliseconds since last tick
      , InputManager * im
      , QuadTree     * qt
      ) = 0;

    virtual void render
      ( SDL_Renderer * r
      ) = 0;

};


#endif // GAMEOBJECT_H
