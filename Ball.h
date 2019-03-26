#ifndef BALL_H
#define BALL_H

#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include <vector>

#include "GameObject.h"
#include "QuadTree.h"

class Ball : public GameObject {
  double x = 500, y = 500;
  double angle = 0.5;
  double velocity = 0.3;
  double radius   = 5;
  bool   wasHit   = false;

  public:

    Rect * getBounds () {
      return nullptr;
    }

    bool hitTest (QuadTree * qt) {
      std::vector<Collidable*> os = qt->getObjectsInBound(Rect {x-radius, y-radius, radius*2, radius*2});
      return !os.empty();
    }

    bool logic
      ( double         tick // Milliseconds since last tick
      , InputManager * im
      , QuadTree     * qt
      ) {

        if ( 1000 - x - radius <= 0
          || 1000 - y - radius <= 0
          || x < radius
          || y < radius
          || hitTest(qt)
           ) {
          if (!wasHit) {
            angle += M_PI * 0.5;
            wasHit = true;
          }
        } else {
          wasHit = false;
        }

        x += velocity * tick * std::cos (angle);
        y += velocity * tick * std::sin (angle);

        // return true;
        return false;
      }

    void render
      ( SDL_Renderer * r
      ) {
        Rect b{x-radius, y-radius, radius*2, radius*2};;
        SDL_SetRenderDrawColor (r, 255, 255, 255, 255);
        SDL_RenderFillRect (r, b.get());
      }
};

#endif // BALL_H
