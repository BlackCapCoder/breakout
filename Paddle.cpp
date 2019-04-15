#include "Math.h"
#include "Paddle.h"
#include "Breakout.h"
#include "Ball.h"
#include "Rocket.h"


void Paddle::operator()(const ProxyIX<RENDER>, const RenderArgs args)
{
  SDL_SetRenderDrawColor (&args.rend, 255, 0, 0, 255);
  SDL_RenderFillRect (&args.rend, bounds.get());
  // if (canSpawnBall) {
  //   SDL_SetRenderDrawColor (&args.rend, 255, 255, 255, 255);
  //   SDL_RenderFillRect (&args.rend, V4{bounds.x+bounds.w/2-5, bounds.y-5, 10, 10}.get());
  // }
}


ColResult Paddle::operator()(const ProxyIX<LOGIC>, const LogicArgs<Breakout*> args)
{
  double o = bounds.x;
  const double speed = 1.7;

  if (args.im().isDown(MoveLeft )) bounds.x -= args.dt() * speed;
  if (args.im().isDown(MoveRight)) bounds.x += args.dt() * speed;
  if (bounds.x < 0) bounds.x = 0;
  if (bounds.x > args.st()->getWidth() - bounds.w) bounds.x = args.st()->getWidth() - bounds.w;

  if (args.im().isDownFirst(ReleaseBall)) {
    args.st()->spawnBall (top);
  }

  if (args.im().isDownFirst(FireRocket)) {
    args.st()->spawnRocket(!top);
  }

  if (o != bounds.x) return BoundsChanged;

  if (remove) return Remove;
  return None;
}

void Paddle::reset (const int sw, const int sh)
{
  const double w = (double) sw / 5;
  const double y = (double) sh - 70;
  bounds = V4 {(sw - w)/2, y, w, height};
}
