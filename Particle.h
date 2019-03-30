#ifndef PARTICLE_H
#define PARTICLE_H

#include "GameObject.h"
#include "Math.h"
#include <functional>


template <class S>
class Particle : public GameObject<S, bool>
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
  Particle (double x, double y, double a, double v, double s, double ttl)
    : x{x}
    , y{y}
    , angle{a}
    , velocity{v}
    , spin{s}
    , ttl{ttl}
    , ttlOrig{ttl}
    {};

  bool logic (double dt, InputManager*, S*)
  {
    x        += std::cos(angle) * velocity * dt;
    y        += std::sin(angle) * velocity * dt;
    ttl      -= dt;
    angle    += spin*dt;
    radius    = 5 * ttl/ttlOrig;
    velocity *= (1/dt)*15;

    return ttl <= 0;
  }

  void render (SDL_Renderer* rnd)
  {
    float k = 1 - std::pow((ttlOrig - ttl) / ttlOrig, 0.3);
    V4 bounds{x-radius, y-radius, radius*2, radius*2};
    SDL_SetRenderDrawColor (rnd, r-64*k, k*64, k*255, 0);
    SDL_RenderFillRect     (rnd, bounds.get());
  }

  static void explosion
    ( V2 pos
    , V2 angle
    , V2 vel
    , V2 spin
    , V2 ttl
    , int min, int max
    , std::function<void(Particle<S> *)> f
    )
  {
    int cnt = rand (min, max);
    for (int i = 0; i < cnt; i++)
      f ( new Particle<S>
            { pos.x, pos.y
            , randDouble (angle) * 2.0 * M_PI
            , randDouble (vel)
            , randDouble (spin)  * 2.0 * M_PI
            , randDouble (ttl)
            }
        );
  }
};

#endif // PARTICLE_H
