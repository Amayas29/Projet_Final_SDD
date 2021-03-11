#include <stdlib.h>
#include "Chaine.h"


int main(int argc, char const *argv[])
{
    
    FILE *fichier = fopen("../resources/00014_burma.cha","r");
    Chaines *graphe = lectureChaines(fichier);
    fclose(fichier);
    FILE *fichier_enr = fopen("test","w");
    ecrireChaines(graphe,fichier_enr);
    liberer_structure(graphe);
    fclose(fichier_enr);

    return 0;
}