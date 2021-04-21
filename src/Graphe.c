#include "Graphe.h"

#include "Struct_File.h"
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
                for (Cellule_arete *cellule = v->L_voisin; cellule; cellule = cellule->suiv) {
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
            graphe->T_som[noeud->nd->num - 1]->L_voisin = cell;
        }
    }

    return graphe;
}

int t(Graphe *graphe, int u, int v) {
    u--;
    v--;

    if (!graphe) {
        print_probleme("Pointeur invalide");
        return -1;
    }

    if (u < 0 || u >= graphe->nb_som || v < 0 || v >= graphe->nb_som) {
        print_probleme("Sommet invalide");
        return -1;
    }

    int *visit = (int *)malloc(sizeof(int) * graphe->nb_som);
    if (!visit) {
        print_probleme("Erreur d'allocation");
        return -1;
    }

    int *pred = (int *)malloc(sizeof(int) * graphe->nb_som);
    if (!pred) {
        print_probleme("Erreur d'allocation");
        free(visit);
        return -1;
    }

    for (int i = 0; i < graphe->nb_som; i++) {
        visit[i] = 0;
        pred[i] = -1;
    }

    visit[u] = 1;
    S_file *file = cree_file();

    if (!file) {
        print_probleme("Erreur de creation");
        free(visit);
        free(pred);
        return -1;
    }

    enfile(file, u);

    while (!est_file_vide(file)) {
        int curr = defile(file);

        for (Cellule_arete *voisins = graphe->T_som[curr]->L_voisin; voisins; voisins = voisins->suiv) {
            int pos = voisins->a->u == curr ? voisins->a->v : voisins->a->u;

            if (visit[pos] == 0) {
                visit[pos] = 1;
                enfile(file, pos);
                pred[pos] = curr;
            }
        }
    }

    int i = v;

    while (pred[i] != -1) {
        printf("%d <-- ", i+1);
        i = pred[i];
    }

    printf("%d\n", u+1);

    free(visit);
    free(pred);
    liberer_file(file);
    return 0;
}

int plus_petit_nb_aretes(Graphe *graphe, int u, int v) {
    u--;
    v--;

    if (!graphe) {
        print_probleme("Pointeur invalide");
        return -1;
    }

    if (u < 0 || u >= graphe->nb_som || v < 0 || v >= graphe->nb_som) {
        print_probleme("Sommet invalide");
        return -1;
    }

    int *visit = (int *)malloc(sizeof(int) * graphe->nb_som);
    if (!visit) {
        print_probleme("Erreur d'allocation");
        return -1;
    }

    int *D = (int *)malloc(sizeof(int) * graphe->nb_som);
    if (!D) {
        print_probleme("Erreur d'allocation");
        free(visit);
        return -1;
    }

    for (int i = 0; i < graphe->nb_som; i++)
        visit[i] = 0;

    visit[u] = 1;
    D[u] = 0;

    S_file *file = cree_file();

    if (!file) {
        print_probleme("Erreur de creation");
        free(visit);
        free(D);
        return -1;
    }

    enfile(file, u);

    while (!est_file_vide(file)) {
        int curr = defile(file);

        for (Cellule_arete *voisins = graphe->T_som[curr]->L_voisin; voisins; voisins = voisins->suiv) {
            int pos = voisins->a->u == curr ? voisins->a->v : voisins->a->u;

            if (visit[pos] == 0) {
                visit[pos] = 1;
                enfile(file, pos);
                D[pos] = D[curr] + 1;
            }
        }
    }

    int result = D[v];
    free(visit);
    free(D);
    liberer_file(file);
    return result;
}

Sommet *cree_sommet(int num, int x, int y) {
    Sommet *new = (Sommet *)malloc(sizeof(Sommet));
    if (!new) {
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