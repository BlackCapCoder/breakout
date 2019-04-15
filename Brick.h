#ifndef BRICK_H
#define BRICK_H

#include "Breakout.h"


class Brick : public ColObj<Breakout*>
{
  friend class Breakout;

private:
  V4 rect;
  const float r;
  const float g;
  const float b;
  const float a;
  bool  removed;
  bool  dirty = true;

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

  void      operator()(const ProxyIX<RENDER>, const RenderArgs) override;
  ColResult operator()(const ProxyIX<LOGIC>,  const LogicArgs<Breakout*>) override;
  V4 &      getBounds () override;
  // void      onHit     (Breakout*) override;
  void operator()(const ProxyIX<ONHIT>, Breakout*) override;
};

#endif // BRICK_H

// Tutor:
// kjetil.raaen... @kristiania.no
