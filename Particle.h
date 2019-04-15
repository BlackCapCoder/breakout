#ifndef PARTICLE_H
#define PARTICLE_H

#include "GameObject.h"
#include "Math.h"
#include <functional>


template <class ... S>
class Particle : public GameObject<bool, S...>
{
private:
  double angle, velocity;
  double radius;
  double ttl;
  double x, y;
  const double r=255, g=0, b=0;
  const double spin;
  const double ttlOrig;

public:
  Particle
    ( const double x
    , const double y
    , const double a
    , const double v
    , const double s
    , const double ttl
    )
    : x{x}
    , y{y}
    , angle{a}
    , velocity{v}
    , spin{s}
    , ttl{ttl}
    , ttlOrig{ttl}
    {};

  bool operator()(const ProxyIX<LOGIC>, const LogicArgs<Breakout*> args) override
  {
    ttl   -= args.dt();
    angle += spin * args.dt();
    radius = 5 * ttl/ttlOrig;
    const double v = velocity * std::pow(ttl/ttlOrig, 1.3);

    x += std::cos(angle) * v * args.dt();
    y += std::sin(angle) * v * args.dt();

    return ttl <= 0;
  }

  void operator()(const ProxyIX<RENDER>, const RenderArgs args) override
  {
    const float k = 1 - std::pow ((ttlOrig - ttl) / ttlOrig, 0.3);
    SDL_SetRenderDrawColor (&args.rend, r-64*k, k*64, k*255, 0);
    SDL_RenderFillRect     (&args.rend, V4{x-radius, y-radius, radius*2, radius*2}.get());
  }

  static void explosion
    ( const V4 pos
    , const V2 angle
    , const V2 vel
    , const V2 spin
    , const V2 ttl
    , const int min
    , const int max
    , std::function<void(Particle<S...>*)> f
    )
  {
    const int cnt = rand (min, max);
    for (int i = 0; i < cnt; i++)
      f (new Particle<S...>
            { randDouble (pos.x, pos.w)
            , randDouble (pos.y, pos.h)
            , randDouble (angle) * 2.0 * M_PI
            , randDouble (vel)
            , randDouble (spin)  * 2.0 * M_PI
            , randDouble (ttl)
            }
        );
  }
};

#endif // PARTICLE_H
