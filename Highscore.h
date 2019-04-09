#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <iostream>
#include <stdexcept>
#include "GameObject.h"

class Highscore : public Scene {
public:
  TTF_Font* font;
  SDL_Surface* surface;
  SDL_Texture* texture;
  SDL_Rect rect;
  Highscore(const InitArgs args)
    : font{args.rm.getFont("resources/DroidSans.ttf", 24)}
    , surface{ TTF_RenderText_Solid(font, "Cock", {255, 255, 255, 255}) }
    , texture{ SDL_CreateTextureFromSurface(&args.rend, surface) }
    , rect { 50, 50, args.w, args.h }
  {
    if (surface == nullptr)
      throw std::runtime_error("Could not create font surface");

    SDL_FreeSurface(surface);
  }

  SceneR tick (const TickArgsS args) override
  {
    SDL_RenderCopy(&args.r, texture, nullptr, &rect);
    return false;
  }

  ~Highscore()
  {
    SDL_DestroyTexture(texture);
  }
};

#endif // HIGHSCORE_H
