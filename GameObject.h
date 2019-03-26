#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "QuadTree.h"


class GameObject {
  public:
    Rect getBounds ();
    void logic  (int tick) {}
    void render () {}
};

#endif // GAMEOBJECT_H
