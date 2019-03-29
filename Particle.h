#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL2/SDL.h>
#include <cmath>
#include <vector>

#include "GameObject.h"
#include "Math.h"


template <class S>
class Particle : public GameObject<S, bool> {
  double angle, velocity;
  double r=255,g,b;
  double radius;
  double spin;
  double ttl;
  double ttlOrig;
  double x=100, y=100;

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
    x        += std::sin(angle) * velocity * dt;
    y        += std::cos(angle) * velocity * dt;
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

};

#endif // PARTICLE_H
