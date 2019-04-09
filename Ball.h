#ifndef BALL_H
#define BALL_H

#include "ColScene.h"
#include "Math.h"
#include <vector>

class Breakout;

class Ball : public GameObject <bool, Breakout*>
{
  friend class Breakout;

private:
  double x, y;
  double vx = 0.4, vy = 0.4;
  bool isMeteor = false;
  bool isDead   = false;
  std::vector<SDL_Point> pts;

  static constexpr double radius = 5;

private:
  V4 getBounds () const;

public:
  Ball (double x = 500, double y = 500, double vx = 0.4, double vy = 0.4)
    : x{x}, y{y}, vx{vx}, vy{vy} {};

  void render (const RenderArgs) override;
  bool logic  (const LogicArgs<Breakout*>) override;
};

#endif // BALL_H
