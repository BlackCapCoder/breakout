#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <tuple>

#include "InputManager.h"
#include "ResourceManager.h"


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
    ( double               dt // Milliseconds since last tick
    , const InputManager & im
    , S...                 args
    )
    : _dt { dt }
    , wad { im, args... }
  {}

  inline constexpr auto   dt () const { return _dt; }
  inline constexpr auto & im () const { return std::get<0>(wad); }
  inline constexpr auto   st () const { return std::get<1>(wad); }

  inline constexpr void setDt (double dt) { _dt = dt; }

  template <unsigned Ix>
  inline constexpr auto get () const { return std::get<Ix-1>(wad); }
};

struct RenderArgs
{
  SDL_Renderer & rend;
  bool         & dirty;
};

template <class ... S>
struct TickArgs
{
  const LogicArgs<S...> & l;
  SDL_Renderer & rend;
  bool         & dirty;

  inline constexpr auto   dt () const { return l.dt (); }
  inline constexpr auto & im () const { return l.im (); }
  inline constexpr auto   st () const { return l.st (); }

  inline constexpr const RenderArgs r () const
  {
    return RenderArgs { rend, dirty };
    // return reinterpret_cast<const RenderArgs&>(*(this + sizeof (LogicArgs<S...> &)));
  }
};

template <class R, class ... S>
struct GameObject
{
  virtual void render (const RenderArgs) {}

  virtual R logic (const LogicArgs<S...>)
  {
    return R {};
  }

  // Combined logic/render tick
  virtual R tick (const TickArgs<S...> args)
  {
    R ret = logic (args.l);
    render (args.r());
    return ret;
  }
};


struct InitArgs
{
  const int         w;
  const int         h;
  ResourceManager & rm;
  SDL_Renderer    & rend;
};

using SceneR     = bool;
using Scene      = GameObject<SceneR>;
using LogicArgsS = LogicArgs<>;
using TickArgsS  = TickArgs<>;


#endif // GAMEOBJECT_H
