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
  ShiftDown,
  DoubleBalls,

  NUM_UPGRADES
};

class Upgrade : public GameObject<Breakout, bool>
{
private:
  V2 p;
  const UpgradeType type;
  static constexpr double size    = 100;
  static constexpr double sizeInc = 100;
  static constexpr double speed   = 0.3;
  SDL_Texture * img = nullptr;

public:
  Upgrade (V2 p)
    : p{p}
    , type{(UpgradeType) (std::rand() % NUM_UPGRADES)}
  {}

  void init (ResourceManager & rm, SDL_Renderer *)
  {
    switch (type) {
      case (SizeUp):
        img = rm.getImage ("resources/sizeup.png");
        break;
      case (SizeDown):
        img = rm.getImage ("resources/sizedown.png");
        break;
      case (SpeedDown):
        img = rm.getImage ("resources/speeddown.png");
        break;
      case (SpeedUp):
        img = rm.getImage ("resources/speedup.png");
        break;
      case (Rocket5):
        img = rm.getImage ("resources/rocket5.png");
        break;
      case (Meteor):
        img = rm.getImage ("resources/meteor.png");
        break;
      case (Magnet):
        img = rm.getImage ("resources/magnet.png");
        break;
      case (ShiftDown):
        img = rm.getImage ("resources/shiftdown.png");
        break;
      case (DoubleBalls):
        img = rm.getImage ("resources/doubleballs.png");
        break;
    }
  }

  bool logic (double dt, const InputManager & im, Breakout * g)
  {
    p.y += dt * speed;
    Paddle * pad = &g->paddle;

    if (pad->bounds.intersects(getBounds())) {
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
        case (ShiftDown):
          g->shiftDown();
          break;
        case (DoubleBalls):
          g->doubleBalls();
          break;
      }
      return true;
    }

    return !g->getBounds().intersects(getBounds());
  }

  V4 getBounds ()
  {
    return V4 { p.x-size/2, p.y-size/2, size, size };
  }

  void render (SDL_Renderer * rend)
  {
    SDL_RenderCopy (rend, img, nullptr, getBounds().get());
  }
};


#endif // ifndef UPGRADE_H
