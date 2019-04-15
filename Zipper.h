#ifndef ZIPPER_H
#define ZIPPER_H


// A zipper is a container with a cursor that is pointing at one of the elements.
//
// This zipper additinally takes a Pack of virtual methods to override, and any
// calls to these virtual methods will be relayed on to the child that has the focus.
//
// The child is assumed to return a boolean, and if false, the cursor will move on
// to the next child.


#include "Product.h"
#include "Virt.h"


template <class, class> struct ZipperCRTP;
template <class D, unsigned N, class...S>
struct ZipperCRTP<D, VIRT<N, bool(S...)>> : public VIRT<N, bool(S...)>
{
  bool inline constexpr operator () (const ProxyIX<N>, S...s) noexcept override
  {
    if (reinterpret_cast<D*>(this)->template getActive<VIRT<N, bool(S...)>>()(ProxyIX<N>{}, s...))
      reinterpret_cast<D*>(this)->toggle();
    return false;
  }
};


template <class,class...> struct Zipper;
template <class...Base, class...Children>
struct Zipper<Pack<Base...>, Children...>
  : public ZipperCRTP<Zipper<Pack<Base...>, Children...>, Base>...
  , public Product<Children...>
{
private:
  unsigned cursor;

public:
  template <class...Params>
  constexpr Zipper<Pack<Base...>, Children...> (const Params... ps) noexcept
    : Product<Children...>{ ps... }
    , cursor { 0 }
  {
  }

  using ZipperCRTP<Zipper<Pack<Base...>, Children...>, Base>::operator()...;

  template <class T>
  inline constexpr T & getActive () noexcept
  {
    return Product<Children...>::template get<T> (cursor % sizeof...(Children));
  }

  inline void toggle () noexcept { cursor++; }
};


#endif // ifndef ZIPPER_H
