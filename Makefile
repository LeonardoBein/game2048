############################# Makefile ##########################
all: game clean

game: main.o Kernel.o Game.o View.o
	g++ -o game main.o Kernel.o Game.o View.o -lncurses
main.o: Game.cpp
	g++ -c main.cpp -lncurses
Kernel.o:
	g++ -c Kernel.cpp -lncurses
Controller.o: View.cpp Kernel.cpp
	g++ -c Game.cpp -lncurses
View.o:
	g++ -c View.cpp -lncurses
clean:
	rm -rf *.o
