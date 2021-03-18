#include <stdlib.h>
#include "Chaine.h"


int main(int argc, char const *argv[])
{
    
    FILE *fichier = fopen("../resources/00014_burma.cha","r");
    Chaines *graphe = lectureChaines(fichier);
    fclose(fichier);
    
    afficheChainesSVG(graphe,"graphe");
    liberer_structure(graphe);
    

    return 0;
}