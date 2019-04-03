#ifndef PADDLE_H
#define PADDLE_H

#include "ColScene.h"

class Breakout;

class Paddle : public ColObj<ColResult, Breakout*>
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

  V4 & getBounds () { return bounds; }

  void render (SDL_Renderer &);
  ColResult logic (const LogicArgs<Breakout*>);

  void reset (const int sw, const int sh);
};


#endif // PADDLE_H
