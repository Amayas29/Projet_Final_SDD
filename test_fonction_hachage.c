#include <stdio.h>
#include <stdlib.h>

// La fonction de clé
int key(int x, int y) {
    return y + (x + y) * (x + y + 1) / 2;
}

int main() {
    // On boucle de 1 à 10 et on affiche les resultats
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
            printf("%d %d = %d\n", i, j, key(i, j));
}