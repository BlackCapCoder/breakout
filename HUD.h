#ifndef HUD_H
#define HUD_H

#include <string>
#include <SDL2/SDL_ttf.h>
#include "ColScene.h"

class Breakout;


class HUD : public GameObject<bool, Breakout*>
{
private:
  const int     fontSize;
  const V4      rect;

  TTF_Font    * font;
  SDL_Surface * surface = nullptr;
  SDL_Texture * texture = nullptr;

  mutable char str[256];

private:
  void renderString (SDL_Renderer &, char *, double x, double y);

public:
  HUD (const InitArgs, const int fontSize);

  void render (const RenderArgs) override;
  bool logic  (const LogicArgs<Breakout*>) override;
};


#endif // ifndef HUD_H
