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
  // template <typename... Args>
  // DualScene<A,B> (Args... args)
  //   : a { A{args...} }
  //   , b { B{args...} }
  //   , active { false }
  //   {}
  DualScene<A,B>
    ( int w
    , int h
    , ResourceManager & rm
    )
    : a { A(w, h, rm) }
    , b { B(w, h, rm) }
    , active { false }
  {
  }

  ~DualScene<A,B>()
  {
    // delete a;
    // delete b;
  }

  void init (ResourceManager & rm, SDL_Renderer * r)
  {
    a.init (rm, r);
    b.init (rm, r);
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
    // return getActive () . logic (dt, im, st);
  }

  SceneR tick
    ( double               dt
    , SDL_Renderer       * r
    , const InputManager & im
    , SceneS             * st )
  {
    if (getActive() . tick (dt, r, im, st)) toggle ();
    return false;
    // return getActive () . tick (dt, r, im, st);
  }

  void toggle ()
  {
    active = !active;
  }

};

#endif // ifndef DUALSCENE_H
