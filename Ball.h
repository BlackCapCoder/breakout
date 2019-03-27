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

class Ball : public GameObject {
  double x = 500, y = 500;
  double radius = 5;
  double vx = 0.6, vy = 0.6;

  public:
    LogicResult logic
      ( double dt
      , Game * g
      ) {

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

        std::vector<Collidable*> os = g->qt->getObjectsInBound(Rect {x-radius, y-radius, radius*2, radius*2});
        if (!os.empty()) {
          GameObject * obj = static_cast<GameObject*>(os[0]);
          obj->onHit();
          Rect b = *(obj->getBounds());

          float ax = b.x;
          float bx = b.x + b.width;
          float ay = b.y + b.height / 2;
          float by = ay;

          float a2p_x = x - ax
              , a2p_y = y - ay;

          float a2b_x = bx - x
              , a2b_y = by - y;

          float atb2 = a2b_x*a2b_x + a2b_y*a2b_y;

          float atp_dot_atb = a2p_x*a2b_x + a2p_y*a2b_y;
          float t = atp_dot_atb / atb2;

          float cx = ax + a2b_x*t
              , cy = ay + a2b_y*t;

          int a = ((int) (std::atan2(cx-x, cy-y) * (180/M_PI)) + 90) % 360;
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
        }

        return None;
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
