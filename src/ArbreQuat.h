#ifndef __ARBRE_QUAT_H__
#define __ARBRE_QUAT_H__

#include "Reseau.h"

/* Arbre quaternaire contenant les noeuds du reseau */
typedef struct arbreQuat {
    double xc, yc;        /* Coordonnees du centre de la cellule */
    double cote_x;        /* Longueur de la cellule */
    double cote_y;        /* Hauteur de la cellule */
    Noeud *noeud;         /* Pointeur vers le noeud du reseau */
    struct arbreQuat *so; /* Sous-arbre sud-ouest, pour x < xc et y < yc */
    struct arbreQuat *se; /* Sous-arbre sud-est, pour x >= xc et y < yc */
    struct arbreQuat *no; /* Sous-arbre nord-ouest, pour x < xc et y >= yc */
    struct arbreQuat *ne; /* Sous-arbre nord-est, pour x >= xc et y >= yc */
} ArbreQuat;

void chaine_coord_min_max(Chaines *C, double *xmin, double *ymin, double *xmax, double *ymax);
ArbreQuat *creer_arbre_quat(double xc, double yc, double cote_x, double cote_y);
void inserer_noeud_arbre(Noeud *noeud, ArbreQuat **arbre, ArbreQuat *parent);

void liberer_arbre(ArbreQuat *arbre);
Noeud *recherche_noeud_arbre(ArbreQuat *arbre, double x, double y);
#endif