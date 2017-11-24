#include <time.h>
#include <stdlib.h>
#include <curses.h>

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
	COR_1024 = COLOR_BLUE,
	VAZIO = 0, LARGURA = 4, ALTURA = 4};

void jogo(void);
void initCurses(void);
int temEspacoVazio(int grade[ALTURA][LARGURA]);
int temAdjacentes(int grade[ALTURA][LARGURA]);
void colorizar(WINDOW * tela,int valor);
int novoNumero(WINDOW * tela, int grade[ALTURA][LARGURA]);
int desenhar(WINDOW * tela, int grade[ALTURA][LARGURA]);
int mover(int grade[ALTURA][LARGURA], int comando);


int main(void)
{
	int i, escolha=0, entrada;
	char *menu[] = {"Jogo", "Info", "Sair"};
	srand(time(NULL));
	initCurses();

	for(;;)
	{
		clear();
		for(i=0; i<3; ++i)
			if(escolha==i)
			{
				attron(A_BOLD);
				mvprintw(LINES/2+i, COLS/2-2, "%s", menu[i]);
				attroff(A_BOLD);
			}
			else
				mvprintw(LINES/2+i, COLS/2-2, "%s", menu[i]);
		refresh();

		entrada = getch();

		if (entrada == KEY_DOWN)
			escolha = (escolha+1)%3;
		if (entrada == KEY_UP)
			escolha = (escolha-1)%3;
		if (entrada == '\r' || entrada == '\n')
			switch(escolha)
			{
				case 0:
					jogo();
					break;
				case 1:
					clear();
					printw
					(
					"Tente formar o numero 2048 juntando os numeros!\n"
					"Use as setas do teclado para \"jogar\" os numeros e junta-los.\n"
					"Aperte a tecla Espaço a qualquer momento apra sair do jogo!\n"
					"\n"
					"Aperte qualquer tecla para continuar..."
					);
					refresh();
					getch();
					break;
				case 2:
					clear();
					attron(A_BOLD);
					mvprintw(LINES/2,COLS/2-9, "OBRIGADO POR JOGAR!");
					refresh();
					endwin();
					return 0;
					break;
				default: /* Se chegar a cair aqui houve algum erro! */
					break;
			}
	}

	return 0;
}

void jogo(void){
	int y, x;
	int ultimo_comando, comando, venceu;
	int grade[ALTURA][LARGURA];
	WINDOW * tela;

	// Inicialização

	tela = newwin(2*ALTURA+1, 5*LARGURA+1, (LINES-2*ALTURA)/2, (COLS-5*LARGURA)/2);

	for(y=0; y<ALTURA; ++y)
	for(x=0; x<LARGURA; ++x)
	grade[y][x] = VAZIO;

	// Meio do Código
	desenhar(tela, grade);
	novoNumero(tela, grade);
	novoNumero(tela, grade);
	refresh();
	wrefresh(tela);

	venceu = 0;
	comando = 0;
	do
	{
		ultimo_comando = comando;
		comando = getch();

		switch(comando)
		{

			case KEY_DOWN: case KEY_LEFT: case KEY_RIGHT: case KEY_UP:
				//if (comando==ultimo_comando)
				//	continue;
				venceu = mover(grade, comando);
				desenhar(tela, grade);
				// nao moveu
				if (venceu == 2) {
					venceu = 0;
				}
				else{
					novoNumero(tela, grade);
				}
				refresh();
				wrefresh(tela);
				break;
			case ' ':
				clear();
				delwin(tela);
				endwin();
				return;

			default:
				break;
		}

	}while(!venceu && (temEspacoVazio(grade) || temAdjacentes(grade)));



	if (venceu == 1)
		mvprintw(LINES/2, COLS/2-4, "VITORIA!");
	else
		mvprintw(LINES/2, COLS/2-4, "GAME-OVER!");



}
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

void colorizar(WINDOW * tela, int valor)
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

int temEspacoVazio(int grade[ALTURA][LARGURA])
{
	int y,x;

	for(y=0; y<ALTURA; ++y)
		for(x=0; x<LARGURA; ++x)
			if (grade[y][x] == VAZIO)
				return 1;


	return 0;
}

int temAdjacentes(int grade[ALTURA][LARGURA])
{
	int y,x, *ultimo_quadradinho;


	for(y=0; y<ALTURA; ++y)
	{
		ultimo_quadradinho = NULL;

		for(x=0; x<LARGURA; ++x)
			if (grade[y][x] != VAZIO)
			{
				if(ultimo_quadradinho != NULL && *ultimo_quadradinho == grade[y][x])
					return 1;
				else
					ultimo_quadradinho = &grade[y][x];
			}
	}

	for(x=0; x<LARGURA; ++x)
	{
		ultimo_quadradinho = NULL;

		for(y=0; y<ALTURA; ++y)
			if (grade[y][x] != VAZIO)
			{
				if(ultimo_quadradinho != NULL && *ultimo_quadradinho == grade[y][x])
					return 1;
				else
					ultimo_quadradinho = &grade[y][x];
			}
	}

	return 0;
}

int novoNumero(WINDOW * tela, int grade[ALTURA][LARGURA])
{
	int y,x,numero;

	if (!temEspacoVazio(grade))
		return 0;

	do
	{
		y = rand()%ALTURA;
		x = rand()%LARGURA;
	}while(grade[y][x] != VAZIO);

	numero = rand()%10;
	if(numero == 0)
		grade[y][x] = 4;
	else
		grade[y][x] = 2;

	colorizar(tela, grade[y][x]);
	wattron(tela, TERMINAL_NOVONUMERO);
	wmove(tela, 2*y+1, 5*x+1);
	wprintw(tela, "%4d", grade[y][x]);
	wattroff(tela, TERMINAL_NOVONUMERO);
	colorizar(tela, 0);
	return 1;
}

int desenhar(WINDOW * tela, int grade[ALTURA][LARGURA])
{
	int y, x;

	clear();
	wattron(tela, TERMINAL_GRADE);

	/* Caixa ao redor */
	wborder(tela, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

	/* Linhas da grade */
	for (y=1; y<ALTURA; ++y)
	mvwhline(tela, 2*y, 1, ACS_HLINE, 5*LARGURA);
	for (x=1; x<LARGURA; ++x)
	mvwvline(tela, 1, 5*x, ACS_VLINE, 2*ALTURA);

	/* Junções entre linhas */
	for (y=0; y<ALTURA+1; ++y)
	for (x=0; x<LARGURA+1; ++x)
	{
		wmove(tela, 2*y, 5*x);
		if (x == 0)
		waddch(tela, ACS_LTEE);
		else if(x == LARGURA)
		waddch(tela, ACS_RTEE);
		else if (y == 0)
		waddch(tela, ACS_TTEE);
		else if (y == ALTURA)
		waddch(tela, ACS_BTEE);
		else
		waddch(tela, ACS_PLUS);
	}

	/* Cantos */
	mvwaddch(tela, 0, 0, ACS_ULCORNER);
	mvwaddch(tela, 0, 5*LARGURA, ACS_URCORNER);
	mvwaddch(tela, 2*ALTURA, 5*LARGURA, ACS_LRCORNER);
	mvwaddch(tela, 2*ALTURA, 0, ACS_LLCORNER);

	wattroff(tela, TERMINAL_GRADE);

	/* Desenha os numeros */
	wattron(tela, TERMINAL_NUMEROS);
	for (y=0; y<ALTURA; ++y)
	{
		for (x=0; x<LARGURA; ++x)
		{
			wmove(tela, 2*y+1, 5*x+1);
			colorizar(tela, grade[y][x]);
			if(grade[y][x] == VAZIO)
				wprintw(tela, "    ");
			else
				wprintw(tela, "%4d", grade[y][x]);
			colorizar(tela, 0);
		}
	}
	wattroff(tela, TERMINAL_NUMEROS);

	return 1;
}

int mover(int grade[ALTURA][LARGURA], int comando)
{
	enum {HORIZONTAL, VERTICAL} sentido;
	int p, q,x;
	int *quadradinho, *ultimo_quadradinho;
	int sorted, incremento;
	int p_inicial, p_interrupt, q_inicial, q_interrupt, p_incremento, q_incremento, mp_inicial, mp_final,	mq_inicial,mq_final;

	switch(comando)
	{
	case KEY_LEFT:
		sentido = HORIZONTAL;
		p_incremento=1;
		q_incremento=1;
		p_inicial = 0; p_interrupt = ALTURA;
		q_inicial = 0; q_interrupt = LARGURA;
		mp_inicial=ALTURA-1;mp_final=-1;
		mq_inicial=LARGURA-1;mq_final=-1;
		incremento=-1;
		break;
	case KEY_RIGHT:
		sentido = HORIZONTAL;
		p_incremento=1;
		q_incremento=-1;
		p_inicial = 0; p_interrupt = ALTURA;
		q_inicial = LARGURA-1; q_interrupt = 0;
		mp_inicial=0;mp_final=ALTURA;
		mq_inicial=0;mq_final=LARGURA;
		incremento=1;
		break;
	case KEY_DOWN:
		sentido = VERTICAL;
		p_incremento=-1;
		q_incremento=-1;
		p_inicial = LARGURA-1; p_interrupt = -1;
		q_inicial = ALTURA-1; q_interrupt = -1;
		mp_inicial=0;mp_final=LARGURA;
		mq_inicial=0;mq_final=ALTURA;
		incremento=1;
		break;
	case KEY_UP:
		sentido = VERTICAL;
		p_incremento=1;
		q_incremento=1;
		p_inicial = 0; p_interrupt = LARGURA;
		q_inicial = 0; q_interrupt = ALTURA;
		mp_inicial=LARGURA-1;mp_final=-1;
		mq_inicial=ALTURA-1;mq_final=-1;
		incremento=-1;
		break;
	default:
		return 0;
	}

	// Junta os quadrinhos
	for(p=p_inicial; p != p_interrupt; p = p+p_incremento)
	{
		ultimo_quadradinho = NULL;

		for(q=q_inicial; q != q_interrupt; q = q+q_incremento)
		{
			if (sentido == HORIZONTAL)
			quadradinho = &grade[p][q];
			else
			quadradinho = &grade[q][p];

			if (*quadradinho == VAZIO)
			continue;

			if(ultimo_quadradinho != NULL && *ultimo_quadradinho == *quadradinho)
			{
				*quadradinho = *quadradinho*2;
				*ultimo_quadradinho = VAZIO;
				ultimo_quadradinho = NULL;
			}
			else
			ultimo_quadradinho = quadradinho;
		}
	}

	x=0;
	do{
		sorted = 1;
		for(p=mp_inicial; p != mp_final; p = p+incremento){
			ultimo_quadradinho = NULL;
			for(q=mq_inicial; q != mq_final; q = q+incremento){
				if (sentido == HORIZONTAL)
				quadradinho = &grade[p][q];
				else
				quadradinho = &grade[q][p];

				if (*quadradinho == VAZIO){
					if (ultimo_quadradinho != NULL){
						*quadradinho = *ultimo_quadradinho;
						*ultimo_quadradinho = VAZIO;
						ultimo_quadradinho = quadradinho;
						sorted = 0;
					}
				}
				else
				ultimo_quadradinho=quadradinho;
 			}
		}
		x++;
	} while(!sorted);

	for(p=0; p<ALTURA; ++p)
		for(q=0; q<LARGURA; ++q)
			if (grade[p][q]==2048)
				return 1;

	if (x == 1) {
		return 2;
	}
	return 0;
}

