#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL2/SDL.h>
#include <cmath>
#include <vector>

#include "GameObject.h"


class Particle : public GameObject {
  int    ttl = 1000;
  double radius = 2;
  double x=100, y=100;
  double angle, velocity;
  double r=255,g,b;

  public:

    Particle (double x, double y, double a, double v) : x{x}, y{y}, angle{a}, velocity{v} {};

    LogicResult logic (double dt, Game * g)
    {
      x += std::sin(angle) * velocity * dt;
      y += std::cos(angle) * velocity * dt;
      ttl -= dt;

      return ttl <= 0? Remove : None;
    }

    void render (SDL_Renderer * rnd)
    {
      Rect bounds{x-radius, y-radius, radius*2, radius*2};;
      SDL_SetRenderDrawColor (rnd, r, g, b, 0);
      SDL_RenderFillRect     (rnd, bounds.get());
    }
};

#endif // PARTICLE_H
