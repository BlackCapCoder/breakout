#include "Ball.h"


void Ball::render (SDL_Renderer * r)
{
  V4 b{x-radius, y-radius, radius*2, radius*2};;
  SDL_SetRenderDrawColor (r, 255, 255, 255, 255);
  SDL_RenderFillRect (r, b.get());
}

bool Ball::logic
  ( double         dt
  , InputManager *
  , Breakout     * g  )
{
  double _x = x, _y = y;

  x += vx * dt;
  y += vy * dt;
  vx *= 1 + dt * 0.00001;
  vy *= 1 + dt * 0.00001;

  if (y + radius >= 1000) {
    g->onBallLost ();
    return true;
  }

  if (x <= radius) {
    x = radius;
    vx = -vx;
  } else if (x + radius >= 1000) {
    x = 1000 - radius;
    vx = -vx;
  }

  if (y <= radius) {
    y = radius;
    vy = -vy;
  }

  auto os = g->getObjectsInBound (V4 {x-radius, y-radius, radius*2, radius*2});
  if (!os.empty()) {
    for (auto obj : os) obj->onHit (g);
    V4 b = *(os[0]->getBounds());

    V2 c = closestPointPointLine(V2{x,y}, V2{b.x, b.y+b.h/2}, V2{b.x+b.w, b.y+b.h/2});
    int a = ((int) (std::atan2(c.x-x, c.y-y) * (180/M_PI)) + 90) % 360;
    if (a < 0) a += 360;

    if ( (a >= 90-45 && a <= 90+45) || (a >= 270-45 && a <= 270+45)) {
      vy = -vy;
    } else {
      vx = -vx;
    }

    x = _x + vx; y = _y + vy;
  }

  return false;
}
