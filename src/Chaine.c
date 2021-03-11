#include <stdlib.h>
#include <string.h>

#include "Chaine.h"
#include "commun.h"

/* Permet de lire la structure de chaines à partir d'un fichier */
Chaines* lectureChaines(FILE *file) {

    /* Verification du fichier */ 
    if(!file) {
        print_probleme("Fichier introuvable");
        return NULL;
    }

    char buffer[BUFSIZ];
    int nb_chaine, gamma;

    /* On reccupére le nombre de chaines et le gamma avec véerificatiob de leur existence */
    if(!(fgets(buffer, BUFSIZ, file) && sscanf(buffer, "NbChain: %d", &nb_chaine) == 1 
        && fgets(buffer, BUFSIZ, file) && sscanf(buffer, "Gamma: %d", &gamma) == 1)) {

            print_probleme("Erreur de lecture : Mauvais format du fichier");
            return NULL;
    }
    
    Chaines *graphe = malloc(sizeof(Chaines));

    if(!graphe) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    graphe->gamma = gamma;
    graphe->chaines = NULL;

    int numero_chaine, nb_points, j;
    double x, y;

    CellPoint *points = NULL;
    CellChaine *chaine = NULL;

    /* On parcours le fichier, tantque on a pas atteint le nombre et que la fin du fichier n'est pas atteinte */
    int nb = 0;
    while(nb < nb_chaine && fgets(buffer, BUFSIZ, file)) {

        /* On supprime le dernier \n du buffer */
        buffer[strlen(buffer) - 1] = '\0';

        /* On réccupere le nombre de chaines et le nombre de points, si on les a pas on ignore la ligne */
        if(sscanf(buffer, "%d %d", &numero_chaine, &nb_points) != 2)
            continue;

        points = NULL;
        int espaces = 0;
        int nb_points_lu = 0;

        for(int i = 0; buffer[i]; i++) {

            if(nb_points_lu == nb_points)
                break;
            
            /* Pour ignorer les valeurs du numero de la chaine et nombre de points*/
            if(espaces < 2) {
                if(buffer[i] == ' ')
                    espaces++;
                while(buffer[i++] == ' ');
                continue;
            }

            /* On réccupere la valeur X du point */
            char X[BUFSIZ];
            j = 0;
            while(buffer[i] != ' ')
                X[ j++ ] = buffer[i++];

            X[j] = '\0';
            
            /* On ignore les espaces entre les points (SI une suite d'espace est introduite dans le fichier) */
            while(buffer[i++] == ' ');

            /* On réccupere la valeur Y du point */
            char Y[BUFSIZ];
            j = 0;
            while(buffer[i] != ' ')
                Y[ j++ ] = buffer[i++];

            Y[j] = '\0';

            /* On convertis les valeur lu en double */
            x = atof(X);
            y = atof(Y);

            /* On crée un point */
            CellPoint *point = malloc(sizeof(CellPoint));

            if(!point)
                continue;

            /* On sauvegarde les donnés du point et on l'ajoute à la liste */
            point->x = x;
            point->y = y;
            point->suiv = points;
            points = point;

            nb_points_lu++;
        }

        /* On crée un élement de chaine */
        chaine = malloc(sizeof(CellChaine));

        /* Si on a une erreur d'allocation on libere la liste des points lus */
        if(!chaine) {
            liberer_liste_points(points);
            continue;
        }

        /* On ajoute la chaine en tete de liste de la liste des chaines */
        chaine->points = points;
        chaine->suiv = graphe;
        graphe->chaines = chaine;
        
        nb++;
    }

    graphe ->nbChaines = nb;
    return graphe;
}

void ecrireChaines(Chaines *C, FILE *file);


void afficheChainesSVG(Chaines *C, char* nomInstance);
double longueurTotale(Chaines *C);
int comptePointsTotal(Chaines *C);


void liberer_point(CellPoint *point) {
    free(point);
}

void liberer_liste_points(CellPoint *liste) {

    CellPoint *curr = NULL;
    while(liste) {
        curr = liste->suiv;
        liberer_point(liste);
        liste = curr;
    }
}

void liberer_chaine(CellChaine *chaine) {
    if(!chaine)
        return;

    liberer_liste_points(chaine->points);
    free(chaine);
}

void liberer_liste_chaines(CellChaine *liste) {

    CellChaine *curr = NULL;
    while(liste) {
        curr = liste->suiv;
        liberer_chaine(liste);
        liste = curr;
    }
}

void liberer_structure(Chaines *graphe) {

    if(!graphe)
        return;
    
    liberer_liste_chaines(graphe->chaines);
    free(graphe);
}