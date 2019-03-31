#ifndef LEVEL1_H
#define LEVEL1_H

#include "../Levels.h"

namespace Levels
{
  template <>
  struct Level<1>
  {
    static int construct
      ( std::vector<Brick> & bricks
      , const int width
      , const int height

      , const float horMarg = 50
      , const float verMarg = 50
      , const float space   = 15
      , const int   ncols   = 10
      , const int   nrows   = 7
      , const float h       = 40
      , const bool  stagger = true
      )
    {
      const float w = (width - 2*horMarg - space*ncols) / ncols;

      for (int i = 0; i < nrows*ncols; i++) {
        int x = i % ncols;
        int y = i / ncols;
        bricks.push_back(Brick
          { horMarg + (float) x * (w+space) + (stagger ? (y % 2 == 0? space : -space) : 0)
          , verMarg + (float) y * (h+space)
          , w
          , h
          , (float) i * 80
          , (float) i * 50 + 90
          , 255
          , 255
          });
      }

      return nrows*ncols;
    }
  };
}

#endif // ifndef LEVEL1_H
