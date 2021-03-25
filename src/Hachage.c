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