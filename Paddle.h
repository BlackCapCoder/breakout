#ifndef PADDLE_H
#define PADDLE_H

#include <SDL2/SDL.h>

#include "QuadTree.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Rect.h"


class Paddle : public GameObject {
  private:
    double x = 0;

  public:
    Rect getBounds () {
      return Rect {x, 900, 100, 30};
    }

    void logic (double tick, InputManager * im) {
      if (im->isDown(MoveLeft )) x -= tick * 1.5;
      if (im->isDown(MoveRight)) x += tick * 1.5;
    }

    void render (SDL_Renderer * r) {
      Rect b = getBounds();
      SDL_SetRenderDrawColor (r, 255, 255, 255, 255);
      SDL_RenderFillRect (r, b.get());
    }
};

#endif // PADDLE_H
