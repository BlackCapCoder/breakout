#include <cstdlib>

#include "Brick.h"
#include "Upgrade.h"


Brick::Brick(float width, float height, float x_coord, float y_coord)
  : rect{x_coord, y_coord, width, height}
  , r{0}
  , g{0}
  , b{0}
  , a{0}
  , removed{false}
{
}

Brick::Brick()
  : rect{0, 0, 0, 0}
  , r{0}
  , g{0}
  , b{0}
  , a{0}
  , removed{false}
{
}

void Brick::setColor(float r, float g, float b, float a)
{
  this->r = r;
  this->g = g;
  this->b = b;
  this->a = a;
}

void Brick::render(SDL_Renderer* rend)
{
  SDL_SetRenderDrawColor (rend, r, g, b, a);
  SDL_RenderFillRect(rend, rect.get());
}

V4 * Brick::getBounds()
{
  return &rect;
}

ColResult Brick::logic (double, InputManager*, Breakout * b)
{
  if (removed) {
    b->numBricks--;
    return Remove;
  }

  return None;
}

void Brick::onHit (Breakout * g)
{
  if (std::rand() % 5 == 0) {
    g->addObject(new Upgrade {rect.getCenter()}, true);
  }

  removed = true;
}

