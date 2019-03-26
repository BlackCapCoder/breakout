#include "Brick.h"


Brick::Brick(float width, float height, float x_coord, float y_coord)
  : rect{x_coord, y_coord, width, height}
  , r{0}
  , g{0}
  , b{0}
  , a{0}
{
}


void Brick::setColor(float r, float g, float b, float a)
{
  this->r = r;
  this->g = g;
  this->b = b;
  this->a = a;
}

void Brick::render(SDL_Renderer* r)
{
  SDL_RenderFillRect(r, rect.get());

}
