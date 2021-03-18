CC = gcc
FLAGS =  -Wall -g
PROGS = ChaineMain
all : $(PROGS)

ChaineMain : ChaineMain.o Chaine.o SVGwriter.o
	$(CC) $(FLAGS) -o $@ $^

ChaineMain.o : ChaineMain.c Chaine.h commun.h
	$(CC) $(FLAGS) -c $<

Chaine.o : Chaine.c  SVGwriter.h Chaine.h commun.h 
	$(CC) $(FLAGS) -c $<

SVGwriter.o : SVGwriter.c  SVGwriter.h 
	$(CC) $(FLAGS) -c $<

clean:
	rm -f *.o $(PROGS) *.html