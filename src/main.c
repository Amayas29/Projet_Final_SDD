#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Reseau.h"
#include "commun.h"

int main(int argc, char **argv){


    if(argc != 2){
        fprintf(stderr,
                "Usage: %s <filename>",argv[0]);
        return 1;
    }

    argc--;
    argc++;

    FILE *file_reseau = fopen(argv[0], "r");

    if(! file_reseau){
        print_probleme("erreur d'ouverture du fichier");
        return 1;
    }

    FILE* file_comparaison = fopen("fichier_comparaison.compare.txt","w");
    if(! file_comparaison){
        print_probleme("erreur d'ouverture du fichier");
        fclose(file_reseau);
        return 1;

    }

    Chaines *chaines = lecture_chaines(file_reseau);
    if (!chaines) {
        fclose(file_reseau);
        fclose(file_comparaison);
        return 1;
    }
    fclose(file_reseau);

    

    clock_t temps_initial;
    clock_t temps_final;
    double temps_cpu_hashage;
    double temps_cpu_arbre;
    double temps_cpu_liste;
    Reseau *reseau  = NULL;
    for(int i = 10;i< NB_MAX ;i++){
        
        //liste
        temps_initial = clock();
        
        reseau = reconstitue_reseau_liste(chaines);

        temps_final = clock();    
        if (!reseau) {
            liberer_structure(chaines);
            fclose(file_comparaison);
            
            return 1;
        }

        temps_cpu_liste = ((double) (temps_final - temps_initial)) / CLOCKS_PER_SEC;        
        liberer_reseau(reseau);
        
        //hash
        temps_initial = clock();
        reseau = reconstitue_reseau_hachage(chaines,i);
        temps_final = clock();

        if (!reseau) {
            liberer_structure(chaines);
            fclose(file_comparaison);
            
            return 1;
        }
        liberer_reseau(reseau);
        temps_cpu_hashage = ((double) (temps_final - temps_initial)) / CLOCKS_PER_SEC;        

        //arbres
        temps_initial = clock();
        reseau = reconstitue_reseau_arbre(chaines);
        temps_final = clock();

        if (!reseau) {
            liberer_structure(chaines);
            fclose(file_comparaison);
            return 1;
        }
        liberer_reseau(reseau);
        temps_cpu_arbre = ((double) (temps_final - temps_initial)) / CLOCKS_PER_SEC;        
        
    
        //ecriture dans le fichier 
        fprintf(file_comparaison,"%d %f %f %f\n",i,temps_cpu_liste,temps_cpu_hashage,temps_cpu_arbre);   
    }

    liberer_structure(chaines);
    fclose(file_comparaison);
    return 0;
}
