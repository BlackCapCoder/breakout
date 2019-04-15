#ifndef VIRT_H
#define VIRT_H


// Passing ProxyIX as an argument is free! I checked godbolt
template <unsigned> struct ProxyIX{};
template <class...> struct Pack;


// Why have I isolated virtual methods to a single struct?
// Because I am still anticipating a workaround
template <unsigned N, class R>
struct VIRT
{
  virtual R operator() (const ProxyIX<N>) = 0;
};

template <unsigned N, class R, class...S>
struct VIRT<N, R(S...)>
{
  virtual R operator() (const ProxyIX<N>, S...) = 0;
};


#endif // ifndef VIRT_H
