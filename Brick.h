#ifndef BRICK_H
#define BRICK_H

#include <iostream>
#include <SDL2/SDL.h>

#include "GameObject.h"
#include "QuadTree.h"
#include "Game.h"

class Brick : public GameObject {
  V4    rect;
  float r;
  float g;
  float b;
  float a;
  bool  removed;

public:
  Brick();
  Brick(float width, float height, float x_coord, float y_coord);

public:
  void        setColor(float r, float g, float b, float a);
  void        render(SDL_Renderer* r);
  V4 *        getBounds();
  LogicResult logic(double, Game*);
  void        onHit(Game*);
};

#endif // BRICK_H

// Tutor:
// kjetil.raaen... @kristiania.no
