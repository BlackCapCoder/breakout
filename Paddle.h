#ifndef PADDLE_H
#define PADDLE_H

#include <SDL2/SDL.h>

#include "QuadTree.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Rect.h"


class Paddle : public GameObject {
  private:
    Rect bounds{(1000 - 200)/2, 930, 200, 30};

  public:
    Rect * getBounds () {
      return &bounds;
    }

    LogicResult logic (double tick, InputManager * im, QuadTree *) {
      float o = bounds.x;
      if (im->isDown(MoveLeft )) bounds.x -= tick * 1.5;
      if (im->isDown(MoveRight)) bounds.x += tick * 1.5;
      if (bounds.x < 0) bounds.x = 0;
      if (bounds.x > 1000 - bounds.width) bounds.x = 1000 - bounds.width;

      if (o != bounds.x) return BoundsChanged;
    }

    void render (SDL_Renderer * r) {
      SDL_SetRenderDrawColor (r, 255, 0, 0, 255);
      SDL_RenderFillRect (r, bounds.get());
    }
};

#endif // PADDLE_H
