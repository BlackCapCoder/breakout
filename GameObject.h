#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "QuadTree.h"
#include "InputManager.h"

class Game;

enum LogicResult {
  None          = 0,
  BoundsChanged = 1,
  Remove        = 2,
};

class GameObject : public Collidable {
  public:

    virtual LogicResult logic
      ( double  tick // Milliseconds since last tick
      , Game *  g
      ) { return None; }

    virtual void render
      ( SDL_Renderer * r
      ) {}

    virtual void onHit (Game *) {}

};


#endif // GAMEOBJECT_H
