#ifndef MATH_H
#define MATH_H


#include <cmath>
#include <cstdlib>


// ------------------ Utils

struct V2;

double randDouble ();
double randDouble (double low, double high);
double randDouble (V2 range);
int rand (int low, int high);

// ------------------ V2

struct V2
{
  double x, y;

  V2(const V2&);
  V2(double _x = 0, double _y = 0);
};

V2 operator+ (const V2 & a, const V2 & b);
V2 operator- (const V2 & a, const V2 & b);
V2 operator* (const V2 & a, const V2 & b);
V2 operator/ (const V2 & a, const V2 & b);

V2 operator+ (const V2 & p, const double & x);
V2 operator- (const V2 & p, const double & x);
V2 operator* (const V2 & p, const double & x);
V2 operator/ (const V2 & p, const double & x);
V2 operator^ (const V2 & p, const double & x);


double dot  (const V2 & a, const V2 & b);
double dist (const V2 & a, const V2 & b);

V2 closestPointPointLine (V2 p, V2 a, V2 b);




// ------------------ V4

#include <SDL2/SDL.h>

struct V4
{
  double x, y, w, h;
  SDL_Rect rect;

  bool contains(const V4 &other) const noexcept;
  bool intersects(const V4 &other) const noexcept;
  double getLeft() const noexcept;
  double getTop() const noexcept;
  double getRight() const noexcept;
  double getBottom() const noexcept;
  V2     getCenter() const noexcept;

  SDL_Rect * get();

  V4(const V4&);
  V4(double _x = 0, double _y = 0, double _w = 0, double _h = 0);
};


#endif
