#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "QuadTree.h"
#include "InputManager.h"


enum LogicResult {
  None          = 0,
  BoundsChanged = 1,
  Remove        = 2,
};

class GameObject : public Collidable {
  public:

    virtual LogicResult logic
      ( double         tick // Milliseconds since last tick
      , InputManager * im
      , QuadTree     * qt
      ) { return None; }

    virtual void render
      ( SDL_Renderer * r
      ) {}

    virtual void onHit () {}

};


#endif // GAMEOBJECT_H
