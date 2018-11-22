#include <iostream>
#include "Kernel.cpp"
#include "View.cpp"

class Controller: public Kernel,public View {
private:
  int route =0 ;
public:
  Controller (void){ }
  
  Controller *menu(void) {
    int action = 0,escolha=0;

    for (;;) {
      this->ViewMenu(escolha);
      action = getch();
      if (action == KEY_DOWN)
  			escolha = (escolha+1)%this->SizeMenuOptions;
  		if (action == KEY_UP)
  			escolha = (escolha-1)%this->SizeMenuOptions;
  		if (action == '\r' || action == '\n'){
        this->route = escolha;
        return this;
      }
    }
  }

  int start(void) {
    std::cout << "GAME" << '\n';
    int action = 0, add=0;
    this->ClearTable();
    this->newNumbers(2);
    do {
      this->ViewGame(this->height,this->width,this->table);
      action = getch();
      if (action == 'q' || action == ' ') {
        return 0;
      }
      this->move(action)->newNumber();

    } while(!this->Has2048() && (this->HasEmptySpace() || this->HasAdjacent()));

    this->ViewWin(this->Has2048());
    while (action != ' ' || action != 'q') {
      action = getch();
    }

  }
  int info(void) {
    this->ViewInfo();
    getch();
  }
  int quit(void) {
    return 1;
  }
  int GetRoute(void) {
    return this->route;
  }

  int action(void) {
    switch (this->route) {
      case 0:
        this->start();
        break;
      case 1:
        this->info();
        break;
      case 2:
        return this->quit();
        break;
      default:
        break;
    }
    return 0;
  }
};
