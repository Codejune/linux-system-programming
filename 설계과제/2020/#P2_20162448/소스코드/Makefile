ssu_mntr: ssu_mntr.o mntr_process.o main.o
	gcc -o ssu_mntr ssu_mntr.o mntr_process.o main.o

ssu_mntr.o: ssu_mntr.c ssu_mntr.h
	gcc -c ssu_mntr.c

mntr_process.o : mntr_process.c ssu_mntr.h
	gcc -c mntr_process.c

main.o: main.c ssu_mntr.h
	gcc -c main.c

clean:
	rm *.o
	rm ssu_mntr
	rm log.txt
