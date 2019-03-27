#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL2/SDL.h>
#include <cmath>
#include <vector>

#include "GameObject.h"


class Particle : public GameObject {
  double    _ttl;
  double    ttl;
  double radius;
  double x=100, y=100;
  double angle, velocity;
  double r=255,g,b;
  double spin;

public:
  Particle (double x, double y, double a, double v, double s, int ttl)
    : x{x}
    , y{y}
    , angle{a}
    , velocity{v}
    , spin{s}
    , ttl{ttl}
    , _ttl{ttl}
    {};

  LogicResult logic  (double dt, Game* g);
  void        render (SDL_Renderer* rnd);
};

#endif // PARTICLE_H
