#ifndef DUALSCENE_H
#define DUALSCENE_H

#include "GameObject.h"
#include "Zipper.h"

template <typename ...Args>
struct NScene : public Scene
{
private:
  using Z = Zipper::Zip<Zipper::First, 0, Args...>;
  Z zip;
  int cursor;

  inline Scene * getActive () noexcept
  {
    return (Scene*) zip.get (cursor % sizeof...(Args));
  }

public:
  NScene<Args...>
    ( const int w
    , const int h
    , ResourceManager & rm
    , SDL_Renderer    * rend
    )
    : zip { Z{w,h,rm,rend} }
    , cursor{0}
  {
  }

  void render (SDL_Renderer * r)
  {
    getActive () -> render (r);
  }

  SceneR logic
    ( double               dt
    , const InputManager & im
    , SceneS             * st )
  {
    if (getActive() -> logic (dt, im, st)) toggle ();
    return false;
  }

  SceneR tick
    ( double               dt
    , SDL_Renderer       * r
    , const InputManager & im
    , SceneS             * st )
  {
    if (getActive() -> tick (dt, r, im, st)) toggle ();
    return false;
  }

  void toggle ()
  {
    cursor++;
  }
};

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
