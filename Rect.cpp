#include <SDL2/SDL.h>
#include "Rect.h"

Rect::Rect(const Rect &other) : Rect(other.x, other.y, other.width, other.height) { }
Rect::Rect(double _x, double _y, double _width, double _height) :
    x(_x),
    y(_y),
    width(_width),
    height(_height) {
}
bool Rect::contains(const Rect &other) const noexcept {
    if (x > other.x) return false;
    if (y > other.y) return false;
    if (x + width  < other.x + other.width) return false;
    if (y + height < other.y + other.height) return false;
    return true; // within bounds
}
bool Rect::intersects(const Rect &other) const noexcept {
    if (x > other.x + other.width)  return false;
    if (x + width < other.x)        return false;
    if (y > other.y + other.height) return false;
    if (y + height < other.y)       return false;
    return true; // intersection
}
double Rect::getLeft()   const noexcept { return x - (width  * 0.5f); }
double Rect::getTop()    const noexcept { return y + (height * 0.5f); }
double Rect::getRight()  const noexcept { return x + (width  * 0.5f); }
double Rect::getBottom() const noexcept { return y - (height * 0.5f); }

SDL_Rect * Rect::get()
{
  rect = SDL_Rect { (int) x, (int) y, (int) width, (int) height};
  return &rect;
}
