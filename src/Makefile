CC = gcc
FLAGS =  -Wall -g
PROGS = ChaineMain ReconstitueReseau
all : $(PROGS)

ReconstitueReseau : ReconstitueReseau.o Reseau.o Chaine.o SVGwriter.o ArbreQaut.o
	$(CC) $(FLAGS) -o $@ $^ -lm

ReconstitueReseau.o : ReconstitueReseau.c commun.h Reseau.h
	$(CC) $(FLAGS) -c $<

ChaineMain : Chaine.o ChaineMain.o SVGwriter.o
	$(CC) $(FLAGS) -o $@ $^ -lm

ChaineMain.o : ChaineMain.c Chaine.h commun.h
	$(CC) $(FLAGS) -c $<

Chaine.o : Chaine.c SVGwriter.h Chaine.h commun.h
	$(CC) $(FLAGS) -c $<

SVGwriter.o : SVGwriter.c SVGwriter.h 
	$(CC) $(FLAGS) -c $<

Reseau.o : Reseau.c Chaine.h commun.h Reseau.h
	$(CC) $(FLAGS) -c $<

ArbreQaut.o : ArbreQaut.c ArbreQaut.h commun.h Reseau.h chaine.h 
	$(CC) $(FLAGS) -c $<

clean:
	rm -f *.o $(PROGS) *.html *.log