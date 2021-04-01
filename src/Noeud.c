#include "Noeud.h"

#include <stdio.h>
#include <stdlib.h>

#include "commun.h"

Noeud *cree_noeud(int numero, double x, double y) {
    Noeud *nd = (Noeud *)malloc(sizeof(Noeud));
    if (!nd) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }
    nd->num = numero;
    nd->voisins = NULL;
    nd->x = x;
    nd->y = y;

    return nd;
}

CellNoeud *cree_cell_noeud(Noeud *noeud) {
    if (!noeud) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    CellNoeud *cell = (CellNoeud *)malloc(sizeof(CellNoeud));
    if (!cell) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    cell->nd = noeud;
    cell->suiv = NULL;
    return cell;
}

CellCommodite *cree_cell_commodite(Noeud *extr_A, Noeud *extr_B) {
    if (!extr_A || !extr_B) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    CellCommodite *comm = (CellCommodite *)malloc(sizeof(CellCommodite));
    if (!comm) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }
    comm->extr_A = extr_A;
    comm->extr_B = extr_B;
    comm->suiv = NULL;

    return comm;
}

void liberer_cell_noeuds(CellNoeud *cells, int rm) {
    CellNoeud *tmp = NULL;

    while (cells) {
        if (rm)
            liberer_noeud(cells->nd);

        tmp = cells->suiv;

        free(cells);
        cells = tmp;
    }
}

void liberer_noeud(Noeud *noeud) {
    if (!noeud) return;

    liberer_cell_noeuds(noeud->voisins, 0);
    free(noeud);
}

void liberer_commodites(CellCommodite *commodites) {
    CellCommodite *tmp = NULL;

    while (commodites) {
        tmp = commodites->suiv;
        free(commodites);
        commodites = tmp;
    }
}