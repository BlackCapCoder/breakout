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
  , Quit

  , NUM_OPTIONS
  };

private:
  const char keyRepeat = 6;
  int selection = StartGame;
  int w, h;
  int cnt;
  Game * game;

public:
  MainMenu (int w, int h, Game * g) : w{w}, h{h}, game{g} {}

  void logic
    ( double         dt // Milliseconds since last tick
    , InputManager * im
    , void         * st )
  {
    if (im->isDown(MoveUp)) {
      if (cnt % keyRepeat == 0)
        selection = (NUM_OPTIONS + selection - 1) % NUM_OPTIONS;
      cnt++;
    } else if (im->isDown(MoveDown)) {
      if (cnt % keyRepeat == 0)
        selection = (selection + 1) % NUM_OPTIONS;
      cnt++;
    } else cnt = 0;

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

  void render (SDL_Renderer * r)
  {
    const double sw = w * 0.96;
    const double sh = h * 0.1;
    const double sp = h * 0.02;

    for (int i = 0; i < NUM_OPTIONS; i++) {
      const double x = (w - sw)/2;
      const double y = sp * (i+1) + sh * i;

      if (i == selection) {
        SDL_SetRenderDrawColor (r, 128, 128, 128, 1);
      } else {
        SDL_SetRenderDrawColor (r, 64, 64, 64, 1);
      }

      SDL_RenderFillRect(r, V4 {x, y, sw, sh} . get());
    }
  }

};


#endif // ifndef MAIN_MENU_H
