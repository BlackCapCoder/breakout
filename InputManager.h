#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL2/SDL.h>
#include <bitset>


enum Input
{ MoveLeft, MoveRight, MoveUp, MoveDown
, Quit
, ReleaseBall
, FireRocket
, PowerMagnet

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


  int cnt=0;
  bool tick (void)
  {
    auto ev  = SDL_Event{};
    bool ret = false;

    for (int i = 0; i < numKeybinds*2; i+=2)
      if (active[keybinds[i+1]] == 255)
        active[keybinds[i+1]] = 1;

    while (SDL_PollEvent(&ev)) {
      if (!(ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP)) continue;

      for (int i = 0; i < numKeybinds*2; i+=2) {
        if (ev.key.keysym.sym != keybinds[i]) continue;
        const bool down  = ev.type == SDL_KEYDOWN;
        const bool check = keyDown.test(ev.key.keysym.sym);

        if (check == down) continue;
        keyDown.set(ev.key.keysym.sym, down);

        const unsigned char a = active[keybinds[i+1]];

        if (a == 255)
          active[keybinds[i+1]] = 1;

        if (a == 0)
          active[keybinds[i+1]] = 255;
        else if (down)
          active[keybinds[i+1]]++;
        else
          active[keybinds[i+1]]--;

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


  inline bool isDownFirst () { return false; }

  template <typename... Args>
  inline bool isDownFirst (Input i, Args... args)
  {
    return active[i] == 255 || isDown (args...);
  }
};

#endif // INPUTMANAGER_H
