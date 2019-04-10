#ifndef PADDLE_H
#define PADDLE_H

#include "ColScene.h"

class Breakout;

class Paddle : public ColObj<Breakout*>
{
  friend class Upgrade;

private:
  bool canSpawnBall = false;
  bool remove = false;
  const bool top;
  V4 bounds;

  static constexpr double height = 30;

public:
  Paddle
    ( const int sw
    , const int sh
    , const bool top
    ) : top {top}
  {
    if (top) {
      const int x = sw;
      const int w = sh;
      bounds = V4 {(double) x, 20, (double) w, height};
    } else
      reset (sw, sh);
  }


  V4 & getBounds () override { return bounds; }

  void render (const RenderArgs) override;
  ColResult logic (const LogicArgs<Breakout*>) override;

  void reset (const int sw, const int sh);

  void setRemoved () { remove = true; }
};


#endif // PADDLE_H
