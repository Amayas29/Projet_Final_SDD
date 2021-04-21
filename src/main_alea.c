#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Reseau.h"
#include "commun.h"

int main() {
    FILE *file_comparaison = fopen("fichier_comparaison_alea.compare.txt", "w");
    if (!file_comparaison) {
        print_probleme("Erreur d'ouverture du fichier");
        return 1;
    }

    Chaines *chaines = NULL;

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

    for (int i = 500; i <= 5000; i += 500) {
        chaines = generation_aleatoire(i, 100, 5000, 5000);
        if (!chaines) {
            fclose(file_comparaison);
            return 1;
        }

        table = cree_table_hachage(i * 100);
        if (!table) {
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }

        //hach
        temps_initial = clock();
        reseau = reconstitue_reseau_hachage(chaines, table);
        temps_final = clock();

        liberer_table_hachage(table);

        if (!reseau) {
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }

        liberer_reseau(reseau);
        temps_cpu_hachage = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;

        chaine_coord_min_max(chaines, &xmin, &ymin, &xmax, &ymax);

        cote_x = xmax - xmin;
        cote_y = ymax - ymin;

        arbre = creer_arbre_quat((xmax + xmin) / 2, (ymax + ymin) / 2, cote_x, cote_y);
        if (!arbre) {
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }

        //arbre
        temps_initial = clock();
        reseau = reconstitue_reseau_arbre(chaines, arbre);
        temps_final = clock();

        liberer_arbre(arbre);

        if (!reseau) {
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }

        liberer_reseau(reseau);
        temps_cpu_arbre = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;

        //liste
        temps_initial = clock();
        reseau = reconstitue_reseau_liste(chaines);
        temps_final = clock();

        if (!reseau) {
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }

        liberer_reseau(reseau);
        temps_cpu_liste = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;

        liberer_structure(chaines);

        // Ecriture dans le fichier
        fprintf(file_comparaison, "%d %f %f %f\n", i, temps_cpu_liste, temps_cpu_hachage, temps_cpu_arbre);
    }

    fclose(file_comparaison);
    return 0;
}
