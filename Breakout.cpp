#include "Breakout.h"
#include "Paddle.h"
#include "Brick.h"
#include "Math.h"
#include "Levels.h"
#include "Upgrade.h"
#include "Rocket.h"

Breakout::Breakout (InitArgs args)
  : ColScene<Breakout_NumLayers, Breakout*>(args)
  , paddle { args.w, args.h }
  , hud    { args, 32 }
  , rm     { args.rm  }
  , audioBlockBreak { args.rm.getAudio("resources/Selection.wav") }
  , audioShoot { args.rm.getAudio("resources/Shooting.wav") }
{
  loadLevel (currentLevel);
}

void Breakout::loadLevel (int level)
{
  bricks.clear ();
  clear        ();
  insert<1> (&paddle);
  insert<1> (&hud);

  numBricks = Levels::construct (level, bricks, getWidth(), getHeight());
  for (int i=0; i < numBricks; i++) insert<0>(&bricks[i]);
}

void Breakout::onWin ()
{
  spareBalls   += numBalls;
  ballCounter   = 0;
  numBalls      = 0;
  levelTime     = 0;
  speedUprades  = 0;
  meteorTime    = 0;
  currentLevel += 1;

  loadLevel (currentLevel);

  if (numBricks > 0) return;
  loadLevel (currentLevel = 1);
  return;
}

SceneR Breakout::tick (const TickArgsS args)
{
  if (gameLost) {
    gameLost      = false;
    currentLevel  = 1;
    spareBalls    = 3;
    numRockets    = 5;
    ballCounter   = 0;
    numBalls      = 0;
    levelTime     = 0;
    speedUprades  = 0;
    meteorTime    = 0;
    magnetCharge  = 0;
    points        = 0;
    paddle.reset (getWidth(), getHeight());
    loadLevel (currentLevel);

    return true;
  }

  levelTime += args.dt();

  if (numBricks <= 0) {
    onWin ();
  } else {
    SDL_SetRenderDrawColor (&args.r, 0, 0, 0, 255);
    SDL_RenderClear(&args.r);
    ColScene<2, Breakout*>::tickChildren
      ({{args.dt(), args.im(), this}, args.r});
  }

  if (args.im().isDown(PowerMagnet) && magnetCharge > 0) {
    magnetCharge -= args.dt();
  }

  return false;
}

void Breakout::spawnBall ()
{
  V4 bounds = paddle.getBounds();

  balls [ballCounter] =
    Ball { bounds.x + bounds.w/2
         , bounds.y - 10
         , (randDouble() - 0.5) * 2
         , -1.0 };

  insert<0> (balls + ballCounter);
  ballCounter++;
  numBalls++;
  spareBalls--;
}

void Breakout::onBallLost (const Ball & b)
{
  if (&b < balls || &b >= balls + ballCounter) return;
  numBalls--;

  if (numBalls == 0 && spareBalls == 0) onLose ();
}


double Breakout::getLevelSpeed () const
{
  double ds = levelTime / (1000 * speedDoubleRate);
  if (ds > speedMaxDoubles) ds = speedMaxDoubles;
  return std::pow (2.0, ds) * speedMult * std::pow (speedUpgradePwr, -speedUprades);
}

void Breakout::meteorUpgrade ()
{
  meteorTime = levelTime + meteorTimeout;
}

bool Breakout::meteorActive () const
{
  return meteorTime > levelTime;
}

void Breakout::magnetUpgrade ()
{
  magnetCharge += magnetUpgrCharge;
}

bool Breakout::hasMagnet () const
{
  return magnetCharge > 0;
}

void Breakout::shiftDown ()
{
  for (int i = 0; i < bricks.size(); i++) {
    if (bricks[i].removed) continue;
    bricks[i].rect.y += 100;
    updateQuadtree(&bricks[i]);
  }
}

void Breakout::onLose ()
{
  gameLost = true;
}

void Breakout::doubleBalls ()
{
  for (int i = ballCounter-1; i >= 0 ; i--) {
    Ball b = balls[i];
    if (b.isDead) continue;
    balls [ballCounter] = Ball { b.x, b.y, -b.vx, b.vy };

    insert<0> (balls + ballCounter);
    ballCounter++;
    numBalls++;
  }
}

void Breakout::onBrickRemoved (const Brick & b)
{
  audioBlockBreak.play ();

  if (std::rand () % upgradeChance == 0) {
    insert<1> (new Upgrade (rm, b.rect.getCenter()));
  }

  points += 1;
}


void Breakout::spawnRocket ()
{
  audioShoot.play();
  V4 & b = paddle.getBounds();
  insert<1> (new Rocket<Breakout>{b.x+b.w/2 , b.y-26});
  numRockets--;
}

void Breakout::onBounce ()
{
}
