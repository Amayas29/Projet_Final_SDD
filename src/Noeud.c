#include "Noeud.h"

#include <stdio.h>
#include <stdlib.h>

#include "commun.h"

Noeud *cree_noeud(int numero, double x, double y) {
    // On alloue la mémoire pour le noeud
    Noeud *nd = (Noeud *)malloc(sizeof(Noeud));

    // Un probleme lors de l'allocation
    if (!nd) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    // On initialise les champs
    nd->num = numero;
    nd->voisins = NULL;
    nd->x = x;
    nd->y = y;

    return nd;
}

CellNoeud *cree_cell_noeud(Noeud *noeud) {
    // On test le pointeur
    if (!noeud) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    // On alloue le cell noeud
    CellNoeud *cell = (CellNoeud *)malloc(sizeof(CellNoeud));

    // Si on a eu un probleme d'allocation
    if (!cell) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    // On initialise les champs
    cell->nd = noeud;
    cell->suiv = NULL;

    return cell;
}

CellCommodite *cree_cell_commodite(Noeud *extr_A, Noeud *extr_B) {
    // On test les pointeurs
    if (!extr_A || !extr_B) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    // On alloue la memoire pour la commodite
    CellCommodite *comm = (CellCommodite *)malloc(sizeof(CellCommodite));

    // Si on a un probleme d'allocation
    if (!comm) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    // On initialise les champs
    comm->extr_A = extr_A;
    comm->extr_B = extr_B;
    comm->suiv = NULL;

    return comm;
}

void liberer_cell_noeuds(CellNoeud *cells, int rm) {
    CellNoeud *tmp = NULL;

    // On parcours la liste
    while (cells) {
        // Si c'est une suppresion forte on supprime les noeud
        if (rm)
            liberer_noeud(cells->nd);

        // On suppime le cell noeud
        tmp = cells->suiv;

        free(cells);
        cells = tmp;
    }
}

void liberer_noeud(Noeud *noeud) {
    if (!noeud) return;

    // On libere sa liste de voisins
    liberer_cell_noeuds(noeud->voisins, 0);

    // On libere l'espace alloue pour le noeud
    free(noeud);
}

void liberer_commodites(CellCommodite *commodites) {
    CellCommodite *tmp = NULL;

    // On parcours la liste des commodites et on supprime une par une
    while (commodites) {
        tmp = commodites->suiv;
        free(commodites);
        commodites = tmp;
    }
}