#ifndef BREAKOUT_H
#define BREAKOUT_H

#include <vector>
#include "ColScene.h"
#include "Paddle.h"
#include "HUD.h"
#include "Ball.h"
#include "Brick.h"

class Brick;

class Breakout : public ColScene<Breakout>
{
  friend class Upgrade;
  friend class Brick;
  friend class Paddle;
  friend class Ball;
  friend class HUD;

private:
  Paddle             paddle;
  HUD                hud;
  std::vector<Brick> bricks;
  Ball               balls[16];

  unsigned short     numBricks;
  unsigned char      ballCounter  = 0;
  unsigned char      currentLevel = 1;
  unsigned short     numRockets   = 5;
  unsigned char      spareBalls   = 3;
  unsigned char      numBalls     = 0;
  double             levelTime    = 0;
  char               speedUprades = 0;
  double             meteorTime   = 0;
  double             magnetCharge = 0;
  unsigned int       points       = 0;
  bool               gameLost     = false;

  static constexpr double speedDoubleRate  = 60;
  static constexpr double speedMaxDoubles  = 3;
  static constexpr double speedMult        = 0.5;
  static constexpr double speedUpgradePwr  = 1.5;
  static constexpr double meteorTimeout    = 10 * 1000;
  static constexpr double magnetUpgrCharge = 5  * 1000;


private:
  void loadLevel (int lvl);
  void onWin     ();
  void onLose    ();

public:
  Breakout
    ( const int w
    , const int h
    , ResourceManager &
    , SDL_Renderer    *
    );

  SceneR tick
    ( double
    , SDL_Renderer*
    , const InputManager&
    , SceneS*
    );

  void   spawnBall     ();
  void   onBallLost    (Ball*);
  double getLevelSpeed (); // Determines the speed of the balls
  void   meteorUpgrade ();
  void   magnetUpgrade ();
  bool   meteorActive  ();
  bool   hasMagnet     ();
  void   shiftDown     ();
  void   doubleBalls   ();
};

#endif // ifndef BREAKOUT_H
