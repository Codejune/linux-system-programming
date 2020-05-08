ssu_mntr: support.o monitoring.o prompt.o ssu_mntr.o common.h monitoring.h prompt.h
	gcc -o ssu_mntr ssu_mntr.o 
	gcc -o monitoring monitoring.o support.o 
	gcc -o prompt prompt.o support.o

support.o: support.c common.h
	gcc -c support.c

monitoring.o: monitoring.c common.h monitoring.h
	gcc -c monitoring.c

prompt.o : prompt.c common.h prompt.h
	gcc -c prompt.c

ssu_mntr.o: ssu_mntr.c common.h
	gcc -c ssu_mntr.c

clean:
	rm *.o
	rm monitoring
	rm ssu_mntr
	rm prompt
	rm log.txt
