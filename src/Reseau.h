#ifndef __RESEAU_H__
#define __RESEAU_H__

#include "Chaine.h"
#include "Hachage.h"

typedef struct noeud Noeud;

/* Liste chainee de noeuds (pour la liste des noeuds du reseau ET les listes des voisins de chaque noeud) */
typedef struct cellnoeud {
    Noeud *nd;              /* Pointeur vers le noeud stock\'e */
    struct cellnoeud *suiv; /* Cellule suivante dans la liste */
} CellNoeud;

/* Noeud du reseau */
struct noeud {
    int num;            /* Numero du noeud */
    double x, y;        /* Coordonnees du noeud*/
    CellNoeud *voisins; /* Liste des voisins du noeud */
};

/* Liste chainee de commodites */
typedef struct cellCommodite {
    Noeud *extr_A, *extr_B;     /* Noeuds aux extremites de la commodite */
    struct cellCommodite *suiv; /* Cellule suivante dans la liste */
} CellCommodite;

/* Un reseau */
typedef struct {
    int nb_noeuds;             /* Nombre de noeuds du reseau */
    int gamma;                 /* Nombre maximal de fibres par cable */
    CellNoeud *noeuds;         /* Liste des noeuds du reseau */
    CellCommodite *commodites; /* Liste des commodites a relier */
} Reseau;

Noeud *recherche_cree_noeud_liste(Reseau *R, double x, double y);
Reseau *reconstitue_reseau_liste(Chaines *C);
void ecrire_reseau(Reseau *R, FILE *file);
int nb_liaisons(Reseau *R);
int nb_commodites(Reseau *R);
void affiche_reseau_SVG(Reseau *R, char *nomInstance);

void liberer_cell_noeuds(CellNoeud *cells, int rm);
void liberer_noeud(Noeud *noeud);
void liberer_commodites(CellCommodite *cmd);
void liberer_reseau(Reseau *reseau);

Noeud *recherche_cree_noeud_hachage(Reseau *R, TableHachage *table, double x, double y);
Reseau *reconstitue_reseau_hachage(Chaines *C, int lenght);

#endif