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
  double r=255,g,b;
  double radius;
  double spin;
  double ttl;
  double ttlOrig;
  double x, y;

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

  bool logic (const LogicArgs<S...> args)
  {
    ttl      -= args.dt();
    angle    += spin * args.dt();
    radius    = 5 * ttl/ttlOrig;
    double v = velocity * std::pow(ttl/ttlOrig, 1.3);
    x        += std::cos(angle) * v * args.dt();
    y        += std::sin(angle) * v * args.dt();

    return ttl <= 0;
  }

  void render (SDL_Renderer & rnd)
  {
    float k = 1 - std::pow ((ttlOrig - ttl) / ttlOrig, 0.3);
    V4 bounds{x-radius, y-radius, radius*2, radius*2};
    SDL_SetRenderDrawColor (&rnd, r-64*k, k*64, k*255, 0);
    SDL_RenderFillRect     (&rnd, bounds.get());
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
    int cnt = rand (min, max);
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
