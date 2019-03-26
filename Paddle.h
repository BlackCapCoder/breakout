#ifndef PADDLE_H
#define PADDLE_H

#include <SDL2/SDL.h>

#include "QuadTree.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Rect.h"


class Paddle : public GameObject {
  private:
    Rect bounds{0, 900, 100, 30};

  public:
    Rect * getBounds () {
      return &bounds;
    }

    bool logic (double tick, InputManager * im, QuadTree *) {
      bool ret = false;
      if (im->isDown(MoveLeft )) { bounds.x -= tick * 1.5; ret = true; }
      if (im->isDown(MoveRight)) { bounds.x += tick * 1.5; ret = true; }
      return ret;
    }

    void render (SDL_Renderer * r) {
      SDL_SetRenderDrawColor (r, 255, 255, 255, 255);
      SDL_RenderFillRect (r, bounds.get());
    }
};

#endif // PADDLE_H
