#ifndef CACHED_VECTOR_H
#define CACHED_VECTOR_H

#include <vector>
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

    void insert (T x)
    {
      queue.push_back (x);
    }

    void flush ()
    {
      std::move (queue.begin(), queue.end(), std::back_inserter (elems));
      queue.clear ();
    }

    void filter (std::function<bool(T)> f)
    {
      elems.erase (std::remove_if(elems.begin(), elems.end(), f), elems.end());
    }

    void clear ()
    {
      queue.clear ();
      elems.clear ();
    }
  };


  // Wrapper <W,C,  a,b,c> => C <W<a>, W<b>, W<c>>

  template <template <class...> class, template <class...> class, class...>
  struct Wrapper;

  template <template <class...> class W, template <class...> class C, class A>
  struct Wrapper<W, C, A>
  {
    using T = A;
  };

  template <template <class...> class W, template <class...> class C, class...P, class A, class ... B>
  struct Wrapper<W, C, C<P...>, A, B...>
  {
    using T = typename Wrapper<W, C, C<P..., W<A>>, B...>::T;
  };

  template <template <class...> class W, template <class...> class C, class ... Xs>
  using Wrap = typename Wrapper<W, C, C<>, Xs...>::T;


  template <template <class...> class C, class...Xs>
  struct CMany
  {
    Wrap<C, std::tuple, Xs...> vs;

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

    inline void constexpr flush ()
    {
      std::apply([](auto& ...x){(..., x.flush());}, vs);
    }

    inline void constexpr filter (auto f)
    {
      std::apply([&f](auto& ...x){(..., x.filter(f));}, vs);
    }

    inline void constexpr clear ()
    {
      std::apply([](auto& ...x){(..., x.clear());}, vs);
    }
  };


  template <template <class...> class C>
  struct Both
  {
    template <class T>
    using One = COne<C, T>;

    template <class...Xs>
    using Many = CMany<One, Xs...>;
  };


  template <template <class...> class C, class...> struct Select;

  template <template <class...> class C, class A>
  struct Select<C,A> {
    using T = typename Both<C>::template One<A>;
  };

  template <template <class...> class C, class...As>
  struct Select {
    using T = typename Both<C>::template Many<As...>;
  };

}

template <template <class...> class C, class...Xs>
using Cached = typename Cache::Select<C, Xs...>::T;


#endif // ifndef CACHED_VECTOR_H
