#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Game.h"
#include "InputManager.h"
#include "Breakout.h"
#include "MainMenu.h"


inline void init ()
{
  SDL_Init (SDL_INIT_VIDEO);
  IMG_Init (IMG_INIT_PNG);
  TTF_Init ();

  std::srand (std::time(nullptr));
}


int main ([[maybe_unused]] int argc, [[maybe_unused]] const char *argv[])
{
  init ();

  InputManager im = InputManager
    ( SDLK_h      , MoveLeft
    , SDLK_l      , MoveRight
    , SDLK_j      , MoveDown
    , SDLK_k      , MoveUp

    , SDLK_SPACE  , ReleaseBall
    , SDLK_f      , FireRocket
    , SDLK_d      , PowerMagnet

    , SDLK_ESCAPE , Quit
    );

  Game g { 1000, 1000, im };
  g.setScene<MainMenu>();
  g.loop();
}
