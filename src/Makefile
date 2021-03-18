CC = gcc
FLAGS =  -Wall -g
all : Chaine_main

Chaine_main : Chaine_main.o Chaine.o SVGwriter.o
	$(CC) $(FLAGS) -o $@ $^

Chaine_main.o : Chaine_main.c Chaine.h commun.h
	$(CC) $(FLAGS) -c $<

Chaine.o : Chaine.c  SVGwriter.h Chaine.h commun.h 
	$(CC) $(FLAGS) -c $<

SVGwriter.o : SVGwriter.c  SVGwriter.h 
	$(CC) $(FLAGS) -c $<

clean:
	rm -f *.o Chaine_main