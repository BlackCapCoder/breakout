#include "Game.h"

int main (int argc, const char *argv[]) {
  Game g {1000, 1000, nullptr};
  // while (g.tick());
  g.dispose();
  return 0;
}
