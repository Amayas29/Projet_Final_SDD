#include <stdlib.h>

#include "Chaine.h"

int main(int argc, char const *argv[]) {
    FILE *fichier = fopen("../resources/00014_burma.cha", "r");
    Chaines *graphe = lecture_chaines(fichier);
    fclose(fichier);

    affiche_chaines_SVG(graphe, "graphe");
    liberer_structure(graphe);

    return 0;
}