CC=clang
CFLAGS=-Wall -O3 
#CFLAGS=-Wall  -g 
LDFLAGS=-W1 -O1 -lm 
#LDFLAGS=-Wall  -lm -g
BIN=lfrac

all:  fractal.o main.o
	$(CC) $(LDFLAGS) main.o fractal.o -o $(BIN)

main.o: main.c fractal_lsystem.h
	$(CC) $(CFLAGS) -c main.c -o main.o

fractal.o: fractal.c fractal_lsystem.h
	$(CC) $(CFLAGS) -c fractal.c -o fractal.o


clean:
	rm -rf *~ *.o $(BIN) *.dat core*
