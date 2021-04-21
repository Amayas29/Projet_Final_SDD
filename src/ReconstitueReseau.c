#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Reseau.h"
#include "commun.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr,
                "Usage: %s <filename>"
                " <number : Le choix de la structure> \n\t- 1 pour les listes \n\t"
                "- 2 pour la table de hachage \n\t- 3 pour les arbres\n",
                argv[0]);
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

    Chaines *chaines = lecture_chaines(file);
    if (!chaines) {
        print_probleme("Erreur de création");
        fclose(file);
        return 1;
    }

    if (number == 1) {
        Reseau *reseau = reconstitue_reseau_liste(chaines);

        if (!reseau) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file);
            return 1;
        }

        FILE *f = fopen("Test.liste.log", "w");
        if (f) {
            ecrire_reseau(reseau, f);
            fclose(f);
        }

        liberer_reseau(reseau);
    }

    if (number == 2) {
        TableHachage *table = cree_table_hachage(5000);
        if (!table) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file);
            return 1;
        }

        Reseau *reseau = reconstitue_reseau_hachage(chaines, table);
        liberer_table_hachage(table);

        if (!reseau) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file);
            return 1;
        }

        FILE *f = fopen("Test.hash.log", "w");
        if (f) {
            ecrire_reseau(reseau, f);
            fclose(f);
        }

        liberer_reseau(reseau);
    }

    if (number == 3) {
        double xmax, xmin, ymax, ymin;
        int cote_x, cote_y;

        chaine_coord_min_max(chaines, &xmin, &ymin, &xmax, &ymax);

        cote_x = xmax - xmin;
        cote_y = ymax - ymin;

        ArbreQuat *arbre = creer_arbre_quat((xmax + xmin) / 2, (ymax + ymin) / 2, cote_x, cote_y);

        if (!arbre) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file);
            return 1;
        }

        Reseau *reseau = reconstitue_reseau_arbre(chaines, arbre);

        liberer_arbre(arbre);

        if (!reseau) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file);
            return 1;
        }

        FILE *f = fopen("Test.arbre.log", "w");
        if (f) {
            ecrire_reseau(reseau, f);
            fclose(f);
        }

        liberer_reseau(reseau);
    }

    liberer_structure(chaines);
    fclose(file);
    return 0;
}