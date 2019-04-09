#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <iostream>
#include <stdexcept>
#include <array>
#include <fstream>
#include "GameObject.h"
#include "HighscoreManager.h"


class Highscore : public Scene
{
private:
  static constexpr unsigned NScores = HighscoreManager::NScores;
  static constexpr int fontSize     = 200;
  static constexpr int topMargin    = 100;
  static constexpr int leftMargin   = 500;

  TTF_Font*                           font;
  std::array<SDL_Texture*, NScores+1> scoreArrayTexture;
  std::array<SDL_Rect,     NScores+1> rect;
  bool redraw = true;

public:
  Highscore (const InitArgs args)
    : font{args.rm.getFont ("resources/DroidSans.ttf", fontSize)}
  {
    SDL_Surface * surf = TTF_RenderText_Solid (font, "Highscore", {255, 255, 255, 255});
    scoreArrayTexture[0] = SDL_CreateTextureFromSurface(&args.rend, surf);
    rect[0] = SDL_Rect {leftMargin, topMargin, surf->w, surf->h};
    SDL_FreeSurface (surf);

    auto scores = HighscoreManager::read();

    for (int i = 1; i < NScores+1; ++i) {
      surf = TTF_RenderText_Solid (font, std::to_string(scores[i-1]).c_str(), {255, 255, 255, 255});
      scoreArrayTexture[i] = SDL_CreateTextureFromSurface(&args.rend, surf);
      rect[i] = SDL_Rect {leftMargin, fontSize*i + topMargin, surf->w, surf->h};
      SDL_FreeSurface (surf);
    }
  }

  SceneR tick (const TickArgsS args) override
  {
    if (redraw) {
      SDL_SetRenderDrawColor(&args.r,0,0,0,255);
      SDL_RenderClear (&args.r);
      for (int i{}; i != scoreArrayTexture.size(); ++i)
        SDL_RenderCopy (&args.r, scoreArrayTexture[i], nullptr, &rect[i]);
      redraw     = false;
      args.dirty = true;
    }

    return redraw = args.im().isDownFirst (MenuSelect);
  }

  ~Highscore()
  {
    for (auto i : scoreArrayTexture)
      SDL_DestroyTexture(i);
  }
};

#endif // HIGHSCORE_H
