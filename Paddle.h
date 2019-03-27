#ifndef PADDLE_H
#define PADDLE_H

#include <SDL2/SDL.h>

#include "QuadTree.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Math.h"
#include "Ball.h"
#include "Rocket.h"


class Paddle : public GameObject {
  private:
    bool didReleaseBall = false;
    bool didFire        = false;

  public:
    unsigned char nballs   = 1;
    unsigned int  nrockets = 0;
    V4 bounds{(1000 - 200)/2, 930, 200, 30};

    V4 * getBounds () { return &bounds; }

    LogicResult logic (double tick, Game * g)
    {
      float o = bounds.x;
      if (g->im->isDown(MoveLeft )) bounds.x -= tick * 1.5;
      if (g->im->isDown(MoveRight)) bounds.x += tick * 1.5;
      if (bounds.x < 0) bounds.x = 0;
      if (bounds.x > 1000 - bounds.w) bounds.x = 1000 - bounds.w;

      if (g->im->isDown(ReleaseBall) != didReleaseBall) {
        didReleaseBall = !didReleaseBall;
        if (g->im->isDown(ReleaseBall) && nballs > 0) {
          g->addObject(new Ball(bounds.x+bounds.w/2, bounds.y-5, (randDouble()-0.5), (randDouble()/2+0.5) * -0.5));
          nballs--;
        }
      }

      if (g->im->isDown(FireRocket) != didFire) {
        didFire = !didFire;
        if (g->im->isDown(FireRocket) && nrockets > 0) {
          g->addObject(new Rocket(bounds.x+bounds.w/2, bounds.y-5));
          nrockets--;
        }
      }


      if (o != bounds.x) return BoundsChanged;

      return None;
    }

    void render (SDL_Renderer * r)
    {
      SDL_SetRenderDrawColor (r, 255, 0, 0, 255);
      SDL_RenderFillRect (r, bounds.get());
      if (nballs > 0) {
        SDL_SetRenderDrawColor (r, 255, 255, 255, 255);
        SDL_RenderFillRect (r, V4{bounds.x+bounds.w/2, bounds.y-5, 10, 10}.get());
      }
    }
};

#endif // PADDLE_H
