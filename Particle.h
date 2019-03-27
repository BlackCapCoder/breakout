#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL2/SDL.h>
#include <cmath>
#include <vector>

#include "GameObject.h"
#include "Math.h"


class Particle : public GameObject {
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

  LogicResult logic  (double dt, Game* g);
  void        render (SDL_Renderer* rnd);
};

#endif // PARTICLE_H
