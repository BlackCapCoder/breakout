#ifndef BRICK_H
#define BRICK_H

#include "Breakout.h"


class Brick : public ColObj<Breakout, ColResult>
{
  friend class Breakout;

private:
  V4    rect;
  float r;
  float g;
  float b;
  float a;
  bool  removed;

public:
  Brick();
  Brick ( float x_coord
        , float y_coord
        , float width
        , float height
        , float r
        , float g
        , float b
        , float a
        );

  void      render(SDL_Renderer* r);
  V4 *      getBounds();
  ColResult logic(double, InputManager*, Breakout*);
  void      onHit(Breakout*);
};

#endif // BRICK_H

// Tutor:
// kjetil.raaen... @kristiania.no
