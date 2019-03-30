#include "Text.h"

Text::Text(SDL_Renderer* r, std::string const& text,
           SDL_Color color, int fontSize, int x, int y)
  : r{ r }
  , font{ TTF_OpenFont("Sans.ttf", fontSize) }
  , surface{ TTF_RenderText_Solid(font, text.c_str(), color) }
  , texture{ SDL_CreateTextureFromSurface(r, surface) }
  , rect{ x, y, 100, 100 }
{
}

void Text::render(SDL_Renderer* r)
{
  SDL_RenderCopy(r, texture, nullptr, &rect);
}
