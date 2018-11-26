#include <vector>
#include <iostream>
#include <curses.h>

class Kernel {

private:
  int _move = 1;
  void addScore(int a) {
    this->_score += a;
    if (this->record < this->_score) {
      this->record = this->_score;
    }
  }


public:
  std::vector<std::vector<int> > table;
  int height;
  int width;
  int null;
  int qtdMoves = 0;
  int _score = 0;
  int record = 0;
  int timeStart= 0;

  //inicia a matriz e zera as variaveis
  Kernel (int w= 4, int h = 4, int null=0){

    if (w != h) {
      std::cout << "Error Interno" << '\n';
      exit(1);
    }

    this->table.resize(w);
    for (int i = 0; i < w; i++) {
      this->table[i] = std::vector<int>(h,0);
    }
    this->height = h;
    this->width = w;
    this->null = null;
    this->qtdMoves = 0;
    this->_score = 0;
    this->record = 0;


  }
  //limpa a matriz, pontos e quantidades de movimentos
  void ClearTable(void){
    for(int i=0; i<this->height; ++i)
  		for(int j=0; j<this->width; ++j)
        this->table[i][j] = this->null;
    this->_score = 0;
    this->qtdMoves = 0;
  }
  //insere um numero na tabela aleatoriamente (90% -> 2, 10% -> 4)
  int newNumber()
  {
    srand(time(NULL)*rand());
    if (!this->_move) {
      return 0;
    }
    int y,x,numero;

  	if (!this->HasEmptySpace())
  		return 0;

    do
    {
      y = rand()%this->height;
      x = rand()%this->width;
    }while(this->table[y][x] != this->null);

    numero = rand()%10;
    if(numero == 0){
      this->table[y][x] = 4;
    }
    else{
      this->table[y][x] = 2;
    }

    return 1;
  }
  // insere vários numeros
  int newNumbers(int loop=0)
  {
    for (int i = 0; i < loop; i++) {
      this->newNumber();
    }
  }
  //pesquisa se possui espaço vazio na matriz
  int HasEmptySpace()
  {
  	int y,x;

  	for(y=0; y<this->height; ++y)
  		for(x=0; x<this->width; ++x)
  			if (this->table[y][x] == this->null)
  				return 1;

  	return 0;
  }
  // pesquisa se na matriz possui adjacentes igual
  int HasAdjacent(void)
  {
  	int y,x, *ultimo_quadradinho;


  	for(y=0; y<this->height; ++y)
  	{
  		ultimo_quadradinho = NULL;

  		for(x=0; x<this->width; ++x)
  			if (this->table[y][x] != this->null)
  			{
  				if(ultimo_quadradinho != NULL && *ultimo_quadradinho == this->table[y][x])
  					return 1;
  				else
  					ultimo_quadradinho = &this->table[y][x];
  			}
  	}

  	for(x=0; x<this->width; ++x)
  	{
  		ultimo_quadradinho = NULL;

  		for(y=0; y<this->height; ++y)
  			if (this->table[y][x] != this->null)
  			{
  				if(ultimo_quadradinho != NULL && *ultimo_quadradinho == this->table[y][x])
  					return 1;
  				else
  					ultimo_quadradinho = &this->table[y][x];
  			}
  	}

  	return 0;
  }
  //função que move os numeros
  Kernel *move(int direction)
  {
  	enum {HORIZONTAL, VERTICAL} sentido;
  	int p, q,x;
  	int *quadradinho, *ultimo_quadradinho;
  	int sorted, incremento;
  	int p_inicial, p_interrupt, q_inicial, q_interrupt, p_incremento, q_incremento, mp_inicial, mp_final,	mq_inicial,mq_final;

  	switch(direction)
  	{
  	case KEY_LEFT:
  		sentido = HORIZONTAL;
  		p_incremento=1;
  		q_incremento=1;
  		p_inicial = 0; p_interrupt = this->height;
  		q_inicial = 0; q_interrupt = this->width;
  		mp_inicial=this->height-1;mp_final=-1;
  		mq_inicial=this->width-1;mq_final=-1;
  		incremento=-1;
  		break;
  	case KEY_RIGHT:
  		sentido = HORIZONTAL;
  		p_incremento=1;
  		q_incremento=-1;
  		p_inicial = 0; p_interrupt = this->height;
  		q_inicial = this->width-1; q_interrupt = 0;
  		mp_inicial=0;mp_final=this->height;
  		mq_inicial=0;mq_final=this->width;
  		incremento=1;
  		break;
  	case KEY_DOWN:
  		sentido = VERTICAL;
  		p_incremento=-1;
  		q_incremento=-1;
  		p_inicial = this->width-1; p_interrupt = -1;
  		q_inicial = this->height-1; q_interrupt = -1;
  		mp_inicial=0;mp_final=this->width;
  		mq_inicial=0;mq_final=this->height;
  		incremento=1;
  		break;
  	case KEY_UP:
  		sentido = VERTICAL;
  		p_incremento=1;
  		q_incremento=1;
  		p_inicial = 0; p_interrupt = this->width;
  		q_inicial = 0; q_interrupt = this->height;
  		mp_inicial=this->width-1;mp_final=-1;
  		mq_inicial=this->height-1;mq_final=-1;
  		incremento=-1;
  		break;
  	default:
      this->_move = 0;
  		return this;
  	}

  	// Junta os quadrinhos
  	for(p=p_inicial; p != p_interrupt; p = p+p_incremento)
  	{
  		ultimo_quadradinho = NULL;

  		for(q=q_inicial; q != q_interrupt; q = q+q_incremento)
  		{
  			if (sentido == HORIZONTAL)
  			quadradinho = &this->table[p][q];
  			else
  			quadradinho = &this->table[q][p];

  			if (*quadradinho == this->null)
  			continue;

  			if(ultimo_quadradinho != NULL && *ultimo_quadradinho == *quadradinho)
  			{
  				*quadradinho = *quadradinho*2;
  				*ultimo_quadradinho = this->null;
  				ultimo_quadradinho = NULL;
          this->addScore(*quadradinho);
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
  				quadradinho = &this->table[p][q];
  				else
  				quadradinho = &this->table[q][p];

  				if (*quadradinho == this->null){
  					if (ultimo_quadradinho != NULL){
  						*quadradinho = *ultimo_quadradinho;
  						*ultimo_quadradinho = this->null;
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

  	if (x == 1) {
  		this->_move = 0;
  	}else{
      this->_move = 1;
      this->qtdMoves++;
    }
  	return this;
  }
  //pesquisa se a matriz possui o numero 2048
  int Has2048(void) {
    for(int i=0; i<this->height; ++i)
  		for(int j=0; j<this->width; ++j)
  			if (this->table[i][j]==2048)
  				return 1;
    return 0;
  }
  //retorna os pontos
  int getScore(void){
    return this->_score;
  }
  //retorna o recorde do jogo
  int getRecord(void){
    return this->record;
  }


};
