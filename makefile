all: functions.o ia.o
		gcc -o laby laby.c functions.o ia.o -Wall -ggdb -lpthread

functions.o: functions.c
		gcc -c functions.c -Wall -ggdb -lpthread

ia.o: ia.c
		gcc -c ia.c -Wall -ggdb -lpthread

clean: 
		rm -f *.o laby

