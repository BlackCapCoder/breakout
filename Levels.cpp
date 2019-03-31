#include "Levels/Level1.h"
#include "Levels/Level2.h"

namespace Levels
{
  int construct
    ( const int level
    , std::vector<Brick> & bricks
    , const int width
    , const int height
    )
  {
    switch (level) {
      case (1): return Level<1>::construct (bricks, width, height);
      case (2): return Level<2>::construct (bricks, width, height);
    }
    return 0;
  }
}
