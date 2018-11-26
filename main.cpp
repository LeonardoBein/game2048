#include "Game.cpp"


int main(int argc, char const *argv[]) {
  Game *g2048 = NULL;
  int stop = 0;

  g2048 = new Game(4,4);

  while (1) {
    stop = g2048->menu()->action();

    if (stop){ break; }

  }

  delete g2048;
  return 0;
}
