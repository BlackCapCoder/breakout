#ifndef LEVELS_H
#define LEVELS_H

#include <vector>
#include "Brick.h"

namespace Levels
{
  template <int N> struct Level {};

  int construct
    ( const int level
    , std::vector<Brick> & bricks
    , const int width
    , const int height
    );
}

#endif // ifndef LEVELS_H
