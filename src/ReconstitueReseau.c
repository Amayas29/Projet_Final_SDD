#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Reseau.h"
#include "commun.h"

int main(int argc, char *argv[]) {
    

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename>"\
            "<number : Le choix de la structure \n\t - 1 pour les listes \n\t"\
            "- 2 pour la table de hachage \n\t - 3 pour les arbres\n>\n", argv[0]);
        return 1;
    }

    argc--;
    argv++;

    int number = atoi(argv[1]);

    if (number < 0 || number > 3) {
        fprintf(stderr, "Le nombre n'est pas valide\n");
        return 1;
    }

    char *filename = strcat(argv[0], ".cha");
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Le fichier %s n'existe pas\n", filename);
        return 1;
    }

    Chaines *chaines = lectureChaines(file);
    if(!chaines) {
        fclose(file);
        return 1;
    }

    if (number == 1) {

        Reseau *reseau = reconstitueReseauListe(chaines);

        if(!reseau) {
            liberer_structure(chaines);
            fclose(file);
            return 1;
        }

        liberer_reseau(reseau);
    }

    if(number == 2) {

    }

    if (number == 3) {

    }

    return 0;
}