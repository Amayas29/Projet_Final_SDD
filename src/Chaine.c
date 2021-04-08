#include "Chaine.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "SVGwriter.h"
#include "commun.h"

/* Permet de lire la structure de chaines à partir d'un fichier */
Chaines *lecture_chaines(FILE *file) {
    /* Verification du fichier */
    if (!file) {
        print_probleme("Fichier introuvable");
        return NULL;
    }

    char buffer[BUFSIZ];
    int nb_chaine, gamma;

    /* On reccupére le nombre de chaines et le gamma avec véerificatiob de leur existence */
    if (!(fgets(buffer, BUFSIZ, file) && sscanf(buffer, "NbChain: %d", &nb_chaine) == 1 && fgets(buffer, BUFSIZ, file) && sscanf(buffer, "Gamma: %d", &gamma) == 1)) {
        print_probleme("Erreur de lecture : Mauvais format du fichier");
        return NULL;
    }

    Chaines *graphe = malloc(sizeof(Chaines));

    if (!graphe) {
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
    while (nb < nb_chaine && fgets(buffer, BUFSIZ, file)) {
        /* On supprime le dernier \n du buffer */
        buffer[strlen(buffer) - 1] = '\0';

        /* On réccupere le nombre de chaines et le nombre de points, si on les a pas on ignore la ligne */
        if (sscanf(buffer, "%d %d", &numero_chaine, &nb_points) != 2)
            continue;

        points = NULL;
        int espaces = 0;
        int nb_points_lu = 0;

        for (int i = 0; buffer[i];) {
            if (nb_points_lu > nb_points)
                break;

            /* Pour ignorer les valeurs du numero de la chaine et nombre de points*/
            if (espaces < 2) {
                if (buffer[i++] == ' ') {
                    espaces++;
                    while (buffer[i] && buffer[i] == ' ') i++;
                }
                continue;
            }

            while (buffer[i] && buffer[i] == ' ') i++;

            /* On réccupere la valeur X du point */
            char X[BUFSIZ];
            j = 0;
            while (buffer[i] && buffer[i] != ' ')
                X[j++] = buffer[i++];

            if (j == 0) break;

            X[j] = '\0';

            /* On ignore les espaces entre les points (SI une suite d'espace est introduite dans le fichier) */
            while (buffer[i] && buffer[i] == ' ') i++;

            /* On réccupere la valeur Y du point */
            char Y[BUFSIZ];
            j = 0;
            while (buffer[i] && buffer[i] != ' ')
                Y[j++] = buffer[i++];

            if (j == 0) break;
            Y[j] = '\0';

            /* On convertis les valeur lu en double */
            x = atof(X);
            y = atof(Y);

            /* On crée un point */
            CellPoint *point = malloc(sizeof(CellPoint));

            if (!point)
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
        if (!chaine) {
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

    graphe->nb_chaines = nb;
    return graphe;
}

void ecrire_chaines(Chaines *C, FILE *file) {
    /* Verification du fichier */
    if (!file) {
        print_probleme("Fichier introuvable");
        return;
    }

    /* Verification de la liste */
    if (!C) {
        print_probleme("Pointeur invalide");
        return;
    }

    fprintf(file, "NbChain: %d\nGamma: %d\n", C->nb_chaines, C->gamma);

    int nb_points;
    /* On parcours les chaines */
    for (CellChaine *chaines = C->chaines; chaines; chaines = chaines->suiv) {
        nb_points = compte_points_chaines(chaines);

        /* On ecrit le numero et le nombre de points de la chaine */
        fprintf(file, "%d %d ", chaines->numero, nb_points);

        /* Pour chaque chaine on parcours sa liste de points et on les écrits */
        for (CellPoint *points = chaines->points; points; points = points->suiv)
            fprintf(file, "%.2f %.2f ", points->x, points->y);

        fprintf(file, "\n");
    }
}

/* Permet de construire un fichier svg représentant la structure de chaines */
void affiche_chaines_SVG(Chaines *C, char *nom_instance) {
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

            pcour = pcour->suiv;
        }

        ccour = ccour->suiv;
    }

    SVG_init(&svg, nom_instance, 500, 500);

    ccour = C->chaines;

    while (ccour != NULL) {
        pcour = ccour->points;
        SVG_line_rand_color(&svg);
        SVG_point(&svg, 500 * (pcour->x - minx) / (maxx - minx), 500 * (pcour->y - miny) / (maxy - miny));
        precx = pcour->x;
        precy = pcour->y;
        pcour = pcour->suiv;

        while (pcour != NULL) {
            SVG_line(&svg, 500 * (precx - minx) / (maxx - minx), 500 * (precy - miny) / (maxy - miny), 500 * (pcour->x - minx) / (maxx - minx), 500 * (pcour->y - miny) / (maxy - miny));
            SVG_point(&svg, 500 * (pcour->x - minx) / (maxx - minx), 500 * (pcour->y - miny) / (maxy - miny));
            precx = pcour->x;
            precy = pcour->y;
            pcour = pcour->suiv;
        }
        ccour = ccour->suiv;
    }
    SVG_finalize(&svg);
}

/* Calcule la distance entre deux points */
static double distance_points(CellPoint *p1, CellPoint *p2) {
    if (!p1 || !p2) return 0;
    return sqrt(pow((p1->x - p2->x), 2) + pow((p1->y - p2->y), 2));
}

/* Calcule la longeur physique d'une chaine */
double longueur_chaine(CellChaine *C) {
    CellPoint *pred = NULL;
    double longeur = 0;

    for (CellPoint *curr = C->points; curr; curr = curr->suiv) {
        longeur += distance_points(pred, curr);
        pred = curr;
    }

    return longeur;
}

/* Calcule la longeur physique totales des chaines */
double longueur_totale(Chaines *C) {
    double total = 0;
    for (CellChaine *cell = C->chaines; cell; total += longueur_chaine(cell), cell = cell->suiv)
        ;
    return total;
}

/* Calcule le nombre de points d'une chaine */
int compte_points_chaines(CellChaine *C) {
    int nb_points = 0;

    /* On calucle le nombre de points */
    for (CellPoint *points = C->points; points; points = points->suiv, nb_points++) continue;
    return nb_points;
}

/* Calcule le nombre totale de points de la structure des chaines */
int compte_points_total(Chaines *C) {
    int total = 0;
    for (CellChaine *cell = C->chaines; cell; total += compte_points_chaines(cell), cell = cell->suiv) continue;
    return total;
}

/* On libere un point */
void liberer_point(CellPoint *point) {
    free(point);
}

/* On libere une liste de points */
void liberer_liste_points(CellPoint *liste) {
    CellPoint *curr = NULL;
    while (liste) {
        curr = liste->suiv;
        liberer_point(liste);
        liste = curr;
    }
}

/* On libere une chaine */
void liberer_chaine(CellChaine *chaine) {
    if (!chaine)
        return;

    liberer_liste_points(chaine->points);
    free(chaine);
}

/* On libere la liste des chaines */
void liberer_liste_chaines(CellChaine *liste) {
    CellChaine *curr = NULL;
    while (liste) {
        curr = liste->suiv;
        liberer_chaine(liste);
        liste = curr;
    }
}

/* On libere toute la structure */
void liberer_structure(Chaines *graphe) {
    if (!graphe)
        return;

    liberer_liste_chaines(graphe->chaines);
    free(graphe);
}

Chaines *generation_aleatoire(int nb_chaines, int nb_points_chaines, int xmax, int ymax) {
    Chaines *chaines = malloc(sizeof(Chaines));
    if (!chaines) {
        print_probleme("Probleme d'allocation");
        return NULL;
    }

    chaines->gamma = 0;
    chaines->nb_chaines = nb_chaines;
    chaines->chaines = NULL;

    for (int i = 0; i < nb_points_chaines; i++) {
        CellChaine *chaine_i = malloc(sizeof(CellChaine));

        if (!chaine_i) {
            liberer_structure(chaines);
            return NULL;
        }

        chaine_i->numero = i;
        chaine_i->points = NULL;

        chaine_i->suiv = chaines->chaines;
        chaines->chaines = chaine_i;

        for (int j = 0; j < nb_points_chaines; j++) {
            CellPoint *point_ij = malloc(sizeof(CellPoint));

            if (!point_ij) {
                liberer_structure(chaines);
                return NULL;
            }

            point_ij->suiv = chaine_i->points;
            chaine_i->points = point_ij;

            point_ij->x = rand() % (xmax + 1);
            point_ij->y = rand() % (ymax + 1);
        }
    }

    return chaines;
}