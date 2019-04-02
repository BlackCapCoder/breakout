#ifndef BRICK_H
#define BRICK_H

#include "Breakout.h"


class Brick : public ColObj<Breakout, ColResult>
{
  friend class Breakout;

private:
  V4    rect;
  const float r;
  const float g;
  const float b;
  const float a;
  const int upgradeChance = 10;
  bool  removed;

public:
  Brick ();
  Brick ( const float x_coord
        , const float y_coord
        , const float width
        , const float height
        , const float r
        , const float g
        , const float b
        , const float a
        );

  void      render    (SDL_Renderer* r);
  V4 &      getBounds ();
  ColResult logic     (double, const InputManager&, Breakout*);
  void      onHit     (Breakout*);
};

#endif // BRICK_H

// Tutor:
// kjetil.raaen... @kristiania.no
