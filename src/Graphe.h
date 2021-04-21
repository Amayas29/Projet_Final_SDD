#ifndef __GRAPHE_H__
#define __GRAPHE_H__

#include <stdio.h>
#include <stdlib.h>

#include "Reseau.h"
#include "Struct_Liste.h"

typedef struct {
    int u, v; /* Numeros des sommets extremité */
} Arete;

typedef struct cellule_arete {
    Arete *a; /* pointeur sur l'arete */
    struct cellule_arete *suiv;
} Cellule_arete;

typedef struct {
    int num; /* Numero du sommet (le meme que dans T_som) */
    double x, y;
    Cellule_arete *L_voisin; /* Liste chainee des voisins */
} Sommet;

typedef struct {
    int e1, e2; /* Les deux extremites de la commodite */
} Commod;

typedef struct {
    int nb_som;     /* Nombre de sommets */
    Sommet **T_som; /* Tableau de pointeurs sur sommets */
    int gamma;
    int nb_commod;    /* Nombre de commodites */
    Commod *T_commod; /* Tableau des commodites */
} Graphe;

Graphe *creer_graphe(Reseau *reseau);

int plus_petit_nb_aretes(Graphe *graphe, int u, int v);
void generate_plus_petit_chaine(Graphe *graphe, int u, int v, ListeEntier *liste);

Sommet *cree_sommet(int num, int x, int y);
Arete *cree_arete(int u, int v);
Cellule_arete *cree_cellule_arete(Arete *arete);
void liberer_graphe(Graphe *graphe);
void liberer_sommet(Sommet *sommet);
void liberer_arete(Cellule_arete *arete);

int reorganise_reseau(Reseau *reseau);

#endif
