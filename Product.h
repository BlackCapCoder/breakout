#ifndef PRODUCT_H
#define PRODUCT_H

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

  template <class ... Params>
  Product<A, As...> (const Params... ps)
    : cur  {A{ps...}}
    , rest {Product<As...>{ps...}}
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

  template <class ... Params>
  Product<A> (const Params... ps)
    : cur {A{ps...}}
    {}
};

#endif // ifndef PRODUCT_H
