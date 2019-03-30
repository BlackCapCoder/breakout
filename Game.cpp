#include "Game.h"


void Game::init ()
{
  SDL_Init (SDL_INIT_VIDEO);

  if ( SDL_CreateWindowAndRenderer
        ( w
        , h
        , SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
        , &wndw
        , &rend )
      ) exit (EXIT_FAILURE);

  if (!IMG_Init(IMG_INIT_PNG))
    exit (EXIT_FAILURE);

  std::srand(std::time(nullptr));
}

bool Game::tick ()
{
  auto tick  = std::chrono::system_clock::now();
  auto delta = std::chrono::duration<double, std::milli>(tick - lastTick).count();

  if (1000 / fps <= delta) {
    im->tick();

    SDL_SetRenderDrawColor (rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);
    (void) s->tick(delta, rend, im, nullptr);
    SDL_RenderPresent (rend);

    lastTick = tick;
    if (im->isDown(Quit)) shouldQuit = true;
  } else {
    std::this_thread::sleep_for
      ( std::chrono::duration<double, std::milli>
        (1000 / fps - delta)
      );
  }

  return !this->shouldQuit;
}

void Game::dispose ()
{
  im->dispose         ();
  SDL_DestroyRenderer (rend);
  SDL_DestroyWindow   (wndw);
}

SDL_Renderer* Game::getRenderer() const
{
  return rend;
}
