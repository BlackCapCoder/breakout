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

#define MenuSelect FireRocket, ReleaseBall

, NUM_INPUTS
};



class InputManager
{
private:
  using KeyT = unsigned char; // sizeof(KeyT) should be 4, but I'm stingy

  const unsigned int numKeybinds;
  const KeyT * keybinds;
  unsigned char active[NUM_INPUTS]{};
  std::bitset<256 << (sizeof(KeyT) - 1)> keyDown;

public:
  template <typename... Args>
  InputManager (const Args... args)
    : numKeybinds { sizeof...(args) }
    , keybinds    { new KeyT [numKeybinds] {(KeyT) args...} }
  {
  }

  ~InputManager (void)
  {
    delete keybinds;
  }


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

  inline bool isDown () const
  {
    return false;
  }

  template <typename... Args>
  inline bool isDown (Input i, Args... args) const
  {
    return active[i] > 0 || isDown (args...);
  }

  inline bool isDownFirst () const
  {
    return false;
  }

  template <typename... Args>
  inline bool isDownFirst (Input i, Args... args) const
  {
    return active[i] == 255 || isDownFirst (args...);
  }
};

#endif // INPUTMANAGER_H
