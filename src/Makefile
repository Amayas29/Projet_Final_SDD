CC = gcc
FLAGS =  -Wall -g
PROGS = ChaineMain ReconstitueReseau
all : $(PROGS)

ReconstitueReseau : ReconstitueReseau.o Noeud.o Reseau.o Hachage.o Chaine.o SVGwriter.o ArbreQuat.o
	$(CC) $(FLAGS) -o $@ $^ -lm

ReconstitueReseau.o : ReconstitueReseau.c commun.h Reseau.h Noeud.h
	$(CC) $(FLAGS) -c $<

ChaineMain : Chaine.o ChaineMain.o SVGwriter.o
	$(CC) $(FLAGS) -o $@ $^ -lm

ChaineMain.o : ChaineMain.c Chaine.h commun.h
	$(CC) $(FLAGS) -c $<

Chaine.o : Chaine.c SVGwriter.h Chaine.h commun.h
	$(CC) $(FLAGS) -c $<

SVGwriter.o : SVGwriter.c SVGwriter.h 
	$(CC) $(FLAGS) -c $<

Reseau.o : Reseau.c Chaine.h commun.h Reseau.h Noeud.h Hachage.h
	$(CC) $(FLAGS) -c $<

ArbreQuat.o : ArbreQuat.c ArbreQuat.h commun.h Reseau.h Chaine.h Noeud.h
	$(CC) $(FLAGS) -c $<

Noeud.o : Noeud.c Noeud.h commun.h
	$(CC) $(FLAGS) -c $<

Hachage.o : Hachage.c Hachage.h Noeud.h commun.h
	$(CC) $(FLAGS) -c $<

clean:
	rm -f *.o $(PROGS) *.html *.log