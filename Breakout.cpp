#include "Breakout.h"
#include "Paddle.h"
#include "Brick.h"
#include "Math.h"
#include "Levels.h"

Breakout::Breakout
  ( const int w
  , const int h
  , ResourceManager & rm
  , SDL_Renderer    * rend
  )
  : ColScene<Breakout>(w, h, rm, rend)
  , hud {32}
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
  // magnetCharge  = 0;
  currentLevel += 1;

  loadLevel (currentLevel);

  if (numBricks > 0) return;
  currentLevel = 1;
  loadLevel(currentLevel);
  return;

  std::cout << "You Win!" << std::endl;
  exit (0);
}

SceneR Breakout::tick
  ( double               dt
  , SDL_Renderer       * rend
  , const InputManager & im
  , SceneS             * ptr
  )
{
  levelTime += dt;

  if (numBricks <= 0) {
    onWin ();
  } else {
    SDL_SetRenderDrawColor (rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);
    ColScene<Breakout>::tick(dt, rend, im, ptr);
  }

  if (im.isDown(PowerMagnet) && magnetCharge > 0) {
    magnetCharge -= dt;
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

void Breakout::onBallLost (Ball * b)
{
  if (b < balls || b >= balls + ballCounter) return;
  numBalls--;

  if (numBalls == 0 && spareBalls == 0) {
    onLose ();
  }
}


double Breakout::getLevelSpeed ()
{
  double ds = levelTime / (1000 * speedDoubleRate);
  if (ds > speedMaxDoubles) ds = speedMaxDoubles;
  return std::pow (2.0, ds) * speedMult * std::pow(speedUpgradePwr, -speedUprades);
}

void Breakout::meteorUpgrade ()
{
  meteorTime = levelTime + meteorTimeout;
}

bool Breakout::meteorActive ()
{
  return meteorTime > levelTime;
}

void Breakout::magnetUpgrade ()
{
  magnetCharge += magnetUpgrCharge;
}

bool Breakout::hasMagnet ()
{
  return magnetCharge > 0;
}

void Breakout::shiftDown ()
{
  for (int i = 0; i < bricks.size(); i++) {
    if (bricks[i].removed) continue;
    bricks[i].rect.y += 100;
    qt.update(&bricks[i]);
  }
}

void Breakout::onLose ()
{
  std::cout << "Game over!" << std::endl;
  exit (0);
}
