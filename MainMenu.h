#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Game.h"
#include "GameObject.h"
#include "Breakout.h"
#include "Text.h"


class MainMenu : public Scene
{
private:
  enum Option
  { StartGame
  , Settings
  , Quit

  , NUM_OPTIONS
  };

private:
  const double animLen = 250;

  int selection    = StartGame;
  int oldSelection = selection;
  double animTime;
  bool dir;
  bool inAnim = false;

  int w, h;
  Game * game;

  TTF_Font    * font;
  SDL_Texture * texture[NUM_OPTIONS];
  int           widths[NUM_OPTIONS];


public:
  MainMenu (int w, int h, Game * g)
    : w{w}
    , h{h}
    , game{g}
  {}

  void init (ResourceManager * rm, SDL_Renderer * r)
  {
    font = rm->getFont ("resources/DroidSans.ttf", 100);
    init (r);
  }

  void init (SDL_Renderer * r) {
    const double sh = h * 0.1;

    SDL_SetRenderDrawColor(r, 255, 255, 255, 0);
    SDL_RenderFillRect(r, V4 {0, sh * selection, (double) w, sh+2} . get());

    for (int i = 0; i < NUM_OPTIONS; i++) {
      std::string str = getText((Option) i);
      SDL_Color c = i == selection ? SDL_Color { 0, 0, 0 } : SDL_Color {255, 255, 255, 0};
      auto s = TTF_RenderText_Solid(font, str.c_str(), c);
      widths[i] = s->w;
      texture[i] = SDL_CreateTextureFromSurface(r, s);
      SDL_FreeSurface(s);

      int sw = widths[i];
      const double y  = sh * i;
      const double x  = (w - sw)/2;
      SDL_RenderCopy (r, texture[i], nullptr, V4 { x, y, (double) sw, sh } . get());
    }
  }


  void logic
    ( double         dt // Milliseconds since last tick
    , InputManager * im
    , void         * st )
  {
    animTime = max (animTime-dt, 0);

    if (!inAnim) {
      if (im->isDown(MoveUp)) {
        oldSelection = selection;
        selection = (NUM_OPTIONS + selection - 1) % NUM_OPTIONS;
        animTime = animLen;
        dir = true;
        inAnim = true;
      } else if (im->isDown(MoveDown)) {
        oldSelection = selection;
        selection = (selection + 1) % NUM_OPTIONS;
        animTime = animLen;
        dir = false;
        inAnim = true;
      }
    }

    if (im->isDownFirst(MoveRight, FireRocket, ReleaseBall))
      onSelect();
  }

  void onSelect ()
  {
    switch (selection) {
      case (StartGame):
        game->setScene<Breakout>();
        break;

      case (Quit): // Quit
        exit(0);
        break;
    }
  }

  std::string getText (Option o)
  {
    switch (o) {
      case (StartGame): return "Start Game";
      case (Settings):  return "Settings";
      case (Quit):      return "Quit";
    }
    return "ERROR";
  }

  void render (SDL_Renderer * r)
  {
    if (!inAnim) return;
    inAnim = animTime > 0;

    const double sh = h * 0.1;
    double t = 1.0 - std::pow(animTime / animLen, 2);

    const double yo = sh * oldSelection;
    const double yn = sh * selection;

    V4 Old, New;

    if (!dir) {
      Old = V4 { 0, yo, (double) w, sh*t };
      New = V4 { 0, yn, (double) w, sh*t };
    } else {
      Old = V4 { 0, yo+sh-sh*t, (double) w, sh*t+1 };
      New = V4 { 0, yn+sh-sh*t, (double) w, sh*t+1 };
    }

    SDL_SetRenderDrawColor(r, 255, 255, 255, 0);
    SDL_RenderFillRect(r, New . get());

    SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
    SDL_RenderFillRect(r, Old . get());

    if (animTime == animLen) {
      SDL_DestroyTexture(texture[selection]);
      auto s = TTF_RenderText_Solid(font, getText((Option) selection).c_str(), SDL_Color{0,0,0});
      texture[selection] = SDL_CreateTextureFromSurface(r, s);
      SDL_FreeSurface(s);

      SDL_DestroyTexture(texture[oldSelection]);
      s = TTF_RenderText_Solid(font, getText((Option) oldSelection).c_str(), SDL_Color{255,255,255});
      texture[oldSelection] = SDL_CreateTextureFromSurface(r, s);
      SDL_FreeSurface(s);
    }

    int sw = widths[selection];
    double x  = (w - sw)/2;

    SDL_RenderSetClipRect (r, New . get());
    SDL_RenderCopy (r, texture[selection], nullptr, V4 { x, sh*selection, (double) sw, sh } . get());

    sw = widths[oldSelection];
    x  = (w - sw)/2;

    SDL_RenderSetClipRect (r, Old . get());
    SDL_RenderCopy (r, texture[oldSelection], nullptr, V4 { x, sh*oldSelection, (double) sw, sh } . get());

    SDL_RenderSetClipRect(r, V4{0,0,(double)w,(double)h}.get());
  }

};


#endif // ifndef MAIN_MENU_H
