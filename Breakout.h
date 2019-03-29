#ifndef BREAKOUT_H
#define BREAKOUT_H

#include <vector>
#include "ColScene.h"
#include "Paddle.h"

class Brick;


class Breakout : public ColScene<Breakout>
{
  friend class Upgrade;
  friend class Brick;
  friend class Paddle;

private:
  Paddle             paddle;
  std::vector<Brick> bricks;

  unsigned short     numBricks;
  unsigned short     numRockets = 5;
  unsigned char      spareBalls = 3;
  unsigned char      numBalls   = 0;

private:
  void createBricks ();

public:
  Breakout (int w, int h);

  void tick ( double       dt
            , SDL_Renderer *
            , InputManager *
            , void         * );

  void onBallLost ();
};


#endif // ifndef BREAKOUT_H
