#ifndef DUALSCENE_H
#define DUALSCENE_H

#include "GameObject.h"


template <typename ...Args>
struct NScene : public Scene
{
private:

  template <class T> struct Proxy;

  template <typename A, typename ... As>
  struct Product
  {
    A cur;
    Product <As...> rest;

    template <class R>
    inline constexpr R & get (unsigned ix, Proxy<R> * const p) noexcept
    {
      if (ix == 0) return reinterpret_cast<R&>(cur);
      return rest.get (ix-1, p);
    }

    Product<A, As...>
      ( const int w
      , const int h
      , ResourceManager & rm
      , SDL_Renderer    * rend
      )
      : cur  {A{w, h, rm, rend}}
      , rest {Product<As...>{w,h,rm,rend}}
      {}
  };

  template <typename A>
  struct Product<A>
  {
    A cur;

    template <typename R>
    inline constexpr R & get (unsigned, Proxy<R>* const) noexcept
    {
      return reinterpret_cast<R&>(cur);
    }

    Product<A>
      ( const int w
      , const int h
      , ResourceManager & rm
      , SDL_Renderer    * rend
      )
      : cur {A{w, h, rm, rend}}
      {}
  };


private:
  Product<Args...> zip;
  unsigned cursor;

  inline Scene & getActive () noexcept
  {
    return zip.get (cursor % sizeof...(Args), (Proxy<Scene>*) NULL);
  }


public:
  NScene<Args...>
    ( const int w
    , const int h
    , ResourceManager & rm
    , SDL_Renderer    * rend
    )
    : zip    { Product<Args...>{w,h,rm,rend} }
    , cursor { 0 }
  {
  }

  inline
  void render (SDL_Renderer * r)
  {
    getActive () . render (r);
  }

  inline
  SceneR logic
    ( double               dt
    , const InputManager & im
    , SceneS             * st )
  {
    if (getActive () . logic (dt, im, st)) toggle ();
    return false;
  }

  inline
  SceneR tick
    ( double               dt
    , SDL_Renderer       * r
    , const InputManager & im
    , SceneS             * st )
  {
    if (getActive () . tick (dt, r, im, st)) toggle ();
    return false;
  }

  inline
  void toggle ()
  {
    cursor++;
  }
};

#endif
