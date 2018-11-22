############################# Makefile ##########################
all: game clean

game: main.o Kernel.o Controller.o View.o
	g++ -o game main.o Kernel.o Controller.o View.o -lncurses
main.o: Controller.cpp
	g++ -c main.cpp -lncurses
Kernel.o:
	g++ -c Kernel.cpp -lncurses
Controller.o: View.cpp Kernel.cpp
	g++ -c Controller.cpp -lncurses
View.o:
	g++ -c View.cpp -lncurses
clean:
	rm -rf *.o
