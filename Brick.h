#ifndef BRICK_H
#define BRICK_H

#include <SDL2/SDL.h>

#include "GameObject.h"
#include "QuadTree.h"

class Brick : public GameObject {
  Rect rect;

  float r, g, b, a;

public:
  Brick(float width, float height, float x_coord, float y_coord);
  void setColor(float r, float g, float b, float a);
  void render(SDL_Renderer* r);

  Rect getBounds()
  {
    return rect;
  }
  void logic
    ( double         tick
    , InputManager * im
    )
  {
  }
};

#endif // BRICK_H

// Tutor:
// kjetil.raaen... @kristiania.no
