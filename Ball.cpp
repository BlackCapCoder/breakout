#include "Ball.h"
#include "Particle.h"


V4 Ball::getBounds ()
{
  return V4 {x-radius, y-radius, radius*2, radius*2};
}

void Ball::bounce (V4 b, double _x, double _y, double mult)
{
  V2 c = closestPointPointLine(V2{x,y}, V2{b.x, b.y+b.h/2}, V2{b.x+b.w, b.y+b.h/2});
  int a = ((int) (std::atan2(c.x-x, c.y-y) * (180/M_PI)) + 90) % 360;
  if (a < 0) a += 360;

  if ((a >= 90-45 && a <= 90+45) || (a >= 270-45 && a <= 270+45)) {
    vy = -vy;
  } else {
    vx = -vx;
  }

  x = _x + vx * mult;
  y = _y + vy * mult;
}

void Ball::render (SDL_Renderer * r)
{
  V4 b{x-radius, y-radius, radius*2, radius*2};;
  SDL_SetRenderDrawColor (r, 255, 255, 255, 255);
  SDL_RenderFillRect (r, b.get());
}

bool Ball::logic
  ( double         dt
  , InputManager * im
  , Breakout     * g  )
{
  double _x = x, _y = y;

  x += vx * g->getLevelSpeed() * dt;
  y += vy * g->getLevelSpeed() * dt;

  if (y + radius >= g->getHeight()) {
    g->onBallLost ();
    return true;
  }

  bool magnet = g->hasMagnet() && im->isDown(PowerMagnet);
  V4 b = *g->getBounds();

  if (magnet) {
    V4 p = *g->paddle.getBounds();
    double q = p.x + p.w/2;
    if (x < q) vx += 0.005 * dt;
    if (x > q) vx -= 0.005 * dt;
  }

  if (x <= radius + b.x) {
    x = radius + b.x;
    vx = -vx;
  } else if (x + radius >= b.w + b.x) {
    x = b.w + b.x - radius;
    vx = -vx;
  }

  if (y <= radius + b.y) {
    y = radius + b.y;
    vy = -vy;
  }

  if (g->meteorActive()) {
    double a = std::atan2 (vy, vx) / (2 * M_PI) + 0.5;
    double s = g->getLevelSpeed();
    Particle<Breakout>::explosion
      ( V2 { x, y }
      , V2 { a, a }
      , V2 { s, s }
      , V2 { -0.0007, 0.0007 }
      , V2 { 50, 500 }
      , 1, 3
      , [ &g ] (auto p) { g->addObject (p); }
      );

    V4 b{x-radius, y-radius, radius*2, radius*2};
    if (b.intersects(*g->paddle.getBounds()))
      bounce(*g->paddle.getBounds(), _x, _y, g->getLevelSpeed() * dt);
  }

  auto os = g->getObjectsInBound (V4 {x-radius, y-radius, radius*2, radius*2});
  for (auto obj : os) obj->onHit (g);

  if (!os.empty() && !g->meteorActive())
    bounce (*os[0]->getBounds(), _x, _y, g->getLevelSpeed() * dt);

  return false;
}
