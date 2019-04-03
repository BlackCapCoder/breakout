#include <cstdlib>

#include "Brick.h"
#include "Upgrade.h"


Brick::Brick
  ( const float x_coord
  , const float y_coord
  , const float width
  , const float height
  , const float r
  , const float g
  , const float b
  , const float a
  ) : rect{x_coord, y_coord, width, height}
    , r{r}
    , g{g}
    , b{b}
    , a{a}
    , removed{false}
{}

void Brick::render (SDL_Renderer & rend)
{
  SDL_SetRenderDrawColor (&rend, r, g, b, a);
  SDL_RenderFillRect     (&rend, rect.get());
}

V4 & Brick::getBounds ()
{
  return rect;
}

ColResult Brick::logic (const LogicArgs<Breakout*> args)
{
  if (removed) {
    args.st()->numBricks--;
    return Remove;
  }

  return None;
}

void Brick::onHit (Breakout * g)
{
  g->onBrickRemoved (*this);
  removed = true;
}

