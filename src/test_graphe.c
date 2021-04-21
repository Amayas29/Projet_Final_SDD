#include <stdio.h>

#include "Graphe.h"
#include "commun.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr,
                "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    argc--;
    argv++;

    FILE *file_reseau = fopen(argv[0], "r");

    if (!file_reseau) {
        print_probleme("Erreur d'ouverture du fichier");
        return 1;
    }

    Chaines *chaines = lecture_chaines(file_reseau);
    if (!chaines) {
        print_probleme("Erreur de création");
        fclose(file_reseau);
        return 1;
    }
    fclose(file_reseau);

    Reseau *reseau = reconstitue_reseau_liste(chaines);
    if (!reseau) {
        liberer_structure(chaines);
        return 1;
    }

    Graphe *graphe = creer_graphe(reseau);

    if (!graphe) {
        print_probleme("Erreur de création");
        liberer_structure(chaines);
        liberer_reseau(reseau);
        return 1;
    }

    int res = reorganise_reseau(reseau);
    printf("%d \n", res);

    liberer_structure(chaines);
    liberer_reseau(reseau);
    liberer_graphe(graphe);

    return 0;
}