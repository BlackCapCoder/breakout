#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>

#include "QuadTree.h"
#include "InputManager.h"


template <class S, class R>
struct GameObject
{
public:
  virtual void render
    ( SDL_Renderer * r
    ) {}

  virtual R logic
    ( double         dt // Milliseconds since last tick
    , InputManager * im
    , S            * st )
  { return tick (dt, nullptr, im, st); }

  // Combined logic/game tick
  virtual R tick
    ( double         dt
    , SDL_Renderer * r
    , InputManager * im
    , S            * st )
  {
    R ret = logic (dt, im, st);
    if (r != nullptr) render (r);
    return ret;
  }
};

// We have to specialize for void, because c++
template <class S>
struct GameObject<S, void>
{
public:
  virtual void render
    ( SDL_Renderer * r
    ) {}

  virtual void logic
    ( double         dt // Milliseconds since last tick
    , InputManager * im
    , S            * st )
  { tick (dt, nullptr, im, st); }

  virtual void tick
    ( double         dt
    , SDL_Renderer * r
    , InputManager * im
    , S            * st )
  {
    logic (dt, im, st);
    if (r != nullptr) render (r);
  }
};


typedef GameObject<void, void> Scene;

#endif // GAMEOBJECT_H
