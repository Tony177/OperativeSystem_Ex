all: start

start: main.o procedure.o
	gcc -pthread -o start main.o procedure.o

procedure.o: procedure.c header.h
	gcc -c procedure.c

main.o: main.c header.h
	gcc -c main.c

clean: 
	rm -f *.o
	rm -f *~
	rm -f ./start
