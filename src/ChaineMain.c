#include <stdlib.h>

#include "Chaine.h"
#include "commun.h"

int main(int argc, char const *argv[]) {
    // On ouvre le fichier
    FILE *fichier = fopen("../resources/00014_burma.cha", "r");

    if (!fichier) {
        print_probleme("Fichier introuvable");
        return 1;
    }

    // On lit depuis le fichier
    Chaines *chaines = lecture_chaines(fichier);

    fclose(fichier);

    if (!chaines) {
        print_probleme("Erreur de creation");
        return 1;
    }

    // On affiche les chaines
    affiche_chaines_SVG(chaines, "chaines");

    // On ouvre un fichier en ecriture
    fichier = fopen("ecriture_burma.cha", "w");
    if (!fichier) {
        print_probleme("Erreur d'ouverture du fichier");
        liberer_structure(chaines);
        return 1;
    }

    // On ecrit les chaines
    ecrire_chaines(chaines, fichier);
    fclose(fichier);

    liberer_structure(chaines);

    return 0;
}