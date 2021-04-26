#include <stdlib.h>

#include "Chaine.h"
#include "commun.h"

int main(int argc, char const *argv[]) {
    // On test les argument
    if (argc != 2) {
        fprintf(stderr,
                "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // On supprime le nom du fichier en argument
    argc--;
    argv++;

    // On ouvre le fichier de chaines
    FILE *fichier = fopen(argv[0], "r");

    // On teste l'ouverture du fichier
    if(!fichier){
        print_probleme("Erreur d'ouverture du fichier");
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
    affiche_chaines_SVG(chaines, "chaines_avant_ecriture");

    // On ouvre un fichier en ecriture
    fichier = fopen("resultat_ecriture.cha", "w");
    if (!fichier) {
        print_probleme("Erreur d'ouverture du fichier");
        liberer_structure(chaines);
        return 1;
    }

    // On ecrit les chaines
    ecrire_chaines(chaines, fichier);
    fclose(fichier);

    liberer_structure(chaines);


    // On ouvre le fichier de chaines
    fichier = fopen("resultat_ecriture.cha", "r");
    if (!fichier) {
        print_probleme("Erreur d'ouverture du fichier");
        return 1;
    }

    // On lit depuis le fichier
    chaines = lecture_chaines(fichier);

    fclose(fichier);

    if (!chaines) {
        print_probleme("Erreur de creation");
        return 1;
    }

    // On affiche les chaines
    affiche_chaines_SVG(chaines, "chaines_apres_ecriture");

    liberer_structure(chaines);


    return 0;
}