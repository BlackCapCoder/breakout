#ifndef UPGRADE_H
#define UPGRADE_H

#include <cstdlib>
#include <vector>
#include "GameObject.h"
#include "QuadTree.h"
#include "Math.h"
#include "Paddle.h"
#include "Breakout.h"


enum UpgradeType {
  SizeUp,
  SizeDown,
  Rocket5,

  NUM_UPGRADES
};


class Upgrade : public GameObject<Breakout, bool> {
  private:
    V2 p;
    UpgradeType type;
    const double size    = 50;
    const double sizeInc = 50;

  public:
    Upgrade (V2 p) : p{p}
    {
      type = (UpgradeType) (std::rand() % NUM_UPGRADES);
    }

    bool logic (double dt, InputManager * im, Breakout * g)
    {
      p.y += dt * 0.3;

      V4 r{p.x-size/2, p.y-size/2, size, size};

      for (auto o: g->getObjectsInBound(r)) {
        Paddle * pad = dynamic_cast<Paddle*>(o);
        if (pad == nullptr) continue;
        switch (type) {
          case (SizeUp):
            pad->bounds.x -= sizeInc/2;
            pad->bounds.w += sizeInc;
            break;
          case (SizeDown):
            pad->bounds.x += sizeInc/2;
            pad->bounds.w -= sizeInc;
            break;
          case (Rocket5):
            pad->nrockets += 5;
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
        case(Rocket5):
          SDL_SetRenderDrawColor (rend, 0, 0, 255, 1);
          break;
      }

      SDL_RenderFillRect (rend, V4 { p.x-size/2, p.y-size/2, size, size }.get());
    }
};


#endif // ifndef UPGRADE_H
