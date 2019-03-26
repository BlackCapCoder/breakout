#pragma once
#include "QuadTree.h"


class GameObject {
  public:
    Rect getBounds ();
    void logic  (int tick) {}
    void render () {}
};
