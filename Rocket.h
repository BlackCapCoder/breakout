#ifndef ROCKET_H
#define ROCKET_H

#include "Math.h"
#include "ColScene.h"
#include "Particle.h"

class Breakout;

// template <class S>
// class Rocket : public GameObject<bool, ColScene<S*>*>
// class Rocket : public GameObject<bool, S*>
class Rocket : public GameObject<bool, Breakout*>
{
private:
  V2 p;
  static constexpr double w     = 10;
  static constexpr double h     = 25;
  static constexpr double speed = 1.5;
  const bool up;

  using S = Breakout;

private:
  V4 getBounds ()
  {
    return V4 { p.x, p.y, w, h };
  }

public:
  Rocket (double x, double y, bool up = true) : p{ V2{x, y}}, up{up} {}

  bool operator()(const ProxyIX<LOGIC>, const LogicArgs<S*> args) override
  {
    double _y = p.y - args.dt() * (up? speed: -speed);
    V4 b { p.x, _y, w, h + p.y - _y };

    ColObj<S*> * c = nullptr;
    for (auto * o : args.st()->getObjectsInBound(b)) {
      b = o->getBounds();
      if (b.y + b.h < _y) continue;
      _y = b.y + b.h;
      c = o;
    }

    Particle<S*>::explosion
      ( V4 { p.x + w/2, _y+h, p.x + w/2, p.y+h }
      , V2 { 0.2, 0.3 }
      , V2 { 0.2, 0.4 }
      , V2 { -0.0003, 0.0003 }
      , V2 { 200, 800 }
      , 0, (int) max (args.dt()/2, 3)
      , [ g=args.st() ] (auto p) { g->insert<1> (p); }
      );

    if (c != nullptr) {
      c->onHit((S *) args.st());
      Particle<S*>::explosion
        ( V4 { p.x + w / 2, _y, p.x + w / 2, _y }
        , V2 { 0.0, 1.0 }
        , V2 { 0.5, 0.9 }
        , V2 { -0.001, 0.001 }
        , V2 { 200, 800 }
        , 40, 100
        , [ g=args.st() ] (auto p) { g->insert<1> (p); }
        );
      args.st()->onExplosion();
      return true;
    }

    p.y = _y;

    return !args.st()->getBounds().intersects(getBounds());
  }

  void operator()(const ProxyIX<RENDER>, const RenderArgs args) override
  {
    SDL_SetRenderDrawColor (&args.rend, 255, 255, 255, 1);
    SDL_RenderFillRect (&args.rend, getBounds() . get ());
  }
};

#endif // ifndef ROCKET_H
