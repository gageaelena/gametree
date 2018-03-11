all: build

build: minimax

minimax: minimax.o
	gcc -Wall minimax.o -o minimax

clean:
	rm -f *.o minimax


