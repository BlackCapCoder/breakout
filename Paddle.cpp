#include "Math.h"
#include "Paddle.h"
#include "Breakout.h"
#include "Ball.h"
#include "Rocket.h"


void Paddle::render (SDL_Renderer & r)
{
  SDL_SetRenderDrawColor (&r, 255, 0, 0, 255);
  SDL_RenderFillRect (&r, bounds.get());
  if (canSpawnBall) {
    SDL_SetRenderDrawColor (&r, 255, 255, 255, 255);
    SDL_RenderFillRect (&r, V4{bounds.x+bounds.w/2-5, bounds.y-5, 10, 10}.get());
  }
}


ColResult Paddle::logic (const LogicArgs<Breakout*> args)
{
  float o = bounds.x;
  canSpawnBall = args.st()->spareBalls > 0;

  const double speed = 1.7;

  if (args.im().isDown(MoveLeft )) bounds.x -= args.dt() * speed;
  if (args.im().isDown(MoveRight)) bounds.x += args.dt() * speed;
  if (bounds.x < 0) bounds.x = 0;
  if (bounds.x > args.st()->getWidth() - bounds.w) bounds.x = args.st()->getWidth() - bounds.w;

  if (canSpawnBall && args.im().isDownFirst(ReleaseBall)) {
    args.st()->spawnBall ();
  }

  if (args.st()->numRockets > 0 && args.im().isDownFirst(FireRocket)) {
    args.st()->spawnRocket();
  }

  if (o != bounds.x) return BoundsChanged;

  return None;
}

void Paddle::reset (const int sw, const int sh)
{
  const double w = (double) sw / 5;
  const double y = (double) sh - 70;
  bounds = V4 {(sw - w)/2, y, w, height};
}
