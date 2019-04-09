#ifndef PADDLE_H
#define PADDLE_H

#include "ColScene.h"

class Breakout;

class Paddle : public ColObj<Breakout*>
{
  friend class Upgrade;

private:
  bool canSpawnBall = false;
  V4 bounds;

  static constexpr double height = 30;

public:
  Paddle
    ( const int sw
    , const int sh
    )
  {
    reset (sw, sh);
  }

  V4 & getBounds () override { return bounds; }

  void render (const RenderArgs) override;
  ColResult logic (const LogicArgs<Breakout*>) override;

  void reset (const int sw, const int sh);
};


#endif // PADDLE_H
