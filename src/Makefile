CC = gcc
FLAGS =  -Wall -g
PROGS = ChaineMain ReconstitueReseau Main Main_alea test_graphe

all : $(PROGS)

ReconstitueReseau : ReconstitueReseau.o Noeud.o Reseau.o Hachage.o Chaine.o SVGwriter.o ArbreQuat.o
	$(CC) $(FLAGS) -o $@ $^ -lm

Main : main.o Noeud.o Reseau.o Hachage.o Chaine.o SVGwriter.o ArbreQuat.o
	$(CC) $(FLAGS) -o $@ $^ -lm

Main_alea : main_alea.o Noeud.o Reseau.o Hachage.o Chaine.o SVGwriter.o ArbreQuat.o
	$(CC) $(FLAGS) -o $@ $^ -lm

ReconstitueReseau.o : ReconstitueReseau.c *.h
	$(CC) $(FLAGS) -c $<

ChaineMain : Chaine.o ChaineMain.o SVGwriter.o
	$(CC) $(FLAGS) -o $@ $^ -lm

test_graphe : Struct_File.o Struct_Liste.o Graphe.o test_graphe.o Noeud.o Reseau.o Hachage.o Chaine.o SVGwriter.o ArbreQuat.o
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

main.o: main.c *.h
	$(CC) $(FLAGS) -c $<

main_alea.o: main_alea.c *.h
	$(CC) $(FLAGS) -c $<

Graphe.o : Graphe.c *.h
	$(CC) $(FLAGS) -c $<

Struct_File.o : Struct_File.c Struct_File.h
	$(CC) $(FLAGS) -c $<

test_graphe.o : test_graphe.c *.h
	$(CC) $(FLAGS) -c $<

Struct_Liste.o : Struct_Liste.c Struct_Liste.h
	$(CC) $(FLAGS) -c $<

plot_comparaison: 
	gnuplot -p < ../comparaison_files/commande_plot_comparaison_liste.txt
	gnuplot -p < ../comparaison_files/commande_plot_comparaison_hash_arbre.txt

plot_comparaison_alea:
	gnuplot -p < ../comparaison_files/commande_plot_comparaison_alea_hash_arbre.txt
	gnuplot -p < ../comparaison_files/commande_plot_comparaison_alea_liste.txt

clean:
	rm -f *.o $(PROGS) *.html *.log *.compare.txt *.ps