#include <stdio.h>

#include "Graphe.h"
#include "commun.h"

int main() {
    FILE *file = fopen("../resources/00014_burma.cha", "r");
    if (!file) {
        print_probleme("Fichier introuvable");
        return 1;
    }

    Chaines *chaines = lecture_chaines(file);
    if (!chaines) {
        fclose(file);
        return 1;
    }

    Reseau *reseau = reconstitue_reseau_liste(chaines);
    if (!reseau) {
        liberer_structure(chaines);
        fclose(file);
        return 1;
    }

    Graphe *graphe = creer_graphe(reseau);

    if (!graphe) {
        print_probleme("Erreur de création");
        liberer_structure(chaines);
        liberer_reseau(reseau);
        fclose(file);
        return 1;
    }

    int i = plus_petit_nb_aretes(graphe, 7, 8);
    printf("%d\n", i);

    t(graphe, 7, 8);

    liberer_structure(chaines);
    liberer_reseau(reseau);
    liberer_graphe(graphe);
    fclose(file);
    return 0;
}