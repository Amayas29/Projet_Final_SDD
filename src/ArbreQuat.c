#include "ArbreQuat.h"

#include <stdio.h>
#include <stdlib.h>

#include "commun.h"

void chaine_coord_min_max(Chaines *C, double *xmin, double *ymin, double *xmax, double *ymax) {
    if (!C || !xmin || !ymin || !xmax || !ymax) {
        print_probleme("Pointeur invalide");
        return;
    }

    int init = 1;
    for (CellChaine *curr = C->chaines; curr; curr = curr->suiv) {
        for (CellPoint *points = curr->points; points; points = points->suiv) {
            if (init) {
                *xmax = points->x;
                *xmin = points->x;
                *ymax = points->y;
                *ymin = points->y;
                init = 0;
                continue;
            }

            if (points->x > *xmax)
                *xmax = points->x;

            if (points->x < *xmin)
                *xmin = points->x;

            if (points->y > *ymax)
                *ymax = points->y;

            if (points->y < *ymin)
                *ymin = points->y;
        }
    }
}

ArbreQuat *creer_arbre_quat(double xc, double yc, double cote_x, double cote_y) {
    ArbreQuat *arbre = (ArbreQuat *)malloc(sizeof(ArbreQuat));

    if (!arbre) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    arbre->xc = xc;
    arbre->yc = yc;
    arbre->cote_x = cote_x;
    arbre->cote_y = cote_y;
    arbre->so = NULL;
    arbre->se = NULL;
    arbre->no = NULL;
    arbre->ne = NULL;
    arbre->noeud = NULL;

    return arbre;
}

void inserer_noeud_arbre(Noeud *noeud, ArbreQuat **arbre, ArbreQuat *parent) {
    if (!noeud) {
        print_probleme("Pointeur invalide");
        return;
    }

    if (!*arbre) {
        *arbre = creer_arbre_quat(noeud->x, noeud->y, parent->cote_x, parent->cote_y);
        if (!arbre) return;
        (*arbre)->noeud = noeud;
        return;
    }

    parent = *arbre;

    if (parent->xc > noeud->x) {
        if (parent->yc > noeud->y)
            return inserer_noeud_arbre(noeud, &parent->so, parent);

        return inserer_noeud_arbre(noeud, &parent->no, parent);
    }

    if (parent->yc > noeud->y)
        return inserer_noeud_arbre(noeud, &parent->se, parent);

    return inserer_noeud_arbre(noeud, &parent->ne, parent);
}