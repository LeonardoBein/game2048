#include "Game.cpp"


int main() {
  Game *g2048 = NULL;
  int stop = 0;

  //cria um objeto do jogo 2048, com matriz 4x4
  g2048 = new Game(4,4);

  //loop de execução
  while (1) {

    //menu do jogo
    stop = g2048->menu()->action();

    if (stop){ break; }

  }

  delete g2048;
  return 0;
}
