#include "Hachage.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "commun.h"

// Retourne une clé unique pour un couple x, y
double key(double x, double y) {
    return y + (x + y) * (x + y + 1) / 2;
}

// Fonction de hachage
int hachage(double key, int lenght) {
    float a = (sqrt(5) - 1) / 2.0;
    double kA = key * a;
    long kA_int = (long)kA;

    return (int)(lenght * (kA - kA_int));
}

// Permet de créer une table de hachage
TableHachage *cree_table_hachage(int taille) {
    // Alocation memoire de la structure
    TableHachage *table = (TableHachage *)malloc(sizeof(TableHachage));
    if (!table) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    table->lenght = taille;

    // Allocation de la table
    table->table = (CellNoeud **)malloc(sizeof(CellNoeud *) * taille);

    if (!table->table) {
        free(table);
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    // Initilaisation des pointeurs a NULL
    for (int i = 0; i < taille; i++)
        table->table[i] = NULL;

    return table;
}

// Permet de liberer la table de hachage
void liberer_table_hachage(TableHachage *table) {
    if (!table->table) return;

    /* 
        On libere les listes des noeuds (Suppression faible : On supprime pas les noeud que les cells noeud, 
        les noeuds seront supprimer lors de la suppresion du reseau)
    */
    for (int i = 0; i < table->lenght; i++)
        liberer_cell_noeuds(table->table[i], 0);

    // On libere la table et la structure
    free(table->table);
    free(table);
}