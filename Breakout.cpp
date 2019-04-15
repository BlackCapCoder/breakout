#include "Breakout.h"
#include "Paddle.h"
#include "Brick.h"
#include "Math.h"
#include "Levels.h"
#include "Upgrade.h"
#include "Rocket.h"
#include "HighscoreManager.h"

Breakout::Breakout (InitArgs args)
  : Parent          ( args )
  , paddle          { args.w, args.h, false }
  , hud             { args, 32 }
  , rm              { args.rm  }
  , audioBlockBreak { args.rm }
  , audioShoot      { args.rm }
  , audioExplosion  { args.rm }
  , audioBounce     { args.rm }
  , audioMeteor     { args.rm }
  , audioUpgrade    { args.rm }
{
  loadLevel (currentLevel);
}

void Breakout::loadLevel (const int level)
{
  bricks.clear      ();
  Parent::clear     ();
  Parent::insert<1> (&paddle);
  Parent::insert<1> (&hud);

  numBricks = Levels::construct (level, bricks, getWidth(), getHeight());
  for (int i=0; i < numBricks; i++)
    Parent::insert<0> (&bricks[i]);
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

  disablePong();
  loadLevel (currentLevel);

  if (numBricks > 0) return;
  loadLevel (currentLevel = 1);
}

SceneR Breakout::operator () (const ProxyIX<TICK>, const TickArgsS args) noexcept
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
    paddle.reset (Parent::getWidth(), Parent::getHeight());

    disablePong ();
    loadLevel (currentLevel);
    args.dirty = true;

    return true;
  }

  levelTime += args.dt();

  if (pongTime < levelTime) {
    disablePong ();
  }

  if (numBricks <= 0) {
    onWin ();
  } else {
    if (numBalls > 0) args.dirty = true;

    Parent::tickChildren ({args.dt(), args.im(), this, args.rend, args.dirty}, true);

    if (args.dirty) {
      SDL_SetRenderDrawColor (&args.rend, 0, 0, 0, 255);
      SDL_RenderClear        (&args.rend);
      Parent::tickChildren ({args.dt(), args.im(), this, args.rend, args.dirty}, false);
    }
  }

  if (args.im().isDown(PowerMagnet) && magnetCharge > 0) {
    magnetCharge -= args.dt();
  }

  return false;
}

void Breakout::spawnBall (bool up)
{
  if (spareBalls <= 0) return;

  if (up) {
    V4 bounds = pongPaddle->getBounds();

    balls [ballCounter] =
      Ball { bounds.x + bounds.w/2
          , bounds.y + bounds.h + 10
          , (randDouble() - 0.5) * 2
          , 1.0 };

    Parent::insert<1> (balls + ballCounter);
  } else {
    V4 bounds = paddle.getBounds();

    balls [ballCounter] =
      Ball { bounds.x + bounds.w/2
          , bounds.y - 10
          , (randDouble() - 0.5) * 2
          , -1.0 };

    Parent::insert<1> (balls + ballCounter);
  }

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
    Parent::updateQuadtree(&bricks[i]);
  }
}

void Breakout::onLose ()
{
  HighscoreManager::commit (points);
  gameLost = true;
}

void Breakout::doubleBalls ()
{
  for (int i = ballCounter-1; i >= 0 ; i--) {
    Ball b = balls[i];
    if (b.isDead) continue;
    balls [ballCounter] = Ball { b.x, b.y, -b.vx, b.vy };

    Parent::insert<0> (balls + ballCounter);
    ballCounter++;
    numBalls++;
  }
}

void Breakout::onBrickRemoved (const Brick & b)
{
  audioBlockBreak.play ();

  if (std::rand () % upgradeChance == 0) {
    Parent::insert<1> (new Upgrade (rm, b.rect.getCenter()));
  }

  points += 1;
}


void Breakout::spawnRocket (bool up)
{
  if (numRockets <= 0) return;
  audioShoot.play();
  numRockets--;

  if (up) {
    V4 b = paddle.getBounds();
    Parent::insert<1> (new Rocket{b.x+b.w/2, b.y-26, true});
  } else {
    V4 b = pongPaddle->getBounds();
    Parent::insert<1> (new Rocket{b.x+b.w/2, b.y+b.h+1, false});
  }
}

void Breakout::onBounce () const
{
  audioBounce.play();
}

void Breakout::onExplosion () const
{
  audioExplosion.play();
}

void Breakout::meteorSound ()
{
  if (levelTime < meteorAudioTime) return;
  // audioMeteor.play();
  meteorAudioTime = levelTime + 3000;
}

void Breakout::upgradeSound () const
{
  audioUpgrade.play ();
}

void Breakout::pongUpgrade ()
{
  if (isPongActive()) return;
  pongTime = levelTime + pongTimeout;
  pongPaddle = new Paddle {(int)paddle.getBounds().x, (int)paddle.getBounds().w, true};
  Parent::insert<1>(pongPaddle);
}

bool Breakout::isPongActive () const
{
  return pongTime > levelTime;
}

void Breakout::disablePong ()
{
  if (pongPaddle == nullptr) return;
  pongTime = 0;
  pongPaddle->setRemoved ();
  // delete pongPaddle;
  pongPaddle = nullptr;
}
