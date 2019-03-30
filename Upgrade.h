#ifndef UPGRADE_H
#define UPGRADE_H

#include "GameObject.h"
#include "Math.h"
#include "Paddle.h"
#include "Breakout.h"


enum UpgradeType
{ SizeUp,
  SizeDown,
  SpeedUp,
  SpeedDown,
  Rocket5,
  Meteor,
  Magnet,

  NUM_UPGRADES
};

class Upgrade : public GameObject<Breakout, bool>
{
private:
  V2 p;
  UpgradeType type;
  const double size    = 50;
  const double sizeInc = 50;
  const double speed   = 0.3;

public:
  Upgrade (V2 p) : p{p}
  {
    type = (UpgradeType) (std::rand() % NUM_UPGRADES);
  }

  bool logic (double dt, InputManager * im, Breakout * g)
  {
    p.y += dt * speed;

    V4 r{p.x-size/2, p.y-size/2, size, size};
    Paddle * pad = &g->paddle;

    if (pad->bounds.intersects(r)) {
      switch (type) {
        case (SizeUp):
          pad->bounds.x -= sizeInc/2;
          pad->bounds.w += sizeInc;
          break;
        case (SizeDown):
          pad->bounds.x += sizeInc/2;
          pad->bounds.w -= sizeInc;
          break;
        case (SpeedDown):
          g->speedUprades++;
          break;
        case (SpeedUp):
          g->speedUprades--;
          break;
        case (Rocket5):
          g->numRockets += 5;
          break;
        case (Meteor):
          g->meteorUpgrade();
          break;
        case (Magnet):
          g->magnetUpgrade();
          break;
      }
      return true;
    }

    return !g->getBounds()->contains(r);
  }

  void render (SDL_Renderer * rend)
  {
    switch (type) {
      case(SizeUp):
        SDL_SetRenderDrawColor (rend, 0, 255, 0, 1);
        break;
      case(SizeDown):
        SDL_SetRenderDrawColor (rend, 255, 0, 0, 1);
        break;
      case(SpeedUp):
        SDL_SetRenderDrawColor (rend, 128, 0, 0, 1);
        break;
      case(SpeedDown):
        SDL_SetRenderDrawColor (rend, 0, 128, 0, 1);
        break;
      case(Rocket5):
        SDL_SetRenderDrawColor (rend, 0, 0, 255, 1);
        break;
      case(Meteor):
        SDL_SetRenderDrawColor (rend, 255, 0, 255, 1);
        break;
      case(Magnet):
        SDL_SetRenderDrawColor (rend, 128, 128, 128, 1);
        break;
    }

    SDL_RenderFillRect (rend, V4 { p.x-size/2, p.y-size/2, size, size }.get());
  }
};


#endif // ifndef UPGRADE_H
