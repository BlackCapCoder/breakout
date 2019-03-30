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
  void explode (ColScene<S> * g)
  {
    int n = 10 + std::rand() % 100;
    for (int i = 0; i < n; i++)
      g->addObject(new Particle<S>
        ( p.x
        , p.y
        , randDouble() * 2 * M_PI
        , randDouble()*0.5+0.3
        , (randDouble()-0.5)*0.01
        , rand()%600+200
        ), true);
  }

public:
  Rocket (double x, double y) : p{V2{x, y}} {}

  bool logic (double dt, InputManager * im, ColScene<S> * g)
  {
    p.y -= dt * speed;

    for (; rand() % 10 > 5;)
      g->addObject(new Particle<S>
        ( p.x
        , p.y
        , randDouble() * M_PI * 0.8 + 0.2 * M_PI
        , randDouble() * 0.5 + 0.3
        , (randDouble()-0.5) * 0.01
        , rand()%600+200
        ), true);

    V4 r {p.x-w/2, p.y-h/2, w, h};
    bool hit = false;

    for (ColObj<S, ColResult> * o : g->getObjectsInBound(r)) {
      o->onHit((S *) g);
      hit = true;
    }

    if (hit) {
      explode(g);
      return true;
    }

    return !g->getBounds()->contains(r);
  }

  void render (SDL_Renderer * rend)
  {
    SDL_SetRenderDrawColor (rend, 255, 255, 255, 1);
    SDL_RenderFillRect (rend, V4 { p.x-w/2, p.y-h/2, w, h }.get());
  }
};

#endif // ifndef ROCKET_H
