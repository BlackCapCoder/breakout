#ifndef BRICK_H
#define BRICK_H

#include "Breakout.h"


class Brick : public ColObj<ColResult, Breakout*>
{
  friend class Breakout;

private:
  V4 rect;
  const float r;
  const float g;
  const float b;
  const float a;
  bool  removed;

public:
  Brick ( const float x_coord
        , const float y_coord
        , const float width
        , const float height
        , const float r
        , const float g
        , const float b
        , const float a
        );

  void      render    (SDL_Renderer & r);
  V4 &      getBounds ();
  ColResult logic     (const LogicArgs<Breakout*>);
  void      onHit     (Breakout*);
};

#endif // BRICK_H

// Tutor:
// kjetil.raaen... @kristiania.no
