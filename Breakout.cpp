#include "Breakout.h"
#include "Paddle.h"
#include "Brick.h"
#include "Math.h"
#include "Text.h"

Breakout::Breakout (int w, int h, Game*)
  : ColScene<Breakout>(w, h)
  , hud {42}
{
  addObject(&paddle);
  createBricks();
  addObject(&hud);
}

void Breakout::createBricks()
{
  const int horMarg = 50;
  const int verMarg = 50;
  const int space   = 20;
  const int ncols   = 10;
  const int nrows   = 7;
  const int w       = (getWidth() - 2*horMarg - space*ncols) / ncols;
  const int h       = 30;

  bricks.clear();

  for (int i{}; i < nrows*ncols; i++) {
    int x = i % ncols;
    int y = i / ncols;
    bricks.push_back(Brick{(float) w, h, horMarg + (float)x*(w+space), verMarg + (float)y*(h+space)});
    bricks[i].setColor(i * 80, 90 + i * 50, 255, 255);
  }

  for (int i{}; i < nrows*ncols; i++) {
    addObject(&bricks[i], false);
  }

  numBricks = nrows*ncols;
}

void Breakout::tick
  ( double dt
  , SDL_Renderer * rend
  , InputManager * im
  , void         * ptr
  )
{
  levelTime += dt;

  if (numBricks <= 0) {
    std::cout << "You Win!" << std::endl;
    exit (0);
  } else {
    ColScene<Breakout>::tick(dt, rend, im, ptr);
  }

  // Text t{rend, "hello", SDL_Color{255,255,255,255}, 24, 150, 150};
  // t.render(rend);

  if (im->isDown(PowerMagnet) && magnetCharge > 0) {
    magnetCharge -= dt;
  }
}


void Breakout::onBallLost ()
{
  numBalls--;

  if (numBalls == 0 && spareBalls == 0) {
    std::cout << "Game over!" << std::endl;
    exit (0);
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
    bricks[i].rect.y += 100;
    qt.update(&bricks[i]);
  }
}

