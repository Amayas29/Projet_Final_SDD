#include <stdio.h>

#include "Graphe.h"
#include "commun.h"

int main(int argc, char **argv) {
    // On test les arguments
    if (argc != 2) {
        fprintf(stderr,
                "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // On supprime le nom du fichier main depuis les arguments
    argc--;
    argv++;

    // On ouvre le fichier des chaines
    FILE *file_chaines = fopen(argv[0], "r");

    if (!file_chaines) {
        print_probleme("Erreur d'ouverture du fichier");
        return 1;
    }

    // On lit les chaines depuis le fichier
    Chaines *chaines = lecture_chaines(file_chaines);

    fclose(file_chaines);

    if (!chaines) {
        print_probleme("Erreur de création");
        return 1;
    }

    // On reconstruit le reseau
    Reseau *reseau = reconstitue_reseau_liste(chaines);

    // Si on a eu une erreur de reconstitution
    if (!reseau) {
        liberer_structure(chaines);
        return 1;
    }

    // On reorganise le reseau et on affiche le resultat
    int res = reorganise_reseau(reseau);
    if (res == 1)
        printf("Le reseau est reorganisé\n");
    else
        printf("Le reseau n'est pas reorganisé\n");

    liberer_structure(chaines);
    liberer_reseau(reseau);

    return 0;
}