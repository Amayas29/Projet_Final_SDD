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
    if (!noeud && !parent) {
        print_probleme("Pointeur invalide");
        return;
    }

    if (!*arbre) {
        *arbre = creer_arbre_quat(parent->xc, parent->yc, parent->cote_x / 2, parent->cote_y / 2);
        if (!arbre) return;

        (*arbre)->noeud = noeud;
        if (parent->xc > noeud->x) {
            if (parent->yc > noeud->y) {
                parent->so = *arbre;
                (*arbre)->xc -= (*arbre)->cote_x / 2;
                (*arbre)->yc -= (*arbre)->cote_y / 2;
                return;
            }
            parent->no = *arbre;
            (*arbre)->xc -= (*arbre)->cote_x / 2;
            (*arbre)->yc += (*arbre)->cote_y / 2;
            return;
        }

        if (parent->yc > noeud->y) {
            parent->se = *arbre;
            (*arbre)->xc += (*arbre)->cote_x / 2;
            (*arbre)->yc -= (*arbre)->cote_y / 2;
            return;
        }
        parent->ne = *arbre;
        (*arbre)->xc += (*arbre)->cote_x / 2;
        (*arbre)->yc += (*arbre)->cote_y / 2;
        return;
    }

    if ((*arbre)->noeud) {
        Noeud *tmp = (*arbre)->noeud;
        (*arbre)->noeud = NULL;

        ArbreQuat *arbre_tmp = NULL;
        inserer_noeud_arbre(tmp, &arbre_tmp, *arbre);

        ArbreQuat *arbre_noeud = NULL;
        inserer_noeud_arbre(noeud, &arbre_noeud, *arbre);

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

void liberer_arbre(ArbreQuat *arbre) {
    if (!arbre)
        return;

    liberer_arbre(arbre->no);
    liberer_arbre(arbre->so);
    liberer_arbre(arbre->ne);
    liberer_arbre(arbre->se);

    free(arbre);
}

Noeud *recherche_noeud_arbre(ArbreQuat *arbre, double x, double y) {
    if (!arbre)
        return NULL;

    if (arbre->noeud) {
        if (arbre->noeud->x == x && arbre->noeud->y == y)
            return arbre->noeud;
        return NULL;
    }

    if (arbre->xc > x) {
        if (arbre->yc > y)
            return recherche_noeud_arbre(arbre->so, x, y);

        return recherche_noeud_arbre(arbre->no, x, y);
    }

    if (arbre->yc > y)
        return recherche_noeud_arbre(arbre->se, x, y);

    return recherche_noeud_arbre(arbre->ne, x, y);
}