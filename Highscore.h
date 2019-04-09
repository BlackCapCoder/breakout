#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <iostream>
#include <stdexcept>
#include <array>
#include <fstream>
#include <algorithm>
#include "GameObject.h"

int globalpoints = 50;

#define SETTEXT(T) \
  TTF_RenderText_Solid(font, T, {255, 255, 255, 255})

#define CREATETEXT(T) \
  SDL_CreateTextureFromSurface(&args.rend, T)

#define SETR(x, y, T) \
  SDL_Rect{x, y, T->w, T->h}

class Highscore : public Scene {
public:
  TTF_Font*                   font;
  SDL_Surface*                highscoreSurface;
  std::array<SDL_Surface*, 4> scoreArraySurface;
  SDL_Texture*                texture;
  std::array<SDL_Texture*, 4> scoreArrayTexture;
  std::array<SDL_Rect, 4>     rect;

  std::array<u_int16_t, 3> readHighscores(std::string const& fp)
  {
    auto* pf = std::fopen(fp.c_str(), "r");
    std::array<u_int16_t, 3> arr{};

    for (int i{}; i < 3; i++)
      arr[i] = std::fgetc(pf) + (std::fgetc(pf) << 8);

    return arr;
  }

  Highscore(const InitArgs args)
    : font{args.rm.getFont("resources/DroidSans.ttf", 200)}
    , scoreArraySurface{ SETTEXT("Highscore") }
  {
    std::array<u_int16_t, 3> scores = readHighscores("scores.txt");

    scoreArrayTexture[0] = CREATETEXT(scoreArraySurface[0]);
    rect[0] = SETR(500, 100, scoreArraySurface[0]);

    for (int i = 1; i != 4; ++i) {
      std::string s = std::to_string(scores[i-1]);
      scoreArraySurface[i] = SETTEXT(s.c_str());
      scoreArrayTexture[i] = CREATETEXT(scoreArraySurface[i]);
      rect[i] = SETR(500, scoreArraySurface[i]->h*i + 100, scoreArraySurface[i]);
    }

    for (auto i : scoreArraySurface)
      SDL_FreeSurface(i);
  }

  SceneR tick (const TickArgsS args) override
  {
    SDL_RenderClear(&args.r);
    for (int i{}; i != scoreArrayTexture.size(); ++i)
      SDL_RenderCopy(&args.r, scoreArrayTexture[i], nullptr, &rect[i]);
    return false;
  }

  ~Highscore()
  {
    for (auto i : scoreArrayTexture)
      SDL_DestroyTexture(i);
  }
};

#endif // HIGHSCORE_H
