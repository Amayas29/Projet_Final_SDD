#include "Hachage.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "commun.h"

double key(double x, double y) {
    return y + (x + y) * (x + y + 1) / 2;
}

int hachage(int key, int lenght) {
    float a = (sqrt(5) - 1) / 2.0;
    return ((int)(lenght * (key * a - (int)(key * a))));
}

TableHachage *cree_table_hachage(int taille) {
    TableHachage *table = (TableHachage *) malloc(sizeof(TableHachage));
    if (!table) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }
    table->lenght = taille;
    table->table = (CellNoeud *) malloc(sizeof(CellNoeud) * taille);

    if (!table->table) {
        free(table);
        print_probleme("Erreur d'allocation");
        return NULL;
    }
    for (int i = 0; i < taille; i++)
        table->table[i] = NULL;

    return table;
}

void liberer_table_hachage(TableHachage *table) {
    if (!table->table) return;

    for (int i = 0; i < table->lenght; i++)
        liberer_cell_noeuds(table->table[i], 0);

    free(table);
}