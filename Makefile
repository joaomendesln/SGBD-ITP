output: main.o functions.o
	gcc -W main.o functions.o -o exe -lm

main.o: main.c
	gcc -W -c main.c -lm

func.o: functions.c functions.h
	gcc: -W -c functions.c -lm