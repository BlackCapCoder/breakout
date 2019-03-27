#ifndef BALL_H
#define BALL_H

#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include <cstdlib>
#include <vector>

#include "GameObject.h"
#include "QuadTree.h"
#include "Particle.h"
#include "Math.h"


class Ball : public GameObject {
  double x, y;
  double radius = 5;
  double vx = 0.4, vy = 0.4;

  public:
    Ball (double x = 500, double y = 500, double vx = 0.4, double vy = 0.4)
      : x{x}, y{y}, vx{vx}, vy{vy} {};

    LogicResult logic
      ( double dt
      , Game * g
      ) {

        double _x = x, _y = y;

        x += vx * dt;
        y += vy * dt;
        vx *= 1 + dt * 0.00001;
        vy *= 1 + dt * 0.00001;

        if (x <= radius) {
          x = radius;
          vx = -vx;
        } else if (x + radius >= 1000) {
          x = 1000 - radius;
          vx = -vx;
        }

        if (y + radius >= 1000) {
          // y = 1000 - radius;
          // vy = -vy;
          std::cout << "LOOOSER!" << std::endl;
          exit(0);
        }

        if (y <= radius) {
          y = radius;
          vy = -vy;
        }

        std::vector<Collidable*> os = g->qt->getObjectsInBound(V4 {x-radius, y-radius, radius*2, radius*2});
        if (!os.empty()) {
          for (auto obj : os) static_cast<GameObject*>(obj)->onHit(g);
          GameObject * obj = static_cast<GameObject*>(os[0]);
          V4 b = *(obj->getBounds());

          V2 c = closestPointPointLine(V2{x,y}, V2{b.x, b.y+b.h/2}, V2{b.x+b.w, b.y+b.h/2});
          int a = ((int) (std::atan2(c.x-x, c.y-y) * (180/M_PI)) + 90) % 360;
          if (a < 0) a += 360;

          if ( (a >= 90-45 && a <= 90+45) || (a >= 270-45 && a <= 270+45)) {
            vy = -vy;
          } else {
            vx = -vx;
          }

          for (; (std::rand() % 3) > 0;)
            g->addObject(new Particle
                ( x
                , y
                , std::atan2(vy,vx)+M_PI+((double)std::rand()/RAND_MAX-0.5)*M_PI*0.8
                , ((double)std::rand()/RAND_MAX)*0.2+0.3
                , ((double)std::rand()/RAND_MAX-0.5)*0.01
                , rand()%600+200
                ));

          x = _x + vx; y = _y + vy;
        }

        return None;
      }

    void render
      ( SDL_Renderer * r
      ) {
        V4 b{x-radius, y-radius, radius*2, radius*2};;
        SDL_SetRenderDrawColor (r, 255, 255, 255, 255);
        SDL_RenderFillRect (r, b.get());
      }
};

#endif // BALL_H
