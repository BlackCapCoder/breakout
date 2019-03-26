#pragma once
#include "QuadTree.h"


class GameObject {
  public:
    virtual Rect getBounds ();
    virtual void logic  (int tick);
    virtual void render ();
};


class Paddle : public GameObject {

  public:
    Rect getBounds () {
      return Rect {0, 0, 100, 30};
    }

    void logic (int tick) {

    }

    void render () {

    }
};
