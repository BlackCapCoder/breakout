#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "GameObject.h"
#include "Breakout.h"


class MainMenu : public Scene
{
private:
  enum Option
  { StartGame
  , Settings
  , Highscore
  , Quit

  , NUM_OPTIONS
  };

private:
  static constexpr double animLen = 150;

  int selection    = StartGame;
  int oldSelection = selection;
  double animTime  = 0;
  bool dir;
  bool inAnim    = false;
  bool redraw    = true;
  bool firstInit = true;

  const int w;
  const int h;
  ResourceManager & rm;

  TTF_Font    * font;
  SDL_Texture * texture[NUM_OPTIONS];
  int           widths[NUM_OPTIONS];

  ResourceManager::Sound<str("resources/Selection.wav")> selectionSound;

public:
  MainMenu (InitArgs args)
    : w{args.w}
    , h{args.h}
    , rm{args.rm}
    , font{args.rm.getFont ("resources/DroidSans.ttf", 100)}
    , selectionSound {args.rm}
  {
  }

  SceneR tick (const TickArgsS args)
  {
    if (logic (args.l)) return true;

    if (redraw) {
      init (args.r);
      redraw = false;
      args.dirty = true;
    } else if (inAnim) {
      render (args.r);
      args.dirty = true;
    }

    return false;
  }

  SceneR logic (const LogicArgsS args)
  {
    animTime = max (animTime - args.dt(), 0);

    if (!inAnim) {
      if (args.im().isDown(MoveUp)) {
        oldSelection = selection;
        selection    = (NUM_OPTIONS + selection - 1) % NUM_OPTIONS;
        animTime     = animLen;
        dir          = true;
        inAnim       = true;
        selectionSound.play ();
      } else if (args.im().isDown(MoveDown)) {
        oldSelection = selection;
        selection    = (selection + 1) % NUM_OPTIONS;
        animTime     = animLen;
        dir          = false;
        inAnim       = true;
        selectionSound.play ();
      }
    }

    if (args.im().isDownFirst(MenuSelect, MoveRight)) {
      switch (selection) {
        case (StartGame):
          redraw = true;
          return true;

        case (Quit): // Quit
          exit(0);
          break;
      }
    }

    return false;
  }

  std::string getText (Option o)
  {
    switch (o) {
      case (StartGame): return "Start Game";
      case (Settings):  return "Settings";
      case (Highscore): return "Highscore";
      case (Quit):      return "Quit";
    }
    return "ERROR";
  }

  // Redraws everything
  void init (SDL_Renderer & r) {
    const double sh = h * 0.1;
    const double off = (h - sh*NUM_OPTIONS)/2;

    SDL_SetRenderDrawColor(&r, 0, 0, 0, 0);
    SDL_RenderClear(&r);
    SDL_SetRenderDrawColor(&r, 255, 255, 255, 0);
    SDL_RenderFillRect(&r, V4 {0, off+sh * selection, (double) w, sh+2} . get());

    for (int i = 0; i < NUM_OPTIONS; i++) {
      std::string str = getText((Option) i);
      SDL_Color c = i == selection ? SDL_Color { 0, 0, 0 } : SDL_Color {255, 255, 255, 0};
      auto s = TTF_RenderText_Solid(font, str.c_str(), c);
      widths[i] = s->w;
      if (!firstInit) SDL_DestroyTexture (texture[i]);
      texture[i] = SDL_CreateTextureFromSurface(&r, s);
      SDL_FreeSurface(s);

      int sw = widths[i];
      const double y  = sh * i;
      const double x  = (w - sw)/2;
      SDL_RenderCopy (&r, texture[i], nullptr, V4 { x, off+y, (double) sw, sh } . get());
    }

    firstInit = false;
  }

  // Partial redraw
  void render (SDL_Renderer & r)
  {
    inAnim = animTime > 0;

    const double sh = h * 0.1;
    const double off = (h - sh * NUM_OPTIONS)/2;
    double t = 1.0 - std::pow(animTime / animLen, 2);

    const double yo = sh * oldSelection;
    const double yn = sh * selection;

    V4 Old, New;

    if (!dir) {
      Old = V4 { 0, off+yo, (double) w, sh*t };
      New = V4 { 0, off+yn, (double) w, sh*t };
    } else {
      Old = V4 { 0, off+yo+sh-sh*t, (double) w, sh*t+1 };
      New = V4 { 0, off+yn+sh-sh*t, (double) w, sh*t+1 };
    }

    SDL_SetRenderDrawColor(&r, 255, 255, 255, 0);
    SDL_RenderFillRect(&r, New . get());

    SDL_SetRenderDrawColor(&r, 0, 0, 0, 0);
    SDL_RenderFillRect(&r, Old . get());

    if (animTime == animLen) {
      SDL_DestroyTexture(texture[selection]);
      auto s = TTF_RenderText_Solid(font, getText((Option) selection).c_str(), SDL_Color{0,0,0});
      texture[selection] = SDL_CreateTextureFromSurface(&r, s);
      SDL_FreeSurface(s);

      SDL_DestroyTexture(texture[oldSelection]);
      s = TTF_RenderText_Solid(font, getText((Option) oldSelection).c_str(), SDL_Color{255,255,255});
      texture[oldSelection] = SDL_CreateTextureFromSurface(&r, s);
      SDL_FreeSurface(s);
    }

    int sw = widths[selection];
    double x  = (w - sw)/2;

    SDL_RenderSetClipRect (&r, New . get());
    SDL_RenderCopy (&r, texture[selection], nullptr, V4 { x, off+sh*selection, (double) sw, sh } . get());

    sw = widths[oldSelection];
    x  = (w - sw)/2;

    SDL_RenderSetClipRect (&r, Old . get());
    SDL_RenderCopy (&r, texture[oldSelection], nullptr, V4 { x, off+sh*oldSelection, (double) sw, sh } . get());

    SDL_RenderSetClipRect(&r, V4{0,0,(double)w,(double)h}.get());
  }

};


#endif // ifndef MAIN_MENU_H
