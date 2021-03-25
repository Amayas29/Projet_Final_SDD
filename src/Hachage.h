#ifndef __HACHAGE_H__
#define __HACHAGE_H__

#include "Reseau.h"

typedef struct table {
    CellNoeud **table;
    int lenght;
} TableHachage;

double key(double x, double y);

int hachage(int key, int lenght);

#endif