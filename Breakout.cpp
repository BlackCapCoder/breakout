#include <array>

#include "Breakout.h"
#include "Paddle.h"
#include "Brick.h"


Breakout::Breakout (int w, int h, Game*) : ColScene<Breakout>(w, h)
{
  addObject(&paddle);
  createBricks();
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
  if (numBricks <= 0) {
    std::cout << "You Win!" << std::endl;
    exit (0);
  } else {
    ColScene<Breakout>::tick(dt, rend, im, ptr);
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
