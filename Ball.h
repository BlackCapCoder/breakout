#ifndef BALL_H
#define BALL_H

#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include <vector>

#include "GameObject.h"
#include "QuadTree.h"

class Ball : public GameObject {
  double x = 500, y = 800;
  double radius   = 5;
  double vx = 0.3, vy = 0.3;

  public:
    LogicResult logic
      ( double         dt
      , InputManager * im
      , QuadTree     * qt
      ) {

        x += vx * dt;
        y += vy * dt;

        if (x <= radius) {
          x = radius;
          vx = -vx;
        } else if (x + radius >= 1000) {
          x = 1000 - radius;
          vx = -vx;
        }

        if (y <= radius) {
          y = radius;
          vy = -vy;
        } else if (y + radius >= 1000) {
          y = 1000 - radius;
          vy = -vy;
        }

        std::vector<Collidable*> os = qt->getObjectsInBound(Rect {x-radius, y-radius, radius*2, radius*2});
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

          std::cout << "HIT: " << a << std::endl;

        }

        return None;
      }

    /*

  return Point.new( :x => A.x + a_to_b[0]*t,
                    :y => A.y + a_to_b[1]*t )
                                      # Add the distance to A, moving
                                      #   towards B

end
    */

    void render
      ( SDL_Renderer * r
      ) {
        Rect b{x-radius, y-radius, radius*2, radius*2};;
        SDL_SetRenderDrawColor (r, 255, 255, 255, 255);
        SDL_RenderFillRect (r, b.get());
      }
};

#endif // BALL_H
