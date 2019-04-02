#ifndef PADDLE_H
#define PADDLE_H

#include "ColScene.h"

class Breakout;

class Paddle : public ColObj<Breakout, ColResult>
{
  friend class Upgrade;

private:
  bool canSpawnBall = false;
  V4 bounds;

public:
  Paddle
    ( const int    sw
    , const int    sh
    , const double w
    , const double h
    , const double y
    )
    : bounds {V4 {(sw - w)/2, y, w, h}}
  {
  }

  V4 & getBounds () { return bounds; }

  void render (SDL_Renderer *);
  ColResult logic (double dt, const InputManager &, Breakout *);
};


#endif // PADDLE_H
