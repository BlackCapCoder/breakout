#ifndef PADDLE_H
#define PADDLE_H

#include <SDL2/SDL.h>

#include "ColScene.h"
#include "Math.h"
#include "Ball.h"
#include "Rocket.h"


template <class S>
class Paddle : public ColObj<ColScene<S>, ColResult> {
  private:
    bool didReleaseBall = false;
    bool didFire        = false;

  public:
    unsigned char nballs   = 1;
    unsigned int  nrockets = 500;
    V4 bounds{(1000 - 200)/2, 930, 200, 30};

    V4 * getBounds () { return &bounds; }

    void render (SDL_Renderer * r)
    {
      SDL_SetRenderDrawColor (r, 255, 0, 0, 255);
      SDL_RenderFillRect (r, bounds.get());
      if (nballs > 0) {
        SDL_SetRenderDrawColor (r, 255, 255, 255, 255);
        SDL_RenderFillRect (r, V4{bounds.x+bounds.w/2, bounds.y-5, 10, 10}.get());
      }
    }

    ColResult logic (double tick, InputManager * im, ColScene<S> * g)
    {
      float o = bounds.x;
      if (im->isDown(MoveLeft )) bounds.x -= tick * 1.5;
      if (im->isDown(MoveRight)) bounds.x += tick * 1.5;
      if (bounds.x < 0) bounds.x = 0;
      if (bounds.x > g->getWidth() - bounds.w) bounds.x = g->getWidth() - bounds.w;

      if (im->isDown(ReleaseBall) != didReleaseBall) {
        didReleaseBall = !didReleaseBall;
        if (im->isDown(ReleaseBall) && nballs > 0) {
          Ball<S> * ball = new Ball<S>(bounds.x+bounds.w/2, bounds.y-5, (randDouble()-0.5), (randDouble()/2+0.5) * -0.5);
          g->addObject(ball);
          nballs--;
        }
      }

      if (im->isDown(FireRocket) != didFire) {
        didFire = !didFire;
        if (im->isDown(FireRocket) && nrockets > 0) {
          g->addObject(new Rocket<S>(bounds.x+bounds.w/2, bounds.y-5));
          nrockets--;
        }
      }


      if (o != bounds.x) return BoundsChanged;

      return None;
    }
};


#endif // PADDLE_H
