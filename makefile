all: functions.o
	        gcc -o laby laby.c functions.o -Wall -ggdb

functions.o: functions.c
	        gcc -c functions.c -Wall -ggdb
clean: 
	        rm -f *.o laby

