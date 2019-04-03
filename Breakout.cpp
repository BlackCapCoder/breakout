#include "Breakout.h"
#include "Paddle.h"
#include "Brick.h"
#include "Math.h"
#include "Levels.h"
#include "Upgrade.h"

Breakout::Breakout (InitArgs args)
  : ColScene<Breakout>(args)
  , paddle { args.w, args.h }
  , hud    { args, 32 }
  , rm     { args.rm  }
{
  loadLevel (currentLevel);
}

void Breakout::loadLevel (int level)
{
  bricks.clear ();
  clear        ();
  addObject    (&paddle);
  addObject    (&hud);

  numBricks = Levels::construct (level, bricks, getWidth(), getHeight());
  for (int i=0; i < numBricks; i++) addObject (&bricks[i], false);
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
    loadLevel(currentLevel);

    return true;
  }

  levelTime += args.dt();

  if (numBricks <= 0) {
    onWin ();
  } else {
    SDL_SetRenderDrawColor (&args.r, 0, 0, 0, 255);
    SDL_RenderClear(&args.r);
    ColScene<Breakout>::tick(args);
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

  addObject (balls + ballCounter, true);
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
    qt.update (&bricks[i]);
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

    addObject (balls + ballCounter, true);
    ballCounter++;
    numBalls++;
  }
}

void Breakout::onBrickRemoved (const Brick & b)
{
  if (std::rand () % upgradeChance == 0) {
    Upgrade * up = new Upgrade (rm, b.rect.getCenter());
    addObject (up, true);
  }

  points += 1;
}
