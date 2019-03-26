#ifndef BRICK_H
#define BRICK_H

#include <iostream>
#include <SDL2/SDL.h>

#include "GameObject.h"
#include "QuadTree.h"

class Brick : public GameObject {
  Rect rect;
  float r, g, b, a;
  bool removed = false;

public:
  Brick();
  Brick(float width, float height, float x_coord, float y_coord);
  void setColor(float r, float g, float b, float a);
  void render(SDL_Renderer* r);

  Rect * getBounds()
  {
    return &rect;
  }

  LogicResult logic (double, Game *) {
    return removed? Remove : None;
  }

  void onHit () {
    removed = true;
  }

};

#endif // BRICK_H

// Tutor:
// kjetil.raaen... @kristiania.no
