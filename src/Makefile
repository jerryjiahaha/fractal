CC=clang
CFLAGS=-Wall -O3 
#CFLAGS=-Wall  -g 
LDFLAGS=-W1 -O1 -lm 
#LDFLAGS=-Wall  -lm -g
BIN=lfrac.out

all:  fractal.o main.o str_process.o str_calc.o fractal_lsystem_readfile.o fileread.o
	$(CC) $(LDFLAGS) main.o fractal.o  str_process.o str_calc.o fractal_lsystem_readfile.o fileread.o -o $(BIN)

main.o: main.c fractal_lsystem.h
	$(CC) $(CFLAGS) -c main.c -o main.o

fractal.o: fractal.c fractal_lsystem.h
	$(CC) $(CFLAGS) -c fractal.c -o fractal.o

fileread.o: fileread.c file.h
	$(CC) $(CFLAGS) -c fileread.c -o fileread.o

str_process.o: str_process.c str_process.h
	$(CC) $(CFLAGS) -c str_process.c -o str_process.o
   
str_calc.o: str_calc.c  str_process.h
	$(CC) $(CFLAGS) -c str_calc.c -o str_calc.o

fractal_lsystem_readfile.o: fractal_lsystem_readfile.c fractal_lsystem.h
	$(CC) $(CFLAGS) -c fractal_lsystem_readfile.c -o fractal_lsystem_readfile.o 
	
#fractal_conf.o: str_process.o str_calc.o fractal_lsystem_readfile.o fileread.o
#	$(CC) $(LDFLAGS) str_process.o str_calc.o fractal_lsystem_readfile.o fileread.o -o fractal_conf.o

clean:
	rm -rf *~ *.o $(BIN) *.dat *core*
