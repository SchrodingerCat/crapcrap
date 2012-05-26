all: functions.o
	        gcc -o laby laby.c functions.o -Wall

functions.o: functions.c
	        gcc -c functions.c -Wall
clean: 
	        rm -f *.o laby

