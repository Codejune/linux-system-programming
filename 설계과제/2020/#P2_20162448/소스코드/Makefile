ssu_mntr: ssu_mntr.o main.o
	gcc -o ssu_mntr ssu_mntr.o main.o

ssu_mntr.o: ssu_mntr.c ssu_mntr.h
	gcc -c ssu_mntr.c

main.o: main.c ssu_mntr.h
	gcc -c main.c

clean:
	rm *.o
	rm ssu_mntr
