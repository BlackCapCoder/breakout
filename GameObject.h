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

  constexpr auto   dt () const { return _dt; }
  constexpr auto & im () const { return std::get<0>(wad); }
  constexpr auto   st () const { return std::get<1>(wad); }

  constexpr void setDt (double dt) { _dt = dt; }

  template <unsigned Ix>
  constexpr auto get () const { return std::get<Ix-1>(wad); }
};

template <class ... S>
struct TickArgs
{
  const LogicArgs<S...> & l;
  SDL_Renderer          & r;
  bool                  & dirty;

  constexpr auto   dt () const { return l.dt (); }
  constexpr auto & im () const { return l.im (); }
  constexpr auto   st () const { return l.st (); }
};

template <class R, class ... S>
struct GameObject
{
  virtual void render (SDL_Renderer & r) {}

  virtual R logic (const LogicArgs<S...> args)
  {
    return R {};
  }

  // Combined logic/render tick
  virtual R tick (const TickArgs<S...> args)
  {
    R ret = logic (args.l);
    render (args.r);
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

using SceneR = bool;
typedef GameObject<SceneR> Scene;
typedef LogicArgs<> LogicArgsS;
typedef TickArgs<>  TickArgsS;


#endif // GAMEOBJECT_H
