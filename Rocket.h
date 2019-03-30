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
    bool hit = false;

    p.y -= dt * speed;

    for (ColObj<S, ColResult> * o : g->getObjectsInBound(getBounds())) {
      o->onHit((S *) g);
      hit = true;
    }

    if (hit) {
      Particle<S>::explosion
        ( getBounds () . getCenter ()
        , V2 { 0.0, 1.0 }
        , V2 { 0.5, 0.9 }
        , V2 { -0.001, 0.001 }
        , V2 { 200, 800 }
        , 40, 100
        , [ &g ] (auto p) { g->addObject (p); }
        );
      return true;
    } else {
      Particle<S>::explosion
        ( getBounds () . getCenter ()
        , V2 { 0.2, 0.3 }
        , V2 { 0.2, 0.4 }
        , V2 { -0.0003, 0.0003 }
        , V2 { 200, 800 }
        , 1, 3
        , [ &g ] (auto p) { g->addObject (p); }
        );
    }

    return !g->getBounds()->contains(getBounds());
  }

  void render (SDL_Renderer * rend)
  {
    SDL_SetRenderDrawColor (rend, 255, 255, 255, 1);
    SDL_RenderFillRect (rend, getBounds() . get ());
  }
};

#endif // ifndef ROCKET_H
