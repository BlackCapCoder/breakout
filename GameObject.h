#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <tuple>

#include "InputManager.h"
#include "ResourceManager.h"
#include "Virt.h"


template <class ... S>
struct LogicArgs
{
private:
  double _dt;

  const
  std::tuple
    < const InputManager &
    , S...
    > wad;

public:
  LogicArgs<S...>
    ( double               dt    // Milliseconds since last tick
    , const InputManager & im
    , bool               & dirty // Set to true if you need a redraw
    , S...                 args  // Custom state
    )
    : _dt   { dt }
    , dirty { dirty }
    , wad   { im, args... }
  {}

  bool & dirty;

  inline constexpr auto   dt () const { return _dt; }
  inline constexpr auto & im () const { return std::get<0>(wad); }
  inline constexpr auto   st () const { return std::get<1>(wad); }

  inline constexpr void setDt (const double dt) { _dt = dt; }

  template <unsigned Ix>
  inline constexpr auto get () const { return std::get<Ix-1>(wad); }
};

struct RenderArgs
{
  SDL_Renderer & rend;
  bool         & dirty;
};

template <class ... S>
struct TickArgs : public LogicArgs<S...>, public RenderArgs
{
  TickArgs <S...>
    ( double dt
    , const InputManager & im
    , S...s
    , SDL_Renderer & rend
    , bool & dirty
    ) : LogicArgs<S...> { dt, im, dirty, s... }
      , RenderArgs      { rend, dirty }
  {}

  using RenderArgs::dirty;

  inline constexpr const RenderArgs & r () const
  {
    return (*this);
  }

  inline constexpr const LogicArgs<S...> & l () const
  {
    return (*this);
  }
};


constexpr unsigned RENDER = 0;
constexpr unsigned LOGIC  = 1;

template <class R, class...S>
using HasLogic  = VIRT<LOGIC, R(LogicArgs<S...>)>;
using HasRender = VIRT<RENDER, void(RenderArgs)>;

template <class R, class...S>
struct GameObject : HasRender, HasLogic<R, S...> {};


inline void render (HasRender & r, const RenderArgs args)
{
  r (ProxyIX<RENDER>{}, args);
}

template <class R, class...S>
inline R logic (HasLogic<R, S...> & l, const LogicArgs<S...> args)
{
  return l (ProxyIX<LOGIC>{}, args);
}


// ------------ Scene

struct InitArgs
{
  const int         w;
  const int         h;
  ResourceManager & rm;
  SDL_Renderer    & rend;
};

using SceneR     = bool;
using LogicArgsS = LogicArgs<>;
using TickArgsS  = TickArgs<>;

constexpr unsigned TICK = 2;

using Scene = VIRT <TICK, SceneR (TickArgsS)>;


#include "Zipper.h"

template <class...Ss>
using ZipperS = Zipper<Pack<Scene>, Ss...>;



#endif // GAMEOBJECT_H
