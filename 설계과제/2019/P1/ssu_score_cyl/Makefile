RESULT_OBJECTS = main.o support.o blank.o program.o

ssu_score : $(RESULT_OBJECTS) 
	gcc -o ssu_score $(RESULT_OBJECTS) -g -lpthread
	make clean

main.o : ssu_score.h main.c
	gcc -c main.c -g

support.o : ssu_score.h support.c
	gcc -c support.c -g

blank.o : ssu_score.h blank.c
	gcc -c blank.c -g

program.o : ssu_score.h program.c
	gcc -c program.c -g

clean :
	rm $(RESULT_OBJECTS)

test1 :
	./ssu_score STD_DIR ANS_DIR -h -p -e 20190001 20190002 -c 20190001 -t 12 13

test2 :
	./ssu_score STD_DIR ANS_DIR -h -p -e 20190001 20190002 -c 20190001
