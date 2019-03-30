#ifndef PADDLE_H
#define PADDLE_H

#include "ColScene.h"

class Breakout;

class Paddle : public ColObj<Breakout, ColResult>
{
  friend class Upgrade;

private:
  bool canSpawnBall = false;
  V4 bounds{(1000 - 200)/2, 930, 200, 30};

public:
  V4 * getBounds () { return &bounds; }

  void render (SDL_Renderer *);
  ColResult logic (double dt, InputManager *, Breakout *);
};


#endif // PADDLE_H
