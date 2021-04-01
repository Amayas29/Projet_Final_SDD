#ifndef __NOEUD_H__
#define __NOEUD_H__

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

Noeud *cree_noeud(int numero, double x, double y);
CellNoeud *cree_cell_noeud(Noeud *noeud);
CellCommodite *cree_cell_commodite(Noeud *extr_A, Noeud *extr_B);

void liberer_cell_noeuds(CellNoeud *cells, int rm);
void liberer_noeud(Noeud *noeud);
void liberer_commodites(CellCommodite *cmd);

#endif