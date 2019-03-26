#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL2/SDL.h>

enum Input
{ MoveLeft, MoveRight
, Quit

, NUM_ITEMS
};


class InputManager {
  private:
    int  * keybinds;
    bool * keyDown;
    int    numKeybinds;
    char   active[NUM_ITEMS] = { 0 };

  public:
    InputManager (int * keybinds = {}, int numKeybinds = 0) {
      this->keybinds    = keybinds;
      this->numKeybinds = numKeybinds;
      keyDown = (bool *) malloc (numKeybinds);
    }

    bool Tick (void) {
      auto ev  = SDL_Event{};
      bool ret = false;

      while (SDL_PollEvent(&ev)) {
        if (!(ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP)) continue;

        for (int i = 0; i < numKeybinds*2; i+=2) {
          if (ev.key.keysym.sym != keybinds[i]) continue;
          bool down = ev.type == SDL_KEYDOWN;
          if (keyDown[ev.key.keysym.sym] == down) continue;
          keyDown[ev.key.keysym.sym] = down;
          active[keybinds[i+1]] += down? 1 : -1;
          ret = true;
          break;
        }
      }

      return ret;
    }

    void dispose () {
      free (this->keyDown);
    }

    inline bool isDown () { return false; }

    template <typename... Args>
    inline bool isDown (Input i, Args... args) {
      return active[i] > 0 || isDown (args...);
    }
};

#endif // INPUTMANAGER_H
