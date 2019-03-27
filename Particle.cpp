#include "Particle.h"


LogicResult Particle::logic (double dt, Game * g)
{
  x += std::sin(angle) * velocity * dt;
  y += std::cos(angle) * velocity * dt;
  ttl -= dt;
  angle += spin*dt;
  radius = 5 * ttl/_ttl;
  velocity *= (1/dt)*15;

  return ttl <= 0? Remove : None;
}

void Particle::render (SDL_Renderer * rnd)
{
  float k = 1 - std::pow((_ttl - ttl) / _ttl, 0.3);
  Rect bounds{x-radius, y-radius, radius*2, radius*2};;
  SDL_SetRenderDrawColor (rnd, r-64*k, k*64, k*255, 0);
  SDL_RenderFillRect     (rnd, bounds.get());
}
