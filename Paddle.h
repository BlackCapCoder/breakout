#ifndef PADDLE_H
#define PADDLE_H

#include <SDL2/SDL.h>

#include "QuadTree.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Math.h"
#include "Ball.h"


class Paddle : public GameObject {
  private:
    V4 bounds{(1000 - 200)/2, 930, 200, 30};
    unsigned char nballs = 1;
    bool didRelease = false;

  public:
    V4 * getBounds () { return &bounds; }

    LogicResult logic (double tick, Game * g)
    {
      float o = bounds.x;
      if (g->im->isDown(MoveLeft )) bounds.x -= tick * 1.5;
      if (g->im->isDown(MoveRight)) bounds.x += tick * 1.5;
      if (bounds.x < 0) bounds.x = 0;
      if (bounds.x > 1000 - bounds.w) bounds.x = 1000 - bounds.w;

      if (g->im->isDown(ReleaseBall) != didRelease) {
        didRelease = !didRelease;
        if (g->im->isDown(ReleaseBall) && nballs > 0) {
          g->addObject(new Ball(bounds.x+bounds.w/2, bounds.y-10, (randDouble()-0.5), (randDouble()/2+0.5) * -0.5));
          nballs--;
        }
      }

      if (o != bounds.x) return BoundsChanged;

      return None;
    }

    void render (SDL_Renderer * r)
    {
      SDL_SetRenderDrawColor (r, 255, 0, 0, 255);
      SDL_RenderFillRect (r, bounds.get());
    }
};

#endif // PADDLE_H
