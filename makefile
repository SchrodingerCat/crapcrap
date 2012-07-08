all: functions.o
	        gcc -o laby laby.c functions.o -Wall -ggdb -lpthread

functions.o: functions.c
	        gcc -c functions.c -Wall -ggdb -lpthread
clean: 
	        rm -f *.o laby

