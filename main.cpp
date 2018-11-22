#include "Controller.cpp"


int main(int argc, char const *argv[]) {
  Controller *controller = NULL;
  int stop = 0;

  controller = new Controller();

  while (1) {
    stop = controller->menu()->action();

    if (stop){ break; }

  }

  delete controller;
  return 0;
}
