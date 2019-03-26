#ifndef PADDLE_H
#define PADDLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "QuadTree.h"
#include "InputManager.h"
#include "GameObject.h"

class Paddle : public GameObject {
  private:
    double x = 0;

  public:
    Rect getBounds () {
      return Rect {x, 900, 100, 30};
    }

    void logic (int tick, InputManager * im) {
      if (im->isDown(MoveLeft )) x -= 1;
      if (im->isDown(MoveRight)) x += 1;
    }

    void render (SDL_Renderer * r) {
      Rect b = getBounds();
      auto rct = SDL_Rect { (int) b.x, (int) b.y, (int) b.width, (int) b.height};
      SDL_SetRenderDrawColor (r, 255, 255, 255, 255);
      SDL_RenderFillRect (r, &rct);
    }
};

#endif // PADDLE_H
