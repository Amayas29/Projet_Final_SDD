#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Reseau.h"
#include "commun.h"

int main() {
    /*
        On crée le fichiers des resultats
    */
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

    // On boucle sur le nombre de chaines de 500 à 5000 avec un pas de 500
    for (int i = 500; i <= 5000; i += 500) {
        // On generer aleatoirement les chaines
        chaines = generation_aleatoire(i, 100, 5000, 5000);
        if (!chaines) {
            print_probleme("Erreur de création");
            fclose(file_comparaison);
            return 1;
        }

        // On créer la table de hachage pour l'iteration i (avec une taille de 100 fois le nombre de chaines)
        table = cree_table_hachage(i * 100);
        if (!table) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }

        // On calucle le temps de reconstitution avec la table de hachage
        temps_initial = clock();
        reseau = reconstitue_reseau_hachage(chaines, table);
        temps_final = clock();
        temps_cpu_hachage = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;

        // On libere la table
        liberer_table_hachage(table);

        // Si la reconstitution est mal passé (On a pas eu de reseau on arrete le test)
        if (!reseau) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }

        liberer_reseau(reseau);

        // On créer l'arbre de l'iteration i
        // On determine les bornes x et y
        chaine_coord_min_max(chaines, &xmin, &ymin, &xmax, &ymax);

        cote_x = xmax - xmin;
        cote_y = ymax - ymin;

        // On crée l'arbre avec les coordonnes
        arbre = creer_arbre_quat((xmax + xmin) / 2, (ymax + ymin) / 2, cote_x, cote_y);
        if (!arbre) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }

        // On calcule le temps de reconstitution avec l'arbre
        temps_initial = clock();
        reseau = reconstitue_reseau_arbre(chaines, arbre);
        temps_final = clock();
        temps_cpu_arbre = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;

        liberer_arbre(arbre);

        // Si la reconstitution est mal passé on arrete le test
        if (!reseau) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }

        liberer_reseau(reseau);

        // On calcule le temps de reconstitution avec les listes
        temps_initial = clock();
        reseau = reconstitue_reseau_liste(chaines);
        temps_final = clock();
        temps_cpu_liste = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;

        // Si la reconstitution est mal passé on arrete le test
        if (!reseau) {
            print_probleme("Erreur de création");
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }

        liberer_reseau(reseau);

        // Ecriture dans le fichier
        fprintf(file_comparaison, "%d %f %f %f\n", i, temps_cpu_liste, temps_cpu_hachage, temps_cpu_arbre);

        // On libere l'instance de chaines de l'iteration i
        liberer_structure(chaines);
    }

    fclose(file_comparaison);
    return 0;
}
