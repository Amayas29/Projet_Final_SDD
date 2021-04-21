#include "Graphe.h"

#include "commun.h"

Graphe *creer_graphe(Reseau *reseau) {
    if (!reseau) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    Graphe *graphe = (Graphe *)malloc(sizeof(Graphe));
    if (!graphe) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    graphe->nb_commod = nb_commodites(reseau);
    graphe->nb_som = reseau->nb_noeuds;
    graphe->gamma = reseau->gamma;

    graphe->T_commod = (Commod *)malloc(sizeof(Commod) * graphe->nb_commod);
    if (!graphe->T_commod) {
        print_probleme("Erreur d'allocation");
        free(graphe);
        return NULL;
    }
    graphe->T_som = (Sommet **)malloc(sizeof(Sommet *) * graphe->nb_som);
    if (!graphe->T_som) {
        print_probleme("Erreur d'allocation");
        free(graphe);
        free(graphe->T_commod);
        return NULL;
    }

    for (int i = 0; i < graphe->nb_som; i++)
        graphe->T_som[i] = NULL;

    int i = 0;
    for (CellCommodite *cmd = reseau->commodites; cmd; cmd = cmd->suiv, i++) {
        graphe->T_commod[i].e1 = cmd->extr_A->num - 1;
        graphe->T_commod[i].e2 = cmd->extr_B->num - 1;
    }

    for (CellNoeud *noeud = reseau->noeuds; noeud; noeud = noeud->suiv) {
        graphe->T_som[noeud->nd->num - 1] = cree_sommet(noeud->nd->num - 1, noeud->nd->x, noeud->nd->y);

        if (!graphe->T_som[noeud->nd->num - 1]) {
            print_probleme("Erreur d'allocation");
            liberer_graphe(graphe);
            return NULL;
        }

        for (CellNoeud *voisin = noeud->nd->voisins; voisin; voisin = voisin->suiv) {
            Arete *arete = NULL;
            Sommet *v = graphe->T_som[voisin->nd->num - 1];
            if (v) {
                for (Cellule_arete *cellule = v->L_voisin; cellule; cellule->suiv) {
                    if (cellule->a->u == noeud->nd->num - 1 || cellule->a->v == noeud->nd->num - 1) {
                        arete = cellule->a;
                        break;
                    }
                }
            } else
                arete = cree_arete(noeud->nd->num - 1, voisin->nd->num - 1);

            if (!arete) {
                print_probleme("Erreur d'allocation");
                liberer_graphe(graphe);
                return NULL;
            }

            Cellule_arete *cell = NULL;
            cell = cree_cellule_arete(arete);
            if (!cell) {
                print_probleme("Erreur de creation");
                liberer_graphe(graphe);
                return NULL;
            }

            cell->suiv = graphe->T_som[noeud->nd->num - 1]->L_voisin;
            graphe->T_som[noeud->nd->num - 1]->L_voisin = cell->suiv;
        }
    }

    return graphe;
}

Sommet *cree_sommet(int num, int x, int y) {
    Sommet *new = (Sommet *)malloc(sizeof(Sommet));
    if (new) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    new->L_voisin = NULL;
    new->num = num;
    new->x = x;
    new->y = y;

    return new;
}

Arete *cree_arete(int u, int v) {
    Arete *arete = (Arete *)malloc(sizeof(Arete));
    if (!arete) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    arete->u = u;
    arete->v = v;

    return arete;
}

Cellule_arete *cree_cellule_arete(Arete *arete) {
    Cellule_arete *cellule = (Cellule_arete *)malloc(sizeof(Cellule_arete));
    if (!cellule) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    cellule->a = arete;
    cellule->suiv = NULL;

    return cellule;
}

void liberer_graphe(Graphe *graphe) {
    if (!graphe)
        return;

    free(graphe->T_commod);

    for (int i = 0; i < graphe->nb_som; i++)
        liberer_sommet(graphe->T_som[i]);

    free(graphe->T_som);
    free(graphe);
}

void liberer_sommet(Sommet *sommet) {
    if (!sommet)
        return;

    Cellule_arete *tmp = NULL;
    while (sommet->L_voisin) {
        tmp = sommet->L_voisin->suiv;
        liberer_arete(sommet->L_voisin);
        sommet->L_voisin = tmp;
    }

    free(sommet);
}

void liberer_arete(Cellule_arete *arete) {
    if (!arete)
        return;

    if (arete->a->u == -1 && arete->a->v == -1)
        free(arete->a);
    else {
        arete->a->u = -1;
        arete->a->v = -1;
    }

    free(arete);
}