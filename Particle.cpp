#include "Particle.h"


LogicResult Particle::logic (double dt, Game * g)
{
  x += std::sin(angle) * velocity * dt;
  y += std::cos(angle) * velocity * dt;
  ttl -= dt;

  return ttl <= 0? Remove : None;
}

void Particle::render (SDL_Renderer * rnd)
{
  Rect bounds{x-radius, y-radius, radius*2, radius*2};;
  SDL_SetRenderDrawColor (rnd, r, g, b, 0);
  SDL_RenderFillRect     (rnd, bounds.get());
}
