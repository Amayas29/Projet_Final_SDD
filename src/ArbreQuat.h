#ifndef __ARBRE_QUAT_H__
#define __ARBRE_QUAT_H__

typedef struct noeud {
} Noeud;

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

#endif
