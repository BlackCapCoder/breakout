#ifndef BREAKOUT_H
#define BREAKOUT_H

#include <vector>
#include "ColScene.h"
#include "Paddle.h"
#include "Game.h"

class Brick;


class Breakout : public ColScene<Breakout>
{
  friend class Upgrade;
  friend class Brick;
  friend class Paddle;
  friend class Ball;
  friend class GameObject <Breakout, bool>;
  friend class ColObj     <Breakout, ColResult>;


private:
  Paddle             paddle;
  std::vector<Brick> bricks;

  unsigned short     numBricks;
  unsigned short     numRockets   = 5;
  unsigned char      spareBalls   = 3;
  unsigned char      numBalls     = 0;
  double             levelTime    = 0;
  char               speedUprades = 0;
  double             meteorTime   = 0;

  // Time in seconds for the level speed to double
  const double speedDoubleRate = 60;
  const double speedMaxDoubles = 3;
  const double speedMult       = 0.5;
  const double speedUpgradePwr = 1.5;
  const double meteorTimeout   = 30 * 1000;

private:
  void createBricks ();

public:
  Breakout (int w, int h, Game *);

  void tick ( double       dt
            , SDL_Renderer *
            , InputManager *
            , void         * );

  void   onBallLost    ();
  double getLevelSpeed (); // Determines the speed of the balls
  void   meteorUpgrade ();
  bool   meteorActive ();
};


#endif // ifndef BREAKOUT_H
