#include "HUD.h"
#include "Breakout.h"


void HUD::renderString (SDL_Renderer * r, std::string & str, double x, double y)
{
  int l = str.length();
  double w = fontSize * 0.35 * l;
  double h = fontSize;

  surface = TTF_RenderText_Solid(font, str.c_str(), SDL_Color {255, 255, 255, 0});
  texture = SDL_CreateTextureFromSurface(r, surface);
  SDL_RenderCopy (r, texture, nullptr, V4 { x, y, w, h } . get());
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void HUD::render (SDL_Renderer * r)
{
  double pad = 10;
  double sp  = fontSize + 3;
  double y   = rect.h - pad - sp*3;
  renderString (r, strBalls  , pad, y + sp*0);
  renderString (r, strRockets, pad, y + sp*1);
  renderString (r, strMagnet , pad, y + sp*2);
}

bool HUD::logic (double dt, InputManager *, Breakout * b)
{
  strBalls   = "Balls:   " + std::to_string (b->spareBalls);
  strRockets = "Rockets: " + std::to_string (b->numRockets);
  strMagnet  = "Magnet:  " + std::to_string (b->magnetCharge);
  return false;
}
