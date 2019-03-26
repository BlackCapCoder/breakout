#ifndef RECT_H
#define RECT_H

#include <SDL2/SDL.h>

struct Rect {
  double x, y, width, height;
  SDL_Rect rect;

  bool contains(const Rect &other) const noexcept;
  bool intersects(const Rect &other) const noexcept;
  double getLeft() const noexcept;
  double getTop() const noexcept;
  double getRight() const noexcept;
  double getBottom() const noexcept;

  SDL_Rect * get();

  Rect(const Rect&);
  Rect(double _x = 0, double _y = 0, double _width = 0, double _height = 0);
};

#endif // RECT_H
