#include "Game.h"


Game::~Game()
{
  delete s;
  SDL_DestroyRenderer (rend);
  SDL_DestroyWindow   (wndw);
}

bool Game::tick ()
{
  const auto tick  = std::chrono::system_clock::now();
  const auto delta = std::chrono::duration<double, std::milli>(tick - lastTick).count();

  im.tick();
  Scene * newScene = s->tick(delta, rend, im, nullptr);

  if (newScene == nullptr) {
    SDL_RenderPresent (rend);
  } else {
    s = newScene;
    s->init (rm, rend);
  }

  lastTick = tick;
  if (im.isDown(Quit)) shouldQuit = true;

  return !this->shouldQuit;
}

void Game::loop ()
{
  while (tick ());
}
