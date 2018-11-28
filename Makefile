output: main.o functions.o checagem.o convert.o
	gcc -W main.o functions.o checagem.o convert.o -o exe -lm

main.o: main.c
	gcc -W -c main.c -lm

func.o: functions.c functions.h
	gcc: -W -c functions.c -lm

chec.o: checagem.c checagem.h
	gcc: -W -c checagem.c -lm

conv.o: convert.c convert.h
	gcc: -W -c convert.c -lm