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

  NUM_UPGRADES
};

class Upgrade : public GameObject<Breakout, bool>
{
private:
  V2 p;
  UpgradeType type;
  const double size    = 100;
  const double sizeInc = 100;
  const double speed   = 0.3;
  SDL_Surface * img = nullptr;
  SDL_Texture * txt = nullptr;

public:
  Upgrade (V2 p) : p{p}
  {
    type = (UpgradeType) (std::rand() % NUM_UPGRADES);

    switch (type) {
      case (SizeUp):
        img = IMG_Load("resources/sizeup.png");
        break;
      case (SizeDown):
        img = IMG_Load("resources/sizedown.png");
        break;
      case (SpeedDown):
        img = IMG_Load("resources/speeddown.png");
        break;
      case (SpeedUp):
        img = IMG_Load("resources/speedup.png");
        break;
      case (Rocket5):
        img = IMG_Load("resources/rocket5.png");
        break;
      case (Meteor):
        img = IMG_Load("resources/meteor.png");
        break;
      case (Magnet):
        img = IMG_Load("resources/magnet.png");
        break;
      case (ShiftDown):
        img = IMG_Load("resources/shiftdown.png");
        break;
    }
  }

  bool logic (double dt, InputManager * im, Breakout * g)
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
      }
      onRemove();
      return true;
    }

    return !g->getBounds()->contains(getBounds());
  }

  V4 getBounds ()
  {
    return V4 { p.x-size/2, p.y-size/2, size, size };
  }

  void onRemove ()
  {
    SDL_FreeSurface(img);
    SDL_DestroyTexture(txt);
  }

  void render (SDL_Renderer * rend)
  {
    if (txt == nullptr) {
      txt = SDL_CreateTextureFromSurface(rend, img);
    }

    SDL_RenderCopy (rend, txt, nullptr, getBounds().get());
  }
};


#endif // ifndef UPGRADE_H
