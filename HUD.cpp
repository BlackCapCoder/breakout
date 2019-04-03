#include "HUD.h"
#include "Breakout.h"

HUD::HUD(const int w, const int h, const int fontSize)
  : fontSize{ fontSize }
  , rect{ 0, 0, (double) w, (double) h }
{}

void HUD::renderString (SDL_Renderer * r, char * str, double x, double y)
{
  surface = TTF_RenderText_Solid(font, str, SDL_Color {255, 255, 255, 0});
  texture = SDL_CreateTextureFromSurface(r, surface);

  double w = surface->w;
  double h = surface->h;
  SDL_RenderCopy (r, texture, nullptr, V4 { x, y, w, h } . get());
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void HUD::init (ResourceManager & rm, SDL_Renderer *)
{
  font = rm.getFont("resources/DroidSans.ttf", fontSize);
}

void HUD::render (SDL_Renderer * r)
{
  double pad = 10;
  renderString (r, str, pad, rect.h - fontSize - pad);
}

bool HUD::logic (double dt, const InputManager &, Breakout * b)
{
  std::sprintf
    ( str
    , "Balls: %u  Rockets: %u  Magnet: %.1fs  Points: %u"
    , b->spareBalls
    , b->numRockets
    , b->magnetCharge/1000
    , b->points
    );

  return false;
}

