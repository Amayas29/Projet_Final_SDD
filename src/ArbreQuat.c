#include "ArbreQuat.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "commun.h"

// Permet de rechercher le minimum et le maximum des x et des y
void chaine_coord_min_max(Chaines *C, double *xmin, double *ymin, double *xmax, double *ymax) {
    // Teste si la memoire est bien allouer avant l'appel a la fonction
    if (!C || !xmin || !ymin || !xmax || !ymax) {
        print_probleme("Pointeur invalide");
        return;
    }

    // Boolean pour savoir si on est dans a premiere iteration (la ou on initialise les variable minimums et maximums)
    int init = 1;

    // Parcourir toutes les chaines et tout les points de chaque chaine pour chercher le minimum et le maximum de x et de y
    for (CellChaine *curr = C->chaines; curr; curr = curr->suiv) {
        for (CellPoint *points = curr->points; points; points = points->suiv) {
            // si c'est la premiere iteration on initialise les variable
            if (init) {
                *xmax = points->x;
                *xmin = points->x;
                *ymax = points->y;
                *ymin = points->y;
                // on met e boolean a False(premiere iteration deja passer)
                init = 0;
                continue;
            }

            /*
            * On teste a chaque fois si les coordonnes du point courrant sont superieur au maximum ou inferieur au minimum 
            * (recherche du minimum et du maximum)
            */
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

// Permet de cree un arbre (allouer la memoire et initialiser ses attributs)
ArbreQuat *creer_arbre_quat(double xc, double yc, double cote_x, double cote_y) {
    // allocation de l'arbre
    ArbreQuat *arbre = (ArbreQuat *)malloc(sizeof(ArbreQuat));

    // Teste d'allocation
    if (!arbre) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    // initialisation des attributs
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

// Permet d'inserer un noeud dans un arbre selon les contraintes donnees
void inserer_noeud_arbre(Noeud *noeud, ArbreQuat **arbre, ArbreQuat *parent) {
    // Teste si la memoire est bien allouer avant l'appel a la fonction
    if (!noeud && !parent) {
        print_probleme("Pointeur invalide");
        return;
    }

    // Si l'arbre n'est pas allouer (donc il n'existe pas)
    if (!*arbre) {
        // On cree l'arbre
        *arbre = creer_arbre_quat(noeud->x, noeud->y, parent->cote_x / 2, parent->cote_y / 2);

        // On teste si il est bien créé
        if (!arbre) return;

        // Mettre le noeud dans l'arbre
        (*arbre)->noeud = noeud;

        // Chercher la bonne position ou il faut mettre l'arbre dans son pere
        if (parent->xc > noeud->x) {
            if (parent->yc > noeud->y) {
                parent->so = *arbre;
                return;
            }
            parent->no = *arbre;
            return;
        }

        if (parent->yc > noeud->y) {
            parent->se = *arbre;
            return;
        }

        parent->ne = *arbre;
        return;
    }
    // Si l'arbre existe et qu'il a un noeud
    if ((*arbre)->noeud) {
        // On garde le noeud de l'arbre et on ecrase le contenue de l'attribut noeud de l'arbre par null
        Noeud *tmp = (*arbre)->noeud;
        (*arbre)->noeud = NULL;

        // Mettre a jour les cote de l'arbre en selon  son emplacement par rapport a son pere
        if (parent->xc > (*arbre)->xc) {
            if (parent->yc > (*arbre)->yc) {
                (*arbre)->xc = parent->xc - (*arbre)->cote_x / 2;
                (*arbre)->yc = parent->yc - (*arbre)->cote_y / 2;
            }

            else {
                (*arbre)->xc = parent->xc - (*arbre)->cote_x / 2;
                (*arbre)->yc = parent->yc + (*arbre)->cote_y / 2;
            }

        } else {
            if (parent->yc > (*arbre)->yc) {
                (*arbre)->xc = parent->xc + (*arbre)->cote_x / 2;
                (*arbre)->yc = parent->yc - (*arbre)->cote_y / 2;
            } else {
                (*arbre)->xc = parent->xc + (*arbre)->cote_x / 2;
                (*arbre)->yc = parent->yc + (*arbre)->cote_y / 2;
            }
        }

        // Ajouter le noeud garder
        /* 
        * Pour ca on doit d'abord chercher le bon emplacement du noeud avant l'insertion  
        * on teste selon ses coordonnes pour trouver son emplacement dans l'arbre
        */
        ArbreQuat **sous_fils_tmp = NULL;
        if (parent->xc > tmp->x) {
            if (parent->yc > tmp->y)
                sous_fils_tmp = &parent->so;
            else
                sous_fils_tmp = &parent->no;
        } else {
            if (parent->yc > tmp->y)
                sous_fils_tmp = &parent->se;
            else
                sous_fils_tmp = &parent->ne;
        }
        // inserer le noeud a son emplacement `sous_fils_tmp`
        inserer_noeud_arbre(tmp, sous_fils_tmp, *arbre);

        // Ajouter le noeud donne en parametre de la fonction
        /* 
        * Pour ca on doit d'abord chercher le bon emplacement du noeud avant l'insertion  
        * on teste selon ses coordonnes pour trouver son emplacement dans l'arbre
        */
        ArbreQuat **sous_fils_noeud = NULL;
        if (parent->xc > noeud->x) {
            if (parent->yc > noeud->y)
                sous_fils_noeud = &parent->so;
            else
                sous_fils_noeud = &parent->no;
        } else {
            if (parent->yc > noeud->y)
                sous_fils_noeud = &parent->se;
            else
                sous_fils_noeud = &parent->ne;
        }

        // inserer le noeud a son emplacement `sous_fils_noeud`
        inserer_noeud_arbre(noeud, sous_fils_noeud, *arbre);
        return;
    }
    // Si l'arbre existe mais il n'a pas de noeud (donc il a au moins deux fils non NULL)
    // On met l'arbre a la place du parent
    parent = *arbre;

    // On cherche l'emplacement ou il faut inserer le noeud selon ses coordone comparer a celle de son pere
    if (parent->xc > noeud->x) {
        if (parent->yc > noeud->y)
            return inserer_noeud_arbre(noeud, &parent->so, parent);

        return inserer_noeud_arbre(noeud, &parent->no, parent);
    }

    if (parent->yc > noeud->y)
        return inserer_noeud_arbre(noeud, &parent->se, parent);

    return inserer_noeud_arbre(noeud, &parent->ne, parent);
}

// Permet de liberer un arbre
void liberer_arbre(ArbreQuat *arbre) {
    if (!arbre)
        return;
    // liberer les 4 fils de l'arbre
    liberer_arbre(arbre->no);
    liberer_arbre(arbre->so);
    liberer_arbre(arbre->ne);
    liberer_arbre(arbre->se);

    // les noeuds sont liberer lors de la liberation du reseau
    //liberer l'arbre
    free(arbre);
}

// Permet de rechercher un noeud dans un arbre (en fonction de ses coordonnes(x,y))
Noeud *recherche_noeud_arbre(ArbreQuat *arbre, double x, double y) {
    // si l'arbre n'est pas allouer on returne null (le noeud n'existe pas)
    if (!arbre)
        return NULL;

    /*
    * Si l'arbre existe et contient un noeud ,
    * on verifie si les coordonne sont egaux au celle du noeud de l'arbre si oui on le returne sinon on return null 
    */
    if (arbre->noeud) {
        if (arbre->noeud->x == x && arbre->noeud->y == y)
            return arbre->noeud;

        return NULL;
    }
    // On continue de decendre dans la branche ou se trouve le noeud (en fonction de ses coordonnes)
    if (arbre->xc > x) {
        if (arbre->yc > y)
            return recherche_noeud_arbre(arbre->so, x, y);

        return recherche_noeud_arbre(arbre->no, x, y);
    }

    if (arbre->yc > y)
        return recherche_noeud_arbre(arbre->se, x, y);

    return recherche_noeud_arbre(arbre->ne, x, y);
}