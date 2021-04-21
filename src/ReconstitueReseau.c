#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Reseau.h"
#include "commun.h"

int main(int argc, char *argv[]) {
    // On test les arguments
    if (argc != 3) {
        fprintf(stderr,
                "Usage: %s <filename>"
                " <number : Le choix de la structure> \n\t- 1 pour les listes \n\t"
                "- 2 pour la table de hachage \n\t- 3 pour les arbres\n",
                argv[0]);
        return 1;
    }

    // On supprime le nom du fichier main des arguments
    argc--;
    argv++;

    int number = atoi(argv[1]);

    // On test le nombre si il est valide
    if (number < 0 || number > 3) {
        fprintf(stderr, "Le nombre n'est pas valide\n");
        return 1;
    }

    // On ouvre le fichier (Il doit pas avoir l'exstension cha On la rajoute dans le main)
    char *filename = strcat(argv[0], ".cha");
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Le fichier %s n'existe pas\n", filename);
        return 1;
    }

    // On lit les chaines depuis le fichier
    Chaines *chaines = lecture_chaines(file);
    if (!chaines) {
        print_probleme("Erreur de création");
        fclose(file);
        return 1;
    }

    // Premiere partie : Avec les liste
    if (number == 1) {
        // On reconstruit le reseau
        Reseau *reseau = reconstitue_reseau_liste(chaines);

        // Si on a eu une erreur de reconstitution
        if (!reseau) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file);
            return 1;
        }

        // On crée le fichier de sortie : Ou on va ecrire le reseau reconstruit
        FILE *f = fopen("Test.liste.log", "w");
        if (f) {
            ecrire_reseau(reseau, f);
            fclose(f);
        }

        liberer_reseau(reseau);
    }

    // Deuxieme partie : Avec la table de hachage
    if (number == 2) {
        // On crée la table de hachage
        TableHachage *table = cree_table_hachage(5000);
        if (!table) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file);
            return 1;
        }

        // On reconstruit le reseau
        Reseau *reseau = reconstitue_reseau_hachage(chaines, table);

        liberer_table_hachage(table);

        // Si on a eu une erreur de reconstitution
        if (!reseau) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file);
            return 1;
        }

        // On crée le fichier de sortie : Ou on va ecrire le reseau reconstruit
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

        // On crée l'arbre
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

        // On reconstruit le reseau
        Reseau *reseau = reconstitue_reseau_arbre(chaines, arbre);

        liberer_arbre(arbre);

        // Si on a eu une erreur de reconstitution
        if (!reseau) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file);
            return 1;
        }

        // On crée le fichier de sortie : Ou on va ecrire le reseau reconstruit
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