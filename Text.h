#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "GameObject.h"
#include "Breakout.h"

class Text : public GameObject<Breakout, bool> {
  SDL_Renderer* r;
  TTF_Font*     font;
  SDL_Surface*  surface;
  SDL_Texture*  texture;
  SDL_Rect      rect;

public:
  explicit Text(SDL_Renderer* r, std::string const& text,
                SDL_Color color, int fontSize, int x, int y);

  void render(SDL_Renderer* r);
};

#endif // TEXT_H
