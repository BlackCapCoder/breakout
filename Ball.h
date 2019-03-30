#ifndef BALL_H
#define BALL_H

#include "Breakout.h"
#include "Math.h"


class Ball : public GameObject<Breakout, bool>
{
private:
  double x, y;
  double vx = 0.4, vy = 0.4;
  double radius = 5;

private:
  V4 getBounds ();
  void bounce (V4, double _x, double _y, double mult);

public:
  Ball (double x = 500, double y = 500, double vx = 0.4, double vy = 0.4)
    : x{x}, y{y}, vx{vx}, vy{vy} {};

  void render (SDL_Renderer *);

  bool logic ( double         dt
             , InputManager *
             , Breakout     * g  );
};

#endif // BALL_H
