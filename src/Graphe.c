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

    int nb_comm = nb_commodites(reseau);
    

    graphe->gamma = reseau->gamma;
    reseau->commodites;
    reseau->noeuds;
    reseau->nb_noeuds;
}