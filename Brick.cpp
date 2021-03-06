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

void Brick::operator()(const ProxyIX<RENDER>, const RenderArgs args)
{
  SDL_SetRenderDrawColor (&args.rend, r, g, b, a);
  SDL_RenderFillRect     (&args.rend, rect.get());
}

V4 & Brick::getBounds ()
{
  return rect;
}

ColResult Brick::operator()(const ProxyIX<LOGIC>, const LogicArgs<Breakout*> args)
{
  if (removed) {
    dirty = true;
    args.st()->numBricks--;
    return Remove;
  }

  return None;
}

void Brick::operator()(const ProxyIX<ONHIT>, Breakout* g)
{
  g->onBrickRemoved (*this);
  removed = true;
}

