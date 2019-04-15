#include "HUD.h"
#include "Breakout.h"

HUD::HUD (const InitArgs args, const int fontSize)
  : fontSize { fontSize }
  , rect     { 0, 0, (double) args.w, (double) args.h }
  , font     { args.rm.getFont ("resources/DroidSans.ttf", fontSize) }
{}

void HUD::renderString (SDL_Renderer & r, char * str, double x, double y)
{
  surface = TTF_RenderText_Solid(font, str, SDL_Color {255, 255, 255, 0});
  texture = SDL_CreateTextureFromSurface(&r, surface);

  double w = surface->w;
  double h = surface->h;
  SDL_RenderCopy (&r, texture, nullptr, V4 { x, y, w, h } . get());
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void HUD::operator()(const ProxyIX<RENDER>, const RenderArgs args)
{
  double pad = 10;
  renderString (args.rend, str, pad, rect.h - fontSize - pad);
}

bool HUD::operator()(const ProxyIX<LOGIC>, const LogicArgs<Breakout*> args)
{
  double hash = 0
    + args.st()->spareBalls
    + args.st()->numRockets
    + args.st()->magnetCharge/1000
    + args.st()->points;

  if (hash == oldHash) return false;
  oldHash = hash;
  args.dirty = true;

  std::sprintf
    ( str
    , "Balls: %u  Rockets: %u  Magnet: %.1fs  Points: %u"
    , args.st()->spareBalls
    , args.st()->numRockets
    , args.st()->magnetCharge/1000
    , args.st()->points
    );

  return false;
}

