all: start p1 p2 p3 p4 p5 p6

start: start.c
	gcc -g start.c -o start

p1: p1.c p.h
	gcc -g p1.c -o p1

p2: p2.c p.h
	gcc -g p2.c -o p2

p3: p3.c p.h
	gcc -g p3.c -o p3

p4: p4.c p.h
	gcc -g p4.c -o p4

p5: p5.c p.h
	gcc -g p5.c -o p5

p6: p6.c p.h
	gcc -g p6.c -o p6

clean:
	rm -f p1 p2 p3 p4 p5 p6 start
	ipcs -q | grep "0x" | awk '{ system("ipcrm -q" $$2) }'
