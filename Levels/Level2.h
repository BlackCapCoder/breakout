#ifndef LEVEL2_H
#define LEVEL2_H

#include "../Levels.h"
#include "Level1.h"

namespace Levels
{
  template <>
  struct Level<2>
  {
    static int construct
      ( std::vector<Brick> & bricks
      , const int width
      , const int height

      , const float horMarg = 50
      , const float verMarg = 50
      , const float space   = 15
      , const int   ncols   = 10*16/9
      , const int   nrows   = 10
      , const float h       = 40
      , const bool  stagger = false
      )
    {
      return Level<1>::construct
        ( bricks, width, height
        , horMarg
        , verMarg
        , space
        , ncols
        , nrows
        , h
        , stagger
        );
    }
  };
}

#endif // ifndef LEVEL2_H
