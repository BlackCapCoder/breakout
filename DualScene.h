#ifndef DUALSCENE_H
#define DUALSCENE_H

#include "GameObject.h"


template <class A, class B>
struct DualScene : public Scene
{
private:
  A a;
  B b;
  bool active;

private:
  inline Scene & getActive () const
  {
    return active ? (Scene&) b : (Scene&) a;
  }

public:
  DualScene<A,B>
    ( const int w
    , const int h
    , ResourceManager & rm
    , SDL_Renderer    * rend
    )
    : a { A (w, h, rm, rend) }
    , b { B (w, h, rm, rend) }
    , active { false }
  {
  }

  void render (SDL_Renderer * r)
  {
    getActive () . render (r);
  }

  SceneR logic
    ( double               dt
    , const InputManager & im
    , SceneS             * st )
  {
    if (getActive() . logic (dt, im, st)) toggle ();
    return false;
  }

  SceneR tick
    ( double               dt
    , SDL_Renderer       * r
    , const InputManager & im
    , SceneS             * st )
  {
    if (getActive() . tick (dt, r, im, st)) toggle ();
    return false;
  }

  void toggle ()
  {
    active = !active;
  }
};

#endif // ifndef DUALSCENE_H
