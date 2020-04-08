ssu_score : main.o ssu_score.o blank.o
	gcc main.o ssu_score.o blank.o -o ssu_score

main.o : main.c ssu_score.h
	gcc -c main.c

ssu_score.o : ssu_score.c ssu_score.h blank.h
	gcc -c ssu_score.c

blank.o : blank.c blank.h
	gcc -c blank.c

clean :
	rm *.o
	rm ssu_score
	rm $(PWD)/ANS_DIR/*.csv
	rm $(PWD)/ANS_DIR/*.exe
	rm $(PWD)/ANS_DIR/*.stdout
	rm $(PWD)/ANS_DIR/ssu_*.txt
	rm $(PWD)/*.txt
	rm $(PWD)/STD_DIR/*/*.stdout
	rm $(PWD)/STD_DIR/*/*.stdexe
	rm $(PWD)/score.csv

