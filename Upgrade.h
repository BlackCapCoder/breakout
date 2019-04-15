#ifndef UPGRADE_H
#define UPGRADE_H

#include "GameObject.h"
#include "Math.h"
#include "Paddle.h"
#include "Breakout.h"
#include <stdexcept>



class Upgrade : public GameObject<bool, Breakout*>
{
private:
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
    Pong,

    NUM_UPGRADES
  };

  V2 p;
  const UpgradeType type;
  static constexpr double size    = 100;
  static constexpr double sizeInc = 100;
  static constexpr double speed   = 0.3;
  SDL_Texture * img = nullptr;

private:
  static auto imagePath (UpgradeType t)
  {
    switch (t) {
      case (SizeUp):      return "resources/sizeup.png";
      case (SizeDown):    return "resources/sizedown.png";
      case (SpeedDown):   return "resources/speeddown.png";
      case (SpeedUp):     return "resources/speedup.png";
      case (Rocket5):     return "resources/rocket5.png";
      case (Meteor):      return "resources/meteor.png";
      case (Magnet):      return "resources/magnet.png";
      case (ShiftDown):   return "resources/shiftdown.png";
      case (DoubleBalls): return "resources/doubleballs.png";
      case (Pong):        return "resources/pong.png";
      default: throw std::runtime_error ("" __FILE__ ": The impossible happened!");
    }
  }

public:
  Upgrade (ResourceManager & rm, V2 p)
    : p{p}
    , type{(UpgradeType) (std::rand () % NUM_UPGRADES)}
    , img { rm.getImage (imagePath(type)) }
  {}

  bool operator()(const ProxyIX<LOGIC>, const LogicArgs<Breakout*> args) override
  {
    p.y += args.dt() * speed;
    Paddle * pad = &args.st()->paddle;

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
          args.st()->speedUprades++;
          break;
        case (SpeedUp):
          args.st()->speedUprades--;
          break;
        case (Rocket5):
          args.st()->numRockets += 5;
          break;
        case (Meteor):
          args.st()->meteorUpgrade();
          break;
        case (Magnet):
          args.st()->magnetUpgrade();
          break;
        case (ShiftDown):
          args.st()->shiftDown();
          break;
        case (DoubleBalls):
          args.st()->doubleBalls();
          break;
        case (Pong):
          args.st()->pongUpgrade();
          break;
        default:
          throw std::runtime_error ("" __FILE__ ": The impossible happened!");
      }

      args.st()->upgradeSound();
      return true;
    }

    return !args.st()->getBounds().intersects(getBounds());
  }

  V4 getBounds () const
  {
    return V4 { p.x-size/2, p.y-size/2, size, size };
  }

  void operator()(const ProxyIX<RENDER>, const RenderArgs args) override
  {
    SDL_RenderCopy (&args.rend, img, nullptr, getBounds().get());
  }
};


#endif // ifndef UPGRADE_H
