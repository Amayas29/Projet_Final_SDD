#include "Graphe.h"

#include "Struct_File.h"
#include "commun.h"

/*  Permet de creer un graphe a partir d'un reseau*/
Graphe *creer_graphe(Reseau *reseau) {
    // si le reseau n'est pas allouer on ne peut rien faire on retourne null
    if (!reseau) {
        print_probleme("Pointeur invalide");
        return NULL;
    }
    // On cree un graphe et on teste si l'allocation est bien bien marcher
    // Sinon on retourne null
    Graphe *graphe = (Graphe *)malloc(sizeof(Graphe));
    if (!graphe) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }
    
    // On initialise les champs les attributs
    graphe->nb_commod = nb_commodites(reseau);
    graphe->nb_som = reseau->nb_noeuds;
    graphe->gamma = reseau->gamma;
    
    // On alloue la memoire necesaire pour le tableau des commodites
    graphe->T_commod = (Commod *)malloc(sizeof(Commod) * graphe->nb_commod);

    // Si l'allocation s'est pas passée on libere la memoire allouer dans la fonction et on retourne null
    if (!graphe->T_commod) {
        print_probleme("Erreur d'allocation");
        free(graphe);
        return NULL;
    }

    // On alloue la memoire necesaire pour le tableau des sommets
    graphe->T_som = (Sommet **)malloc(sizeof(Sommet *) * graphe->nb_som);

    // Si l'allocation s'est pas passée on libere la memoire allouer dans la fonction et on retourne null
    if (!graphe->T_som) {
        print_probleme("Erreur d'allocation");
        free(graphe);
        free(graphe->T_commod);
        return NULL;
    }
    // On initialise toutes les cases du tableau des sommets a null
    for (int i = 0; i < graphe->nb_som; i++)
        graphe->T_som[i] = NULL;

    // On initialise les champs des structure du tableau commodites suivant les commodites du reseau
    int i = 0;
    for (CellCommodite *cmd = reseau->commodites; cmd; cmd = cmd->suiv, i++) {
        graphe->T_commod[i].e1 = cmd->extr_A->num - 1;
        graphe->T_commod[i].e2 = cmd->extr_B->num - 1;
    }

    // On boucle sur les noeuds du reseau pour cree les sommets du graphe 
    for (CellNoeud *noeud = reseau->noeuds; noeud; noeud = noeud->suiv) {

        // On cree les sommet et on initialise les champs de la structure selon les noeuds du reseau (numeros et coordonnees) 
        graphe->T_som[noeud->nd->num - 1] = cree_sommet(noeud->nd->num - 1, noeud->nd->x, noeud->nd->y);
        // Si la creation s'est mal passée on libere tout le graphe et retourne null 
        if (!graphe->T_som[noeud->nd->num - 1]) {
            print_probleme("Erreur d'allocation");
            liberer_graphe(graphe);
            return NULL;
        }
        // On boucle sur les voisin des noeuds pour creer les aretes
        for (CellNoeud *voisin = noeud->nd->voisins; voisin; voisin = voisin->suiv) {
            Arete *arete = NULL;
            // On recuperer le sommet voisin et on teste si il existe
            Sommet *v = graphe->T_som[voisin->nd->num - 1];
            
            // Si le sommet existe donc l'arete entre les deux sommet existe forcement
            if (v) {

                // On cherche l'arete dans la liste d'adjacence du sommet
                for (Cellule_arete *cellule = v->L_voisin; cellule; cellule = cellule->suiv) {
                    // On on la trouve on sort de la boucle
                    if (cellule->a->u == noeud->nd->num - 1 || cellule->a->v == noeud->nd->num - 1) {
                        arete = cellule->a;
                        break;
                    }
                }
            // Si le sommet n'existe pas donc l'arete non plus
            } else
                // On cree l'arete
                arete = cree_arete(noeud->nd->num - 1, voisin->nd->num - 1);
            
            // Si l'arete est toujours null donc soit c'est un probleme d'allocation ou c'est qu'on l'a pas trouvé
            // Dans les eux cas c'est un probleme donc on libere toute la memoire et on retourne null 
            if (!arete) {
                print_probleme("Erreur d'allocation");
                liberer_graphe(graphe);
                return NULL;
            }
            
            // On cree la cellule de l'arete 
            Cellule_arete *cell = cree_cellule_arete(arete);
            // on teste si l'allocation s'est bien passer
            if (!cell) {
                print_probleme("Erreur de creation");
                liberer_graphe(graphe);
                return NULL;
            }
            // on l'insert en tete de liste
            cell->suiv = graphe->T_som[noeud->nd->num - 1]->L_voisin;
            graphe->T_som[noeud->nd->num - 1]->L_voisin = cell;
        }
    }
    // On retourne le graphe
    return graphe;
}

/* Retourne un liste de numeros de sommets de la plus courte chaine entre deux sommets */
void generate_plus_petit_chaine(Graphe *graphe, int u, int v, ListeEntier *liste) {
    // On initialise la liste a null
    init_liste(liste);

    // On soustrait -1 des numero de sommets (indice du tableau commence par 0)
    u--;
    v--;
    // Si le graphe est null on retourne directement (On sort de la fonction)
    if (!graphe) {
        print_probleme("Pointeur invalide");
        return;
    }

    // Si les numero de sommet depasse le numero max on sort de la fonction
    if (u < 0 || u >= graphe->nb_som || v < 0 || v >= graphe->nb_som) {
        print_probleme("Sommet invalide");
        return;
    }

    // On cree un tableau de boolean,(Si la case est 0 donc le sommet n'est pas encore visté)
    int *visit = (int *)malloc(sizeof(int) * graphe->nb_som);
    // On teste si l'allocation du tableau s'est bien passé
    if (!visit) {
        print_probleme("Erreur d'allocation");
        return;
    }
    // On cree un tableau de predecesseur 
    //( si la case dont l'indice est le numero du sommet est a -1 donc elle n'a pas de predecesseur, sinon son predecesseur a pour numero de sommet la valeur de la case)
    // on l'utilise pour tracer un chemin entre deux sommets 
    int *pred = (int *)malloc(sizeof(int) * graphe->nb_som);
   
    // On teste si l'allocation du tableau s'est bien passé
   if (!pred) {
        print_probleme("Erreur d'allocation");
        free(visit);
        return;
    }

    // On initialise les cases des deux tableau (pred a -1 et visit a 0)
    for (int i = 0; i < graphe->nb_som; i++) {
        visit[i] = 0;
        pred[i] = -1;
    }

    // On met la valeur du premier sommet qui est `U` a 1 
    visit[u] = 1;

    // On cree une file 
    // Elle sert a stoquer les somets qui ne sont pas encore visité et qui doivent etre visité
    // A chaque fois on defile un sommets pour le parcourir et o enfile les sommets adjacents
    S_file *file = cree_file();

    // On teste si la file est bien allouer
    if (!file) {
        print_probleme("Erreur de creation");
        free(visit);
        free(pred);
        return;
    }

    // On enfile le premier sommet pour le parcourir
    enfile(file, u);

    // Tant qu'il existe encore des sommets qu'on doit parcourir
    while (!est_file_vide(file)) {
        // On defile le sommet a parcourir
        int curr = defile(file);

        // On boucle sur sa liste d'adjacents
        for (Cellule_arete *voisins = graphe->T_som[curr]->L_voisin; voisins; voisins = voisins->suiv) {
            // On recupere le numero du sommet adjacent 
            int pos = voisins->a->u == curr ? voisins->a->v : voisins->a->u;
            
            // Si il n'st pas encore visiter 
            if (visit[pos] == 0) {
                // On met le boolean a 1 pour ne pas l'ajouter une deuxieme fois
                visit[pos] = 1;
                enfile(file, pos);
                // On garde le precedent
                pred[pos] = curr;
            }
        }
    }


    int i = v;
    // On fait le parcours inverse pour recuperer le chemin
    while (pred[i] != -1) {
        ajoute_en_tete(liste, i + 1);
        i = pred[i];
    }

    ajoute_en_tete(liste, u + 1);

    free(visit);
    free(pred);
    liberer_file(file);
}

int plus_petit_nb_aretes(Graphe *graphe, int u, int v) {

    // On soustrait -1 des numero de sommets (indice du tableau commence par 0)
    u--;
    v--;

    // Si le graphe est null on retourne directement (On sort de la fonction)
    if (!graphe) {
        print_probleme("Pointeur invalide");
        return -1;
    }

    // Si les numero de sommet depasse le numero max on sort de la fonction
    if (u < 0 || u >= graphe->nb_som || v < 0 || v >= graphe->nb_som) {
        print_probleme("Sommet invalide");
        return -1;
    }

    // On cree un tableau de boolean,(Si la case est 0 donc le sommet n'est pas encore visté)
    int *visit = (int *)malloc(sizeof(int) * graphe->nb_som);
    // On teste si l'allocation du tableau s'est bien passé
    if (!visit) {
        print_probleme("Erreur d'allocation");
        return -1;
    }

    // On cree un tableau pour garder les distances minimales entre les deux sommets
    int *D = (int *)malloc(sizeof(int) * graphe->nb_som);
    // On teste si l'allocation  du tableau s'est bien passé
    if (!D) {
        print_probleme("Erreur d'allocation");
        free(visit);
        return -1;
    }

    // On initialise les cases du tabeau  a 0)
    for (int i = 0; i < graphe->nb_som; i++)
        visit[i] = 0;

    // On met la valeur du premier sommet qui est `U` a 1 et le distance du premier sommet a 0
    visit[u] = 1;
    D[u] = 0;

    // On cree une file 
    S_file *file = cree_file();

    // On teste si la file est bien allouer
    if (!file) {
        print_probleme("Erreur de creation");
        free(visit);
        free(D);
        return -1;
    }

    // On enfile le premier sommet pour le parcourir
    enfile(file, u);

    // Tant qu'il existe encore des sommets qu'on doit parcourir
    while (!est_file_vide(file)) {
        // On defile le sommet a parcourir
        int curr = defile(file);

        // On boucle sur sa liste d'adjacents
        for (Cellule_arete *voisins = graphe->T_som[curr]->L_voisin; voisins; voisins = voisins->suiv) {
            // On recupere le numero du sommet adjacent 
            int pos = voisins->a->u == curr ? voisins->a->v : voisins->a->u;

            // Si il n'st pas encore visiter 
            if (visit[pos] == 0) {
                // On met le boolean a 1 pour ne pas l'ajouter une deuxieme fois
                visit[pos] = 1;
                enfile(file, pos);
                // On calcule la distance entre le sommet et le premier sommet
                D[pos] = D[curr] + 1;
            }
        }
    }

    // On recupere la distance du dernier sommet et on libere les tableau et la file 
    int result = D[v];
    free(visit);
    free(D);
    liberer_file(file);
    return result;
}

Sommet *cree_sommet(int num, int x, int y) {
    // On cree un sommet 
    Sommet *new = (Sommet *)malloc(sizeof(Sommet));
    // On teste si l'allocation s'est bien
    if (!new) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }
    // On initialise les champs 
    new->L_voisin = NULL;
    new->num = num;
    new->x = x;
    new->y = y;

    return new;
}

Arete *cree_arete(int u, int v) {
    // On cree une arete 
    Arete *arete = (Arete *)malloc(sizeof(Arete));

    // On teste si l'allocation s'est bien
    if (!arete) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    // On initialise les champs 
    arete->u = u;
    arete->v = v;

    return arete;
}

Cellule_arete *cree_cellule_arete(Arete *arete) {
    // On cree une cellule d'arete 
    Cellule_arete *cellule = (Cellule_arete *)malloc(sizeof(Cellule_arete));

    // On teste si l'allocation s'est bien
    if (!cellule) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    // On initialise les champs 
    cellule->a = arete;
    cellule->suiv = NULL;

    return cellule;
}

void liberer_graphe(Graphe *graphe) {
    // Si le graphe est null on retourne
    if (!graphe)
        return;
    // on liberer le tableau des structure de commodites
    free(graphe->T_commod);

    // On libere tout les sommets
    for (int i = 0; i < graphe->nb_som; i++)
        liberer_sommet(graphe->T_som[i]);

    // On libere le tableau des sommet et le graphe
    free(graphe->T_som);
    free(graphe);
}

void liberer_sommet(Sommet *sommet) {
    // Si le sommet est null on sort de la fonction
    if (!sommet)
        return;
    // On libere toute la liste des adjacents
    Cellule_arete *tmp = NULL;
    while (sommet->L_voisin) {
        tmp = sommet->L_voisin->suiv;
        liberer_arete(sommet->L_voisin);
        sommet->L_voisin = tmp;
    }
    // on libere le sommet
    free(sommet);
}

void liberer_arete(Cellule_arete *arete) {
    // si l'arete est null on sort de la fonction
    if (!arete)
        return;
    // Sachant qu'une arete n la trouve deux fois dans la liste des adjacents,la premiere fois on met les numero a -1 la deuxieme fois on la libere
    if (arete->a->u == -1 && arete->a->v == -1)
        free(arete->a);
    else {
        arete->a->u = -1;
        arete->a->v = -1;
    }
    // On free la cellule de l'arete
    free(arete);
}

int reorganise_reseau(Reseau *reseau) {
    // Si le reseau est null on sort de la fonction
    if (!reseau) {
        print_probleme("Pointeur invalide");
        return 0;
    }
    // On cree un graphe et on teste l'allocation
    Graphe *graphe = creer_graphe(reseau);
    if (!graphe) {
        print_probleme("Erreur de création");
        return 0;
    }

    // On cree une matrice (sommet-sommet) et on teste l'allocation
    int **mat_chaines = (int **)malloc(sizeof(int *) * graphe->nb_som);
    if (!mat_chaines) {
        print_probleme("Erreur d'allocation");
        liberer_graphe(graphe);
        return 0;
    }

    for (int i = 0; i < graphe->nb_som; i++) {
        mat_chaines[i] = (int *)malloc(sizeof(int) * graphe->nb_som);

        if (!mat_chaines[i]) {
            print_probleme("Erreur d'allocation");

            for (int j = 0; j < i; j++)
                free(mat_chaines[j]);

            free(mat_chaines);
            liberer_graphe(graphe);
            return 0;
        }
        // On initialise les case de la matrice a 0
        for (int j = 0; j < graphe->nb_som; j++)
            mat_chaines[i][j] = 0;
    }

    ListeEntier liste;
    for (int i = 0; i < graphe->nb_commod; i++) {
    // On recupere pour chaque commodites le plus court chemin
        generate_plus_petit_chaine(graphe, graphe->T_commod[i].e1 + 1, graphe->T_commod[i].e2 + 1, &liste);

        Cell_entier *pred = NULL;
        // On incremente la case de chaque arete dans la matrice lorsqu'on la trouve dans le chemin de l'arete 
        for (Cell_entier *curr = liste; curr; pred = curr, curr = curr->suiv) {
            if (pred) {
                mat_chaines[curr->u - 1][pred->u - 1]++;
                mat_chaines[pred->u - 1][curr->u - 1]++;
                // Si le nombre d'apparaissance est superieur a gamma on retourne faux (0) et on libere tout les tableu et la matrice
                if (mat_chaines[curr->u - 1][pred->u - 1] > graphe->gamma) {
                    for (int i = 0; i < graphe->nb_som; i++)
                        free(mat_chaines[i]);

                    free(mat_chaines);
                    liberer_graphe(graphe);
                    desalloue(&liste);

                    return 0;
                }
            }
        }

        desalloue(&liste);
    }
    // Sinon on retourne 1 si tout les apparaissance des arete est inferieur a gamma
    for (int i = 0; i < graphe->nb_som; i++)
        free(mat_chaines[i]);

    free(mat_chaines);

    liberer_graphe(graphe);
    return 1;
}