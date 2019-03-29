#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL2/SDL.h>
#include <bitset>


enum Input
{ MoveLeft, MoveRight
, Quit
, ReleaseBall
, FireRocket

, NUM_INPUTS
};


class InputManager
{
private:
  using KeyT = unsigned char; // sizeof(KeyT) should be 4, but I'm stingy

  KeyT * keybinds;
  unsigned int  numKeybinds;
  unsigned char active[NUM_INPUTS] = { 0 };
  std::bitset< 256 << (sizeof(KeyT) - 1) > keyDown;

public:
  template <typename... Args>
  InputManager (Args... args)
  {
    numKeybinds = sizeof...(args);
    KeyT kbs[numKeybinds]{ (KeyT) args... };
    keybinds = (KeyT *) malloc (numKeybinds * sizeof (KeyT));
    memcpy (keybinds, kbs, numKeybinds * sizeof (KeyT));
  }


  bool tick (void)
  {
    auto ev  = SDL_Event{};
    bool ret = false;

    while (SDL_PollEvent(&ev)) {
      if (!(ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP)) continue;

      for (int i = 0; i < numKeybinds*2; i+=2) {
        if (ev.key.keysym.sym != keybinds[i]) continue;
        bool down = ev.type == SDL_KEYDOWN;
        if (keyDown.test(ev.key.keysym.sym) == down) continue;
        keyDown.set(ev.key.keysym.sym, down);
        active[keybinds[i+1]] += down? 1 : -1;
        ret = true;
        break;
      }
    }

    return ret;
  }

  void dispose () { free (keybinds); }

  inline bool isDown () { return false; }

  template <typename... Args>
  inline bool isDown (Input i, Args... args)
  {
    return active[i] > 0 || isDown (args...);
  }
};

#endif // INPUTMANAGER_H
