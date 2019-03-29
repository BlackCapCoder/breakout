#ifndef PADDLE_H
#define PADDLE_H

#include <SDL2/SDL.h>

#include "QuadTree.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Math.h"
#include "Ball.h"
#include "Rocket.h"
#include "Breakout.h"


class Paddle : public ColObj<Breakout, ColResult> {
  private:
    bool didReleaseBall = false;
    bool didFire        = false;

  public:
    unsigned char nballs   = 1;
    unsigned int  nrockets = 5;
    V4 bounds{(1000 - 200)/2, 930, 200, 30};

    V4 * getBounds () { return &bounds; }

    ColResult logic (double tick, InputManager * im, Breakout * g)
    {
      float o = bounds.x;
      if (im->isDown(MoveLeft )) bounds.x -= tick * 1.5;
      if (im->isDown(MoveRight)) bounds.x += tick * 1.5;
      if (bounds.x < 0) bounds.x = 0;
      if (bounds.x > g->getWidth() - bounds.w) bounds.x = g->getWidth() - bounds.w;

      if (im->isDown(ReleaseBall) != didReleaseBall) {
        didReleaseBall = !didReleaseBall;
        if (im->isDown(ReleaseBall) && nballs > 0) {
          g->addObject(new Ball(bounds.x+bounds.w/2, bounds.y-5, (randDouble()-0.5), (randDouble()/2+0.5) * -0.5));
          nballs--;
        }
      }

      if (im->isDown(FireRocket) != didFire) {
        didFire = !didFire;
        if (im->isDown(FireRocket) && nrockets > 0) {
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
