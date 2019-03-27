#ifndef ROCKET_H
#define ROCKET_H

#include "Math.h"
#include "GameObject.h"


class Rocket : public GameObject {
  private:
    V2 p;
    const double w = 10;
    const double h = 25;
    const double speed = 1.5;

    void explode (Game * g) {
      int n = 10 + std::rand() % 100;
      for (int i = 0; i < n; i++)
        g->addObject(new Particle
          ( p.x
          , p.y
          , randDouble() * 2 * M_PI
          , randDouble()*0.5+0.3
          , (randDouble()-0.5)*0.01
          , rand()%600+200
          ));
    }

  public:
    Rocket (double x, double y) : p{V2{x, y}} {}


    LogicResult logic (double dt, Game * g)
    {
      bool hit = false;

      p.y -= dt * speed;

      for (; rand() % 10 > 5;)
        g->addObject(new Particle
          ( p.x
          , p.y
          , randDouble() * M_PI * 0.8 + 0.2 * M_PI
          , randDouble() * 0.5 + 0.3
          , (randDouble()-0.5) * 0.01
          , rand()%600+200
          ));


      for (auto o : g->qt->getObjectsInBound(V4 { p.x-w/2, p.y-h/2, w, h })) {
        GameObject * obj = static_cast<GameObject*>(o);
        obj->onHit(g);
        hit = true;
      }

      if (hit) {
        explode(g);
        return Remove;
      }

      if (!V4 {0, 0, (double) g->getWidth(), (double) g->getHeight()}.contains(V4 { p.x-w/2, p.y-h/2, w, h })) {
        return Remove;
      }

      return None;
    }

    void render (SDL_Renderer * rend)
    {
      SDL_SetRenderDrawColor (rend, 255, 255, 255, 1);
      SDL_RenderFillRect (rend, V4 { p.x-w/2, p.y-h/2, w, h }.get());
    }

};

#endif // ifndef ROCKET_H
