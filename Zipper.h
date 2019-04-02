#ifndef ZIPPER_H
#define ZIPPER_H

#include <type_traits>
#include "GameObject.h"


namespace Zipper
{
  struct First;

  // N-th
  template <class F, int N, class A, typename ...Args>
  struct Zip
  {
    A current;
    Zip<F, N+1, Args...> rest;

    Zip<F, N, A, Args...>(F & f, const int w, const int h, ResourceManager & rm, SDL_Renderer * rend)
      : current { A{w, h, rm, rend} }
      , rest { Zip<F, N+1, Args...>{f, w, h, rm, rend} }
    {};

    inline void * get (int ix)
    {
      if (ix == N) return &current;
      return rest.get (ix);
    }
  };

  // First
  template <typename A, typename ...Args>
  struct Zip<First, 0, A, Args...>
  {
    using Me  = Zip<First, 0, A, Args...>;

    A current;
    Zip<Me, 1, Args...> rest;

    Zip<First, 0, A, Args...>(const int w, const int h, ResourceManager & rm, SDL_Renderer * rend)
      : current { A{w, h, rm, rend} }
      , rest { Zip<Me, 1, Args...>{(Me&) *this, w, h, rm, rend} }
    {};

    inline void * get (int ix)
    {
      if (ix == 0) return &current;
      return rest.get (ix);
    }
  };

  // Last
  template <class F, int N, class A>
  struct Zip<F, N, A>
  {
    A current;
    const F & rest;

    Zip<F, N, A>(F & f, const int w, const int h, ResourceManager & rm, SDL_Renderer * rend)
      : current { A{w, h, rm, rend} }
      , rest { f }
    {};

    inline void * get (int ix)
    {
      return &current;
    }
  };
}

#endif // ifndef ZIPPER_H
