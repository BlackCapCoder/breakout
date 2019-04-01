#ifndef ROCKET_H
#define ROCKET_H

#include "Math.h"
#include "ColScene.h"
#include "Particle.h"


template <class S>
class Rocket : public GameObject<ColScene<S>, bool>
{
private:
  V2 p;
  const double w = 10;
  const double h = 25;
  const double speed = 1.5;

private:
  V4 getBounds ()
  {
    return V4 { p.x, p.y, w, h };
  }

public:
  Rocket (double x, double y) : p{ V2{x, y}} {}

  bool logic (double dt, InputManager * im, ColScene<S> * g)
  {
    double _y = p.y - dt * speed;
    V4 b { p.x, _y, w, h + p.y - _y };

    ColObj<S, ColResult> * c = nullptr;
    for (ColObj<S, ColResult> * o : g->getObjectsInBound(b)) {
      b = *o->getBounds();
      if (b.y + b.h < _y) continue;
      _y = b.y + b.h;
      c = o;
    }

    Particle<S>::explosion
      ( V4 { p.x + w/2, _y+h, p.x + w/2, p.y+h }
      , V2 { 0.2, 0.3 }
      , V2 { 0.2, 0.4 }
      , V2 { -0.0003, 0.0003 }
      , V2 { 200, 800 }
      , 0, (int) max(dt/2, 3)
      , [ &g ] (auto p) { g->addObject (p); }
      );

    if (c != nullptr) {
      c->onHit((S *) g);
      Particle<S>::explosion
        ( V4 { p.x + w / 2, _y, p.x + w / 2, _y }
        , V2 { 0.0, 1.0 }
        , V2 { 0.5, 0.9 }
        , V2 { -0.001, 0.001 }
        , V2 { 200, 800 }
        , 40, 100
        , [ &g ] (auto p) { g->addObject (p); }
        );
      return true;
    }

    p.y = _y;

    return !g->getBounds()->contains(getBounds());
  }

  void render (SDL_Renderer * rend)
  {
    SDL_SetRenderDrawColor (rend, 255, 255, 255, 1);
    SDL_RenderFillRect (rend, getBounds() . get ());
  }
};

#endif // ifndef ROCKET_H
