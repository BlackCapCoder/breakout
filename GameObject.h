#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "QuadTree.h"
#include "InputManager.h"


class GameObject {
  public:
    virtual Rect getBounds ()      = 0;
    virtual void logic  (int tick, InputManager * im) = 0;
    virtual void render (SDL_Renderer * r) = 0;
};


class Paddle : public GameObject {
  private:
    double x = 0;

  public:
    Rect getBounds () {
      return Rect {x, 0, 100, 30};
    }

    void logic (int tick, InputManager * im) {
      if (im->isDown(MoveLeft )) x += 10;
      if (im->isDown(MoveRight)) x -= 10;
    }

    void render (SDL_Renderer * r) {
      Rect b = getBounds();
      auto rct = SDL_Rect {b.x, b.y, b.width, b.height};
      SDL_RenderFillRect (r, &rct);
    }
};


#endif // GAMEOBJECT_H
