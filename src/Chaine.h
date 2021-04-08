#ifndef __CHAINE_H__
#define __CHAINE_H__

#include <stdio.h>

/* Liste chainee de points */
typedef struct cellPoint {
    double x, y;            /* Coordonnees du point */
    struct cellPoint *suiv; /* Cellule suivante dans la liste */
} CellPoint;

/* Celllule d une liste (chainee) de chaines */
typedef struct cellChaine {
    int numero;              /* Numero de la chaine */
    CellPoint *points;       /* Liste des points de la chaine */
    struct cellChaine *suiv; /* Cellule suivante dans la liste */
} CellChaine;

/* L'ensemble des chaines */
typedef struct {
    int gamma;           /* Nombre maximal de fibres par cable */
    int nb_chaines;       /* Nombre de chaines */
    CellChaine *chaines; /* La liste chainee des chaines */
} Chaines;

Chaines *lecture_chaines(FILE *file);
void ecrire_chaines(Chaines *C, FILE *file);
void affiche_chaines_SVG(Chaines *C, char *nom_instance);
double longueur_chaine(CellChaine *C);
double longueur_totale(Chaines *C);

int compte_points_chaines(CellChaine *C);
int compte_points_total(Chaines *C);

void liberer_point(CellPoint *point);
void liberer_liste_points(CellPoint *liste);

void liberer_chaine(CellChaine *chaine);
void liberer_liste_chaines(CellChaine *liste);

void liberer_structure(Chaines *graphe);

Chaines *generation_aleatoire(int nb_chaines, int nb_points_chaines, int xmax, int ymax);

#endif
