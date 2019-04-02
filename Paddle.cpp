#include "Math.h"
#include "Paddle.h"
#include "Breakout.h"
#include "Ball.h"
#include "Rocket.h"


void Paddle::render (SDL_Renderer * r)
{
  SDL_SetRenderDrawColor (r, 255, 0, 0, 255);
  SDL_RenderFillRect (r, bounds.get());
  if (canSpawnBall) {
    SDL_SetRenderDrawColor (r, 255, 255, 255, 255);
    SDL_RenderFillRect (r, V4{bounds.x+bounds.w/2-5, bounds.y-5, 10, 10}.get());
  }
}

ColResult Paddle::logic (double dt, const InputManager & im, Breakout * g)
{
  float o = bounds.x;
  canSpawnBall = g->spareBalls > 0;

  const double speed = 2.0;

  if (im.isDown(MoveLeft )) bounds.x -= dt * speed;
  if (im.isDown(MoveRight)) bounds.x += dt * speed;
  if (bounds.x < 0) bounds.x = 0;
  if (bounds.x > g->getWidth() - bounds.w) bounds.x = g->getWidth() - bounds.w;

  if (canSpawnBall && im.isDownFirst(ReleaseBall)) {
    g->spawnBall ();
  }

  if (g->numRockets > 0 && im.isDownFirst(FireRocket)) {
    g->addObject(new Rocket<Breakout>(bounds.x+bounds.w/2, bounds.y-26));
    g->numRockets--;
  }

  if (o != bounds.x) return BoundsChanged;

  return None;
}
