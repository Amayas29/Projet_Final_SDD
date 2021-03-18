#include <stdlib.h>
#include <string.h>

#include "Chaine.h"
#include "commun.h"
#include "SVGwriter.h"

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

        for(int i = 0; buffer[i]; ) {

            if(nb_points_lu > nb_points)
                break;
            
            /* Pour ignorer les valeurs du numero de la chaine et nombre de points*/
            if(espaces < 2) {
                if(buffer[i++] == ' ') {
                    espaces++;
                    while(buffer[i] && buffer[i] == ' ') i++;
                }
                continue;
            }

            while(buffer[i] && buffer[i] == ' ') i++;

            /* On réccupere la valeur X du point */
            char X[BUFSIZ];
            j = 0;
            while(buffer[i] && buffer[i] != ' ')
                X[j++] = buffer[i++];

            if(j == 0) break;

            X[j] = '\0';
            
            /* On ignore les espaces entre les points (SI une suite d'espace est introduite dans le fichier) */
            while(buffer[i] && buffer[i] == ' ') i++;
            

            /* On réccupere la valeur Y du point */
            char Y[BUFSIZ];
            j = 0;
            while(buffer[i] && buffer[i] != ' ')
                Y[ j++ ] = buffer[i++];

            if(j == 0) break;
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
        chaine->numero = numero_chaine;

        chaine->suiv = graphe->chaines;
        graphe->chaines = chaine;

        nb++;
    }

    graphe ->nbChaines = nb;
    return graphe;
}

void ecrireChaines(Chaines *C, FILE *file) {

    /* Verification du fichier */ 
    if(!file) {
        print_probleme("Fichier introuvable");
        return;
    }

    /* Verification de la liste */
    if(!C) {
        print_probleme("Pointeur invalide");
        return;
    }

    fprintf(file, "NbChain: %d\nGamma: %d\n", C->nbChaines, C->gamma);

    int nb_points;
     /* On parcours les chaines */
    for(CellChaine *chaines = C->chaines;  chaines; chaines = chaines->suiv) {

        nb_points = 0;
        
        /* On calucle le nombre de points */
        for(CellPoint *points = chaines->points; points; points = points->suiv, nb_points++);

        /* On ecrit le numero et le nombre de points de la chaine */
        fprintf(file, "%d %d ", chaines->numero, nb_points);

        /* Pour chaque chaine on parcours sa liste de points et on les écrits */
        for(CellPoint *points = chaines->points; points; points = points->suiv)
            fprintf(file, "%.2f %.2f ", points->x, points->y);

        fprintf(file, "\n");
    }
}

/* Permet de construire un fichier svg représentant la structure de chaines */
void afficheChainesSVG(Chaines *C, char* nomInstance) {
    
    int i;
    double maxx = 0, maxy = 0, minx = 1e6, miny = 1e6;
    double precx, precy;
    
    CellChaine *ccour;
    CellPoint *pcour;
    SVGwriter svg;

    ccour = C->chaines;
    while (ccour != NULL) {

        pcour = ccour->points;

        while (pcour != NULL) {

            if (maxx < pcour->x) maxx = pcour->x;
            if (maxy < pcour->y) maxy = pcour->y;
            if (minx > pcour->x) minx = pcour->x;
            if (miny > pcour->y) miny = pcour->y;

            pcour=pcour->suiv;
        }

        ccour=ccour->suiv;
    }

    SVGinit(&svg, nomInstance, 500, 500);

    ccour = C->chaines;

    while (ccour != NULL) {

        pcour = ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg, 500 * (pcour->x-minx) / (maxx-minx), 500 * (pcour->y-miny) / (maxy-miny)); 
        precx = pcour->x;
        precy = pcour->y;  
        pcour = pcour->suiv;

        while (pcour != NULL) {
            SVGline(&svg, 500 * (precx-minx) / (maxx-minx), 500 * (precy-miny) / (maxy-miny), 500 * (pcour->x-minx) / (maxx-minx), 500 * (pcour->y-miny) / (maxy-miny));
            SVGpoint(&svg, 500 * (pcour->x-minx) / (maxx-minx), 500 * (pcour->y-miny) / (maxy-miny));
            precx = pcour->x;
            precy = pcour->y;    
            pcour = pcour->suiv;
        }
        ccour = ccour->suiv;
    }
    SVGfinalize(&svg);
}

double longueurTotale(Chaines *C);
int comptePointsTotal(Chaines *C);

/* On libere un point */
void liberer_point(CellPoint *point) {
    free(point);
}

/* On libere une liste de points */
void liberer_liste_points(CellPoint *liste) {

    CellPoint *curr = NULL;
    while(liste) {
        curr = liste->suiv;
        liberer_point(liste);
        liste = curr;
    }
}

/* On libere une chaine */
void liberer_chaine(CellChaine *chaine) {
    if(!chaine)
        return;

    liberer_liste_points(chaine->points);
    free(chaine);
}

/* On libere la liste des chaines */
void liberer_liste_chaines(CellChaine *liste) {

    CellChaine *curr = NULL;
    while(liste) {
        curr = liste->suiv;
        liberer_chaine(liste);
        liste = curr;
    }
}

/* On libere toute la structure */
void liberer_structure(Chaines *graphe) {

    if(!graphe)
        return;
    
    liberer_liste_chaines(graphe->chaines);
    free(graphe);
}