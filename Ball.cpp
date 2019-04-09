#include <set>
#include "Ball.h"
#include "Breakout.h"
#include "Particle.h"
#include <SDL2/SDL2_gfxPrimitives.h>


inline
V4 Ball::getBounds () const
{
  return V4 {x-radius, y-radius, radius*2, radius*2};
}

void Ball::render (const RenderArgs args)
{
  for (int i = 1; i < pts.size(); i++) {
    if (isMeteor)
      thickLineRGBA (&args.rend, pts[i-1].x, pts[i-1].y, pts[i].x, pts[i].y, radius*2, 255,   0,   0, 255);
    else
      thickLineRGBA (&args.rend, pts[i-1].x, pts[i-1].y, pts[i].x, pts[i].y, radius*2, 255, 255, 255, 255);
  }

  SDL_SetRenderDrawColor (&args.rend, 255, 255, 255, 255);
  SDL_RenderFillRect (&args.rend, getBounds().get());
}


bool Ball::logic (const LogicArgs<Breakout*> args)
{
  if (args.st()->hasMagnet() && args.im().isDown(PowerMagnet)) {
    V4 p = args.st()->paddle.getBounds();
    double q = p.x + p.w/2;
    if (x < q) vx += 0.005 * args.dt();
    if (x > q) vx -= 0.005 * args.dt();
  }

  isMeteor = args.st()->meteorActive();

  pts.clear ();
  pts.push_back({(int) x, (int) y});

  // We need to loop through all the collisions that
  // have taken place since last tick. See (1) below.
  double dt = args.dt();
  double q, mx, my, consumed;
  bool hor;
  char numCollisions = 0;

  V4 screen = args.st()->getBounds();
  std::set<ColObj<Breakout*>*> hit;

  do {
    mx       = vx * args.st()->getLevelSpeed();
    my       = vy * args.st()->getLevelSpeed();
    consumed = dt;

    double _x = x + mx * dt;
    double _y = y + my * dt;
    V4 b = V4 { min   (x, _x) - radius
              , min   (y, _y) - radius
              , delta (x, _x) + radius * 2
              , delta (y, _y) + radius * 2
              };


    // ---- Screen edges

    // bottom
    q = (screen.y + screen.h - radius - y)/my;
    if (q > 0 && q < consumed) {
      isDead = true;
      args.st()->onBallLost (*this);
      return true;
    }

    // top
    q = (screen.y + radius - y)/my;
    if (q > 0 && q < consumed) { consumed = q; hor = false; }

    // left
    q = (screen.x + radius - x)/mx;
    if (q > 0 && q < consumed) { consumed = q; hor = true; }

    // right
    q = (screen.x + screen.w - radius - x)/mx;
    if (q > 0 && q < consumed) { consumed = q; hor = true; }


    // ---- Object collision
    auto os = args.st()->getObjectsInBound (b);

    // Ignore everything but the paddle if meteor
    if (isMeteor) {
      double a = std::atan2 (my, mx) / (2 * M_PI) + 0.5;
      double s = args.st()->getLevelSpeed()*0.4;
      Particle<Breakout*>::explosion
        ( V4 { min(x, _x), min(y, _y), max(x, _x), max(y, _y) }
        , V2 { a, a }
        , V2 { s, s }
        , V2 { -0.0007, 0.0007 }
        , V2 { 50, 300 }
        , 0, (int) max (dt/2, 2)
        , [ g=args.st() ] (auto p) { g->insert ((GameObject<bool, Breakout*>*)p); }
        );

      for (auto obj : os) obj->onHit (args.st());
      os.clear();

      if (b.intersects(args.st()->paddle.getBounds()))
        os.push_back(&args.st()->paddle);
    }

    // Check for collisions
    if (!os.empty()) {
      ColObj<Breakout*> * c = nullptr;

      for (auto obj : os) {
        if (hit.find (obj) != hit.end()) continue;
        b = obj->getBounds();

        q = ((mx > 0 ? b.x : b.x + b.w) - x - radius) / mx;
        if (q < consumed && q > 0) { hor = true;  consumed = q; c = obj; }

        q = ((my > 0 ? b.y : b.y + b.h) - y - radius) / my;
        if (q < consumed && q > 0) { hor = false; consumed = q; c = obj; }
      }

      if (c != nullptr) {
        c->onHit   (args.st());
        hit.insert (c);
      }
    }


    // ---- Update variables

    x  += consumed * mx;
    y  += consumed * my;

    if (consumed < dt) {
      if (hor) vx = -vx; else vy = -vy;

      if (isMeteor) {
        V2 dir;

        if (!hor && vy > 0) dir = {0.0, 0.5};
        if (!hor && vy < 0) dir = {0.5, 1.0};
        if ( hor && vx < 0) dir = {0.25, 0.75};
        if ( hor && vx > 0) dir = {0.75, 1.25};

        double s = args.st()->getLevelSpeed();

        Particle<Breakout*>::explosion
          ( V4 { x, y, x, y }
          , dir
          , V2 { 0.5*s, 1.2*s }
          , V2 { -0.0007, 0.0007 }
          , V2 { 100, 600 }
          , 8*s, 20*s
          , [ g=args.st() ] (auto p) { g->insert ((GameObject<bool, Breakout*>*) p); }
          );
      }
    }

    dt -= consumed;
    pts.push_back({(int) x, (int) y});
    if (dt > 0) numCollisions++;
  } while (dt > 0);

  // Just to be safe..
  if (y < screen.y + radius)            y = screen.y + radius + 1;
  if (y > screen.y + screen.h - radius) y = screen.y + screen.h - radius - 1;
  if (x < screen.x + radius)            x = screen.x + radius + 1;
  if (x > screen.x + screen.w - radius) x = screen.x + screen.w - radius - 1;

  if (numCollisions > 0) {
    args.st()->onBounce();
  }

  return false;
}


/* (1) Math

  Let `r`        be the radius of our ball
  Let `(x, y)`   be the position of the center of our ball
  Let `(vx, vy)` be the movement vector

  Our new position after `dt` time has passed is given by:

      (x, y) + (vx, vy) * dt = (x', y')


  For every object that intersects the square `(x' - r, y' - r, r*2, r*2)`,
  let `(ox, oy, ow, oh)` its bounds.

  Assuming that an object is rectangular, it has 4 sides:

      Top:    (ox     , oy     , ox + ow, oy     )
      Bottom: (ox     , oy + oh, ox + ow, oy + oh)
      Left:   (ox     , oy     , ox     , oy + oh)
      Right:  (ox + ow, oy     , ox + ow, oy + oh)

  Instead of thinking of these as line segments, we can think
  of them as rays, and describe them as vectors with a stepper `t`,
  in the same way that we did with our ball:

      Top:    (ox, oy     ) + (1, 0)*t
      Bottom: (ox, oy + oh) + (1, 0)*t
      Left:   (ox, oy     ) + (0, 1)*t
      Right:  (ox + ow, oy) + (0, 1)*t

  We want to find out how long it took for our ball to hit one of the sides.
  To find this we set them equal and solve for the side with the 0 stepper:

      (x, y) + (vx, vy) * X = (ox, oy) + (1, 0)*t
      y + vy * X = oy + 0 * t
      vy * X = oy - y
      X = (oy - y) / vy

  We can only collide with 2 of the 4 sides: if we are traveling
  towards the left side, we cannot collide with the right side first.
  The same goes for the vertical sides:

      hor  = iff vx > 0 then ox else ox + ow
      vert = iff vy > 0 then oy else oy + oh

      Xh = (hor  - x) / vx;
      Xv = (vert - y) / vy;


  `Xh` and `Xy` is how long it took for the center point of
  our ball to reach some side, so we need to subtract the radius:

      X = min (Xh - r / vx, Xv - r / vy)


  To find what we collided with first, we find the smallest `X`
  for all objects.

*/
