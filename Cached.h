#ifndef CACHED_VECTOR_H
#define CACHED_VECTOR_H

#include <functional>
#include <cstdlib>
#include <algorithm>
#include <tuple>


namespace Cache
{
  template <template <class...> class C, class T>
  struct COne
  {
    C <T> elems;
    C <T> queue;

    inline void insert (T x)
    {
      queue.push_back (x);
    }

    inline T * skip (T x)
    {
      elems.push_back (x);
      return &elems.back();
    }

    inline void flush ()
    {
      std::move (queue.begin(), queue.end(), std::back_inserter (elems));
      queue.clear ();
    }

    inline void filter (std::function<bool(T)> f)
    {
      elems.erase (std::remove_if(elems.begin(), elems.end(), f), elems.end());
    }

    inline void clear ()
    {
      queue.clear ();
      elems.clear ();
    }
  };

  template <template <class...> class C, class...Xs>
  struct CMany
  {
    std::tuple<COne<C, Xs>...> vs;

    template <unsigned Ix, class X>
    inline void constexpr insert (X x)
    {
      std::get<Ix>(vs).insert (x);
    }

    template <class X>
    inline void constexpr insert (X x)
    {
      std::get<X>(vs).insert (x);
    }

    template <unsigned Ix, class X>
    inline X * skip (X x)
    {
      return std::get<Ix>(vs).skip (x);
    }

    template <class X>
    inline X * skip (X x)
    {
      return std::get<X>(vs).skip (x);
    }

    inline void constexpr flush ()
    {
      std::apply([](auto& ...x)constexpr{(..., x.flush());}, vs);
    }

    inline void constexpr filter (auto f)
    {
      std::apply([&f](auto& ...x)constexpr{(..., x.filter(f));}, vs);
    }

    inline void constexpr clear ()
    {
      std::apply([](auto& ...x)constexpr{(..., x.clear());}, vs);
    }
  };


  // Use COne when only 1 type
  template <template <class...> class C, class...> struct Select;

  template <template <class...> class C, class A>
  struct Select<C,A> {
    using T = COne<C, A>;
  };

  template <template <class...> class C, class...As>
  struct Select {
    using T = CMany<C, As...>;
  };


  // Replicate arguments N times
  template <class...> struct Pack;

  template <template <class...> class C, class, unsigned, class...> struct Rep;

  template <template <class...> class C, class...A, unsigned N, class...B>
  struct Rep<C,Pack<A...>, N, B...>
  {
    using T = typename Rep<C,Pack<A...,B...>, N-1, B...>::T;
  };

  template <template <class...> class C, class...A, class...B>
  struct Rep<C,Pack<A...>, 1, B...>
  {
    using T = typename Select<C,A...>::T;
  };


  // Accessor
  template <template <class...> class C, unsigned N, class...Ts>
  using F = typename Rep<C,Pack<Ts...>,N,Ts...>::T;
}


template <template <class...> class C, class...Xs>
using Cached = typename Cache::Select<C, Xs...>::T;

template <template <class...> class C, unsigned N, class...Xs>
using CachedN = Cache::F<C,N,Xs...>;



#endif // ifndef CACHED_VECTOR_H
