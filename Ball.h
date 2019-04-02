#ifndef BALL_H
#define BALL_H

#include "ColScene.h"
#include "Math.h"
#include <vector>

class Breakout;

class Ball : public GameObject<Breakout, bool>
{
  friend class Breakout;

private:
  double x, y;
  double vx = 0.4, vy = 0.4;
  double radius = 5;
  std::vector<SDL_Point> pts;
  bool isMeteor;
  bool isDead = false;

private:
  V4 getBounds ();

public:
  Ball (double x = 500, double y = 500, double vx = 0.4, double vy = 0.4)
    : x{x}, y{y}, vx{vx}, vy{vy} {};

  void render (SDL_Renderer *);

  bool logic ( double             dt
             , const InputManager &
             , Breakout           * );
};

#endif // BALL_H
