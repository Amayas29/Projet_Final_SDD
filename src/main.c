#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Reseau.h"
#include "commun.h"

int main(int argc, char **argv) {
    // On test les argument
    if (argc != 2) {
        fprintf(stderr,
                "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // On supprime le nom du fichier en argument
    argc--;
    argv++;

    // On ouvre le fichier de chaines
    FILE *file_reseau = fopen(argv[0], "r");

    if (!file_reseau) {
        print_probleme("Erreur d'ouverture du fichier");
        return 1;
    }

    // On crée le fichier des resultats
    FILE *file_comparaison = fopen("fichier_comparaison.compare.txt", "w");
    if (!file_comparaison) {
        print_probleme("Erreur d'ouverture du fichier");
        fclose(file_reseau);
        return 1;
    }

    // On lit les chaines depuis le fichier
    Chaines *chaines = lecture_chaines(file_reseau);
    if (!chaines) {
        print_probleme("Erreur de création");
        fclose(file_reseau);
        fclose(file_comparaison);
        return 1;
    }
    fclose(file_reseau);

    clock_t temps_initial;
    clock_t temps_final;
    double temps_cpu_hachage;
    double temps_cpu_arbre;
    double temps_cpu_liste;
    Reseau *reseau = NULL;
    double xmax, xmin, ymax, ymin;
    int cote_x, cote_y;
    ArbreQuat *arbre = NULL;
    TableHachage *table = NULL;

    // Pour varier la taille de la table
    for (int i = 10; i < 20; i++) {
        // On crée la table de hachage correspondante à l'iterartion i
        table = cree_table_hachage(i * 1000);

        if (!table) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }

        // On calcule le temps de reconstitution avec hachage
        temps_initial = clock();
        reseau = reconstitue_reseau_hachage(chaines, table);
        temps_final = clock();
        temps_cpu_hachage = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;

        liberer_table_hachage(table);

        // Si on a eu une erreur de reconstitution on arrete le test
        if (!reseau) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }

        liberer_reseau(reseau);

        // On creer l'arbre
        chaine_coord_min_max(chaines, &xmin, &ymin, &xmax, &ymax);

        cote_x = xmax - xmin;
        cote_y = ymax - ymin;

        arbre = creer_arbre_quat((xmax + xmin) / 2, (ymax + ymin) / 2, cote_x, cote_y);
        if (!arbre) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }

        // On calcule les temps avec l'arbre
        temps_initial = clock();
        reseau = reconstitue_reseau_arbre(chaines, arbre);
        temps_final = clock();
        temps_cpu_arbre = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;

        liberer_arbre(arbre);

        // Si on a eu une erreur de reconstitution on arrete le test
        if (!reseau) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }

        liberer_reseau(reseau);

        // On calcule le temps de reconstitution avec la liste
        temps_initial = clock();
        reseau = reconstitue_reseau_liste(chaines);
        temps_final = clock();
        temps_cpu_liste = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;

        // Si on a eu une erreur de reconstitution on arrete le test
        if (!reseau) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }

        liberer_reseau(reseau);

        // Ecriture dans le fichier
        fprintf(file_comparaison, "%d %f %f %f\n", i, temps_cpu_liste, temps_cpu_hachage, temps_cpu_arbre);
    }

    liberer_structure(chaines);
    fclose(file_comparaison);
    return 0;
}
