CC =  g++

all: prodcon

prodcon: main.o 
	$(CC) main.o -pthread -o prodcon

main.o: main.cpp main.h
	$(CC) -c main.cpp -O -pthread -o main.o

clean:
	rm -f main.o prodcon