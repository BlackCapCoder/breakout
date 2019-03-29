#ifndef BREAKOUT_H
#define BREAKOUT_H


#include <vector>
#include "ColScene.h"
#include "Paddle.h"

class Brick;

class Breakout : public ColScene<Breakout>
{
  friend class Upgrade;
  friend class Brick;

  private:
    Paddle<Breakout>   paddle;
    std::vector<Brick> bricks;
    unsigned short     numBricks;

    void createBricks ();

  public:
    Breakout (int w, int h);


    void tick
      ( double dt
      , SDL_Renderer * rend
      , InputManager * im
      , void         * ptr
      )
    {
      if (numBricks <= 0) {
        std::cout << "You Won!" << std::endl;
      } else {
        ColScene<Breakout>::tick(dt, rend, im, ptr);
      }
    }
};


#endif // ifndef BREAKOUT_H
