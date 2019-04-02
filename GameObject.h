#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>

#include "InputManager.h"
#include "ResourceManager.h"


template <class S, class R>
struct GameObject
{
public:
  virtual void init (ResourceManager &, SDL_Renderer *) {}

  virtual void render
    ( SDL_Renderer * r
    ) {}

  virtual R logic
    ( double               dt // Milliseconds since last tick
    , const InputManager & im
    , S                  * st )
  {
    return tick (dt, nullptr, im, st);
  }

  // Combined logic/render tick
  virtual R tick
    ( double               dt
    , SDL_Renderer       * r
    , const InputManager & im
    , S                  * st )
  {
    R ret = logic (dt, im, st);
    render (r);
    return ret;
  }
};

// struct Scene;
// struct Scene : GameObject<void, Scene*> {};
using SceneS = void;
using SceneR = bool;
typedef GameObject<SceneS, SceneR> Scene;


#endif // GAMEOBJECT_H
