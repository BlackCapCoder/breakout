#ifndef HUD_H
#define HUD_H

#include <string>
#include <SDL2/SDL_ttf.h>
#include "ColScene.h"

class Breakout;


class HUD : public GameObject<Breakout, bool>
{
private:
  const int     fontSize;
  const V4      rect;

  TTF_Font    * font;
  SDL_Surface * surface = nullptr;
  SDL_Texture * texture = nullptr;

  mutable std::string str;

private:
  void renderString (SDL_Renderer *, std::string &, double x, double y);

public:
  HUD
    ( const int fontSize
    )
    : fontSize{fontSize}
    , rect { 0, 0, 1000, 1000 }
  {}

  void init   (ResourceManager &, SDL_Renderer *);
  void render (SDL_Renderer *);
  bool logic  (double dt, const InputManager &, Breakout *);
};


#endif // ifndef HUD_H
