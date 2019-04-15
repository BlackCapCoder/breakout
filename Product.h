#ifndef PRODUCT_H
#define PRODUCT_H


template <class A>
struct ProdElem
{
protected:
  A elem;

  template <class R>
  inline constexpr R & getRef () { return reinterpret_cast<R&>(elem); }

  template <class ... Params>
  constexpr ProdElem<A> (const Params... ps)
    : elem {{ps...}}
  {}
};


// Product<As...> is isomorphic to std::tuple<As...>, but supports
// runtime indexing
template <class...As>
struct Product : ProdElem<As>...
{
  template <class ... Params>
  constexpr Product<As...> (const Params... ps)
    : ProdElem<As>{ps...}...
  {}

  template <typename R>
  inline constexpr R & get (const unsigned i) noexcept
  {
    R * ptrs[sizeof...(As)] = { &ProdElem<As>::template getRef<R>() ... };
    return *ptrs[i];
  }
};


#endif // ifndef PRODUCT_H
