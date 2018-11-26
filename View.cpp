#include <curses.h>
#include <iostream>
#include <vector>

enum CONFIGS
{
  TERMINAL_NOVONUMERO = A_BOLD,
  TERMINAL_GRADE = A_DIM,
  TERMINAL_NUMEROS = A_NORMAL,
  COR_FUNDO = COLOR_BLACK,
  COR_2 = COLOR_RED,
  COR_4 = COLOR_RED,
  COR_8 = COLOR_YELLOW,
  COR_16 = COLOR_YELLOW,
  COR_32 = COLOR_GREEN,
  COR_64 = COLOR_GREEN,
  COR_128 = COLOR_CYAN,
  COR_256 = COLOR_CYAN,
  COR_2048 = COLOR_WHITE,
  COR_512 = COLOR_BLUE,
  COR_1024 = COLOR_BLUE
};


class View {

private:
  const char *menu[3] = {"Jogo", "Info", "Sair"};
  WINDOW * windows = NULL;

  void initCurses(void)
  {
  	initscr();
  	cbreak();
  	noecho();
  	curs_set(0);
  	keypad(stdscr, TRUE);

  	if (has_colors() == TRUE)
  	{
  		start_color();
  		init_pair(1, COR_2, COR_FUNDO);
  		init_pair(2, COR_4, COR_FUNDO);
  		init_pair(3, COR_8, COR_FUNDO);
  		init_pair(4, COR_16, COR_FUNDO);
  		init_pair(5, COR_32, COR_FUNDO);
  		init_pair(6, COR_64, COR_FUNDO);
  		init_pair(7, COR_128, COR_FUNDO);
  		init_pair(8, COR_256, COR_FUNDO);
  		init_pair(9, COR_512, COR_FUNDO);
  		init_pair(10, COR_1024, COR_FUNDO);
  		init_pair(11, COR_2048, COR_FUNDO);
  	}
  }
  void colorizar(WINDOW * tela,int valor)
  {
  	int log;
  	static int ultimo=0;

  	if (has_colors() == FALSE)
  		return;

  	if(valor>0)
  	{
  		for(log=0; valor != 1; ++log)
  			valor = valor/2;
  		wattron(tela, COLOR_PAIR(log));
  		ultimo=log;
  	}else if(ultimo>0)
  	{
  		wattroff(tela, COLOR_PAIR(ultimo));
  		ultimo = 0;
  	}
  }

  WINDOW *GetWindow(int height,int width) {
    if (!this->windows) {
      return newwin(2*height+1, 5*width+1, (LINES-2*height)/2, (COLS-5*width)/2);
    }
    return this->windows;
  }

public:
  int SizeMenuOptions = 3;

  View (void){
    this->initCurses();

  }

  void ViewMenu(int escolha=0) {
    clear();
		for(int i=0; i<3; ++i){
			if(escolha==i)
			{
				attron(A_BOLD);
				mvprintw(LINES/2+i, COLS/2-2, "%s",  this->menu[i]);
				attroff(A_BOLD);
			}
			else
				mvprintw(LINES/2+i, COLS/2-2, "%s", this->menu[i]);
    }
		refresh();
  }
  View *ViewGame(int height,int width, std::vector<std::vector<int>> table,int null=0) {
    WINDOW * tela = this->GetWindow(height,width);
    int x,y;
    clear();
  	wattron(tela, TERMINAL_GRADE);

  	/* Caixa ao redor */
  	wborder(tela, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

  	/* Linhas da grade */
  	for (y=1; y<height; ++y)
  	mvwhline(tela, 2*y, 1, ACS_HLINE, 5*height);
  	for (x=1; x<width; ++x)
  	mvwvline(tela, 1, 5*x, ACS_VLINE, 2*width);

  	/* Junções entre linhas */
  	for (y=0; y<height+1; ++y)
  	for (x=0; x<width+1; ++x)
  	{
  		wmove(tela, 2*y, 5*x);
  		if (x == 0)
  		waddch(tela, ACS_LTEE);
  		else if(x == width)
  		waddch(tela, ACS_RTEE);
  		else if (y == 0)
  		waddch(tela, ACS_TTEE);
  		else if (y == height)
  		waddch(tela, ACS_BTEE);
  		else
  		waddch(tela, ACS_PLUS);
  	}

  	/* Cantos */
  	mvwaddch(tela, 0, 0, ACS_ULCORNER);
  	mvwaddch(tela, 0, 5*width, ACS_URCORNER);
  	mvwaddch(tela, 2*height, 5*width, ACS_LRCORNER);
  	mvwaddch(tela, 2*height, 0, ACS_LLCORNER);

  	wattroff(tela, TERMINAL_GRADE);

  	/* Desenha os numeros */
  	wattron(tela, TERMINAL_NUMEROS);
  	for (y=0; y<height; ++y)
  	{
  		for (x=0; x<width; ++x)
  		{
  			wmove(tela, 2*y+1, 5*x+1);
  			this->colorizar(tela,table[y][x]);
  			if(table[y][x] == null)
  				wprintw(tela, "    ");
  			else
  				wprintw(tela, "%4d", table[y][x]);
          this->colorizar(tela,0);
  		}
  	}
  	wattroff(tela, TERMINAL_NUMEROS);
    refresh();
    wrefresh(tela);
    return this;
  }
  View *PrintMoves(int moves) {
    mvprintw(LINES/2,0, "Moves:   %4d", moves);
    return this;
  }
  View *PrintScore(int score) {
    mvprintw(LINES/2+1,0, "Pontos:  %4d", score);
    return this;
  }
  View *PrintRecord(int record) {
    mvprintw(LINES/2+2,0, "Recorde: %4d", record);
    return this;
  }
  void Render(void){
    refresh();
  }
  void ViewWin(int victory) {
    if (victory) {
      mvprintw(LINES/2, COLS/2-4, "VITORIA!");
    }
  	else{
      mvprintw(LINES/2, COLS/2-4, "GAME-OVER!");
    }
    refresh();
  }
  void ViewInfo(void) {
    clear();
    mvprintw(LINES/2,0,
    "Tente formar o numero 2048 juntando os numeros!\n"
    "Use as setas do teclado para \"jogar\" os numeros e junta-los.\n"
    "Aperte a tecla Espaço a qualquer momento apra sair do jogo!\n"
    "\n"
    "Aperte qualquer tecla para continuar...");
    refresh();
  }


  ~View (){
    endwin();
  }
};
