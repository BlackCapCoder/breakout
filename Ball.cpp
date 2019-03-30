#include <set>
#include "Ball.h"
#include "Particle.h"


V4 Ball::getBounds ()
{
  return V4 {x-radius, y-radius, radius*2, radius*2};
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
  if (g->hasMagnet() && im->isDown(PowerMagnet)) {
    V4 p = *g->paddle.getBounds();
    double q = p.x + p.w/2;
    if (x < q) vx += 0.005 * dt;
    if (x > q) vx -= 0.005 * dt;
  }

  V4 screen = *g->getBounds();
  std::set<ColObj<Breakout, ColResult>*> hit;


  // We need to loop through all the collisions that
  // have taken place since last tick
  do {
    double mx = vx * g->getLevelSpeed()
         , my = vy * g->getLevelSpeed();

    double _x  = x + mx * dt
         , _y  = y + my * dt
         , _vx = vx
         , _vy = vy;


    // ---- Screen edges
    if (_y + radius >= g->getHeight()) {
      g->onBallLost ();
      return true;
    }

    if (_x <= radius + screen.x) {
      _x = radius + screen.x;
      _vx = -vx;
    } else if (_x + radius >= screen.w + screen.x) {
      _x = screen.w + screen.x - radius;
      _vx = -vx;
    }
    if (_y <= radius + screen.y) {
      _y = radius + screen.y;
      _vy = -vy;
    }

    // ---- Hit testing
    V4 b = V4 {_x - radius, _y - radius, radius*2, radius*2};
    auto os = g->getObjectsInBound(b);

    if (g->meteorActive()) {
      double a = std::atan2 (my, mx) / (2 * M_PI) + 0.5;
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

      for (auto obj : os) obj->onHit (g);
      os.clear();

      if (b.intersects(*g->paddle.getBounds()))
        os.push_back(&g->paddle);
    }

    // ---- Bouncing
    bool didBounce = false;

    if (!os.empty()) {
      bool hor;
      double low = DBL_MAX;
      ColObj<Breakout, ColResult> * c = nullptr;

      double rx = std::abs(radius / mx);
      double ry = std::abs(radius / my);

      for (auto obj : os) {
        if (hit.find (obj) != hit.end ()) continue;

        b = *obj->getBounds();
        double h = std::abs(((mx > 0 ? b.x : b.x + b.w) - x) / mx) - rx
             , v = std::abs(((my > 0 ? b.y : b.y + b.h) - y) / my) - ry;

        if (h < low && h > 0) { hor = true;  low = h; c = obj; }
        if (v < low && v > 0) { hor = false; low = v; c = obj; }
      }

      if (c != nullptr) {
        if (hor) {
          _vx = -vx;
          _x  = x + low * mx;
          dt -= low * mx;
        } else {
          _vy = -vy;
          _y  = y + low * my;
          dt -= low * my;
        }

        c->onHit   (g);
        hit.insert (c);
        didBounce = true;
      }
    }

    x = _x; y = _y; vx = _vx; vy = _vy;

    if (!didBounce) break;
  } while (dt > 0);

  return false;
}

