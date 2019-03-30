#include "Math.h"


// ------------------ Utils

double randDouble ()
{
  return (double) std::rand() / RAND_MAX;
}

double randDouble (double low, double high)
{
  return randDouble () * (high - low) + low;
}

double randDouble (V2 range)
{
  return randDouble (range.x, range.y);
}

int rand (int low, int high)
{
  return low + (std::rand() % (high - low));
}


// ------------------ V2

V2::V2(const V2 &other) : V2(other.x, other.y) {}
V2::V2(double _x, double _y) : x(_x), y(_y) {}

V2 operator+ (const V2 & a, const V2 & b) { return V2 { a.x + b.x, a.y + b.y }; }
V2 operator- (const V2 & a, const V2 & b) { return V2 { a.x - b.x, a.y - b.y }; }
V2 operator* (const V2 & a, const V2 & b) { return V2 { a.x * b.x, a.y * b.y }; }
V2 operator/ (const V2 & a, const V2 & b) { return V2 { a.x / b.x, a.y / b.y }; }

V2 operator+ (const V2 & p, const double & x) { return V2 { p.x + x, p.y + x }; }
V2 operator- (const V2 & p, const double & x) { return V2 { p.x - x, p.y - x }; }
V2 operator* (const V2 & p, const double & x) { return V2 { p.x * x, p.y * x }; }
V2 operator/ (const V2 & p, const double & x) { return V2 { p.x / x, p.y / x }; }
V2 operator^ (const V2 & p, const double & x) { return V2 { std::pow(p.x, x), std::pow(p.y, x) }; }


double dot  (const V2 & a, const V2 & b) { return a.x * b.x + a.y * b.y; }
double dist (const V2 & a, const V2 & b)
{
  return std::sqrt (std::pow (b.x - a.x, 2) + std::pow (b.y - a.y, 2));
}

V2 closestPointPointLine (V2 p, V2 a, V2 b)
{
  V2    k = b - p;
  float t = dot (p - a, k) / dot (k, k);
  return a + k * t;
}


// ------------------ V4

V4::V4(const V4 &other) : V4(other.x, other.y, other.w, other.h) { }
V4::V4(double _x, double _y, double _w, double _h) :
  x(_x),
  y(_y),
  w(_w),
  h(_h) {}

bool V4::contains(const V4 &other) const noexcept
{
  if (x > other.x) return false;
  if (y > other.y) return false;
  if (x + w < other.x + other.w) return false;
  if (y + h < other.y + other.h) return false;
  return true; // within bounds
}

bool V4::intersects(const V4 &other) const noexcept
{
  if (x > other.x + other.w) return false;
  if (x + w < other.x)       return false;
  if (y > other.y + other.h) return false;
  if (y + h < other.y)       return false;
  return true; // intersection
}

double V4::getLeft()   const noexcept { return x - (w * 0.5f); }
double V4::getTop()    const noexcept { return y + (h * 0.5f); }
double V4::getRight()  const noexcept { return x + (w * 0.5f); }
double V4::getBottom() const noexcept { return y - (h * 0.5f); }

V2 V4::getCenter() const noexcept
{
  return V2 { x + w/2, y + h/2 };
}

SDL_Rect * V4::get()
{
  rect = SDL_Rect { (int) x, (int) y, (int) w, (int) h };
  return &rect;
}
