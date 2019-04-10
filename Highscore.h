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
  static constexpr int fontSizeHeader = 200;
  static constexpr int fontSize       = 100;
  static constexpr int topMargin      = 100;

  TTF_Font* fontHeader;
  TTF_Font* font;
  bool      redraw = true;
  int       w;

public:
  Highscore (const InitArgs args)
    : fontHeader {args.rm.getFont ("resources/DroidSans.ttf", fontSizeHeader)}
    , font       {args.rm.getFont ("resources/DroidSans.ttf", fontSize)}
    , w          {args.w}
  {
  }

  void render (SDL_Renderer & rend)
  {
    SDL_SetRenderDrawColor (&rend, 0, 0, 0, 255);
    SDL_RenderClear        (&rend);

    SDL_Surface * surf;
    SDL_Texture * text;
    SDL_Rect      rect;

    surf = TTF_RenderText_Solid (fontHeader, "HIGHSCORES", {255, 255, 255, 255});
    text = SDL_CreateTextureFromSurface(&rend, surf);
    rect = SDL_Rect {(w - surf->w)/2, topMargin, surf->w, surf->h};
    SDL_RenderCopy (&rend, text, nullptr, &rect);
    SDL_FreeSurface (surf);
    SDL_DestroyTexture (text);

    auto scores = HighscoreManager::read();
    bool foundScore = false;

    for (int i = 1; i < scores.size()+1; ++i) {
      if (!foundScore && scores[i-1] == HighscoreManager::getLastScore()) {
        foundScore = true;
        surf = TTF_RenderText_Solid (font, std::to_string(scores[i-1]).c_str(), {255, 255, 0, 0});
      } else {
        surf = TTF_RenderText_Solid (font, std::to_string(scores[i-1]).c_str(), {255, 255, 255, 255});
      }

      text = SDL_CreateTextureFromSurface(&rend, surf);
      rect = SDL_Rect {(w - surf->w)/2, rect.y + rect.h, surf->w, surf->h};
      SDL_RenderCopy (&rend, text, nullptr, &rect);
      SDL_FreeSurface (surf);
      SDL_DestroyTexture (text);
    }
  }

  SceneR tick (const TickArgsS args) override
  {
    if (redraw) {
      render (args.rend);
      redraw     = false;
      args.dirty = true;
    }

    return redraw = args.im().isDownFirst (MenuSelect);
  }
};

#endif // HIGHSCORE_H
