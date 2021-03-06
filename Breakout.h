#ifndef BREAKOUT_H
#define BREAKOUT_H

#include <vector>
#include "ColScene.h"
#include "Paddle.h"
#include "HUD.h"
#include "Ball.h"
#include "Brick.h"


class Brick;


class Breakout : public ColScene<2, Breakout*>
{
  using Parent = ColScene<2, Breakout*>;

  friend class Upgrade;
  friend class Brick;
  friend class Paddle;
  friend class Ball;
  friend class HUD;

private:
  Paddle             paddle;
  Paddle *           pongPaddle = nullptr;
  HUD                hud;
  std::vector<Brick> bricks;
  Ball               balls[256];

  unsigned short     numBricks;
  unsigned char      ballCounter     = 0;
  unsigned char      currentLevel    = 1;
  unsigned short     numRockets      = 5;
  unsigned char      spareBalls      = 3;
  unsigned char      numBalls        = 0;
  double             levelTime       = 0;
  char               speedUprades    = 0;
  double             meteorTime      = 0;
  double             magnetCharge    = 0;
  unsigned           points          = 0;
  bool               gameLost        = false;
  double             meteorAudioTime = 0;
  double             pongTime        = 0;

  static constexpr double   speedDoubleRate  = 60;
  static constexpr double   speedMaxDoubles  = 3;
  static constexpr double   speedMult        = 0.5;
  static constexpr double   speedUpgradePwr  = 1.5;
  static constexpr double   meteorTimeout    = 10 * 1000;
  static constexpr double   magnetUpgrCharge = 5  * 1000;
  static constexpr unsigned upgradeChance    = 10;
  static constexpr double   pongTimeout      = 40 * 1000;

  ResourceManager & rm;


  const ResourceManager::Sound
    < str ("resources/break.wav")
    > audioBlockBreak;

  const ResourceManager::Sound
    < str ("resources/shoot.wav")
    , str ("resources/shoot2.wav")
    > audioShoot;

  const ResourceManager::Sound
    < str ("resources/Explosion1.wav")
    , str ("resources/Explosion2.wav")
    , str ("resources/Explosion3.wav")
    , str ("resources/Explosion4.wav")
    > audioExplosion;

  const ResourceManager::Sound
    < str ("resources/bounce1.wav")
    , str ("resources/bounce2.wav")
    , str ("resources/bounce3.wav")
    > audioBounce;

  const ResourceManager::Sound
    < str ("resources/meteor.wav")
    > audioMeteor;

  const ResourceManager::Sound
    < str ("resources/upgrade.wav")
    > audioUpgrade;

private:
  void loadLevel (const int lvl);
  void onWin     ();
  void onLose    ();
  void disablePong ();

public:
  Breakout (InitArgs);

  SceneR operator () (const ProxyIX<TICK>, const TickArgsS args) noexcept override;

  void   onBallLost     (const Ball &);
  double getLevelSpeed  () const; // Determines the speed of the balls
  void   meteorUpgrade  ();
  void   magnetUpgrade  ();
  bool   meteorActive   () const;
  bool   hasMagnet      () const;
  void   shiftDown      ();
  void   doubleBalls    ();
  void   onBrickRemoved (const Brick &);
  void   onBounce       () const;
  void   onExplosion    () const;
  void   meteorSound    ();
  void   upgradeSound   () const;

  void spawnBall   (bool up = false);
  void spawnRocket (bool up = true);
  void pongUpgrade ();
  bool isPongActive () const;
};

#endif // ifndef BREAKOUT_H
