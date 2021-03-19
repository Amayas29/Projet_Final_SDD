#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Reseau.h"
#include "commun.h"
#include "SVGwriter.h"

Noeud *rechercheCreeNoeudListe(Reseau *R, double x, double y) {

    if(!R) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    CellNoeud *pred = NULL;

    for(CellNoeud *noeud = R->noeuds; noeud; noeud = noeud->suiv) {
        if (noeud->nd && noeud->nd->x == x && noeud->nd->y == y)
            return noeud->nd;

        pred = noeud;
    }

    CellNoeud *cell = malloc(sizeof(CellNoeud));
    if(!cell) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    Noeud *nd = malloc(sizeof(Noeud));

    if(!nd) {
        free(cell);
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    nd->num = ++R->nbNoeuds;
    nd->voisins = NULL;
    nd->x = x;
    nd->y = y;
    cell->nd = nd;
    cell->suiv = NULL;

    if(!pred) {
        R->noeuds = cell;
        return nd;
    }
    
    pred->suiv = cell;
    return nd;
}

Reseau *reconstitueReseauListe(Chaines *C) {

    if(!C) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    Reseau *reseau = malloc(sizeof(Reseau));

    if(!reseau) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    reseau->gamma = C->gamma;
    reseau->commodites = NULL;
    reseau->nbNoeuds = 0;
    reseau->noeuds = NULL;
    
    for(CellChaine *chaine = C->chaines; chaine; chaine = chaine->suiv) {

        Noeud *first = NULL, *last = NULL;

        for(CellPoint *point = chaine->points; point; point = point->suiv) {

            Noeud *noeud = rechercheCreeNoeudListe(reseau, point->x, point->y);
            
            if(!noeud) {
                liberer_reseau(reseau);
                return NULL;
            }

            if(!first)
                first = noeud;

            if(last) {

                int existe = 0;
                for(CellNoeud *voisins = noeud->voisins; voisins; voisins = voisins->suiv) {
                    if(voisins->nd->num == last->num) {
                        existe = 1;
                        break;
                    }
                }

                if(existe) {
                    last = noeud;
                    continue;
                }

                
                CellNoeud *vn = malloc(sizeof(CellNoeud));
                CellNoeud *vl = malloc(sizeof(CellNoeud));

                if(!vn || !vl) {
                    liberer_reseau(reseau);
                    print_probleme("Erreur d'allocation");
                    free(vn);
                    free(vl);
                    return NULL;
                }

                vn->nd = last;
                vl->nd = noeud;

                vn->suiv = noeud->voisins;
                noeud->voisins = vn;

                vl->suiv = last->voisins;
                last->voisins = vl;
            }

            last = noeud;
        }

        if (first) {

            CellCommodite *cmd = malloc(sizeof(CellCommodite));

            if(!cmd) {
                liberer_reseau(reseau);
                return NULL;
            }

            cmd->extrA = first;
            cmd->extrB = last;
            cmd->suiv = reseau->commodites;
            reseau->commodites = cmd;
        }
    }

    return reseau;
}

void ecrireReseau(Reseau *R, FILE *file) {
    if(!R) {
        print_probleme("Pointeur invalide");
        return;
    }

    if(!file) {
        print_probleme("Fichier introuvable");
        return;
    }

    int nb_liaisons = nbLiaisons(R);
    fprintf(file, "NbNoeuds: %d\n", R->nbNoeuds);
    fprintf(file, "NbLiaisons: %d\n", nb_liaisons);
    fprintf(file, "NbCommodites: %d\n", nbCommodites(R));
    fprintf(file, "Gamma: %d\n\n", R->gamma);

    for(CellNoeud *noeud = R->noeuds; noeud; noeud = noeud->suiv)
        fprintf(file, "v %d %.6f %.6f\n", noeud->nd->num, noeud->nd->x, noeud->nd->y);
    
    fprintf(file, "\n");

    char **cache = malloc(sizeof(char *) * nb_liaisons);

    if(!cache) {
        print_probleme("Erreur d'allocation");
        return;
    }

    for(int i = 0; i < nb_liaisons; i++)
        cache[i] = NULL;

    char s[BUFSIZ];
    int curr = 0;

    for(CellNoeud *noeud = R->noeuds; noeud; noeud = noeud->suiv) { 

        for(CellNoeud *voisin = noeud->nd->voisins; voisin; voisin = voisin->suiv) {
            int i = 0;
            sprintf(s, "%d %d", voisin->nd->num, noeud->nd->num);

            for(; i < nb_liaisons; i++) {

                if(cache[i] && strcmp(cache[i], s) == 0)
                    break;
            }

            if(i == nb_liaisons) {
                sprintf(s, "%d %d", noeud->nd->num, voisin->nd->num);
                cache[curr++] = strdup(s);
                fprintf(file, "l %s\n", s);
            }
        }
    }

    for(int i = 0; i < nb_liaisons; i++)
        free(cache[i]);
    free(cache);

    fprintf(file, "\n");

    for(CellCommodite *cmd = R->commodites; cmd; cmd = cmd->suiv)
        fprintf(file, "k %d %d\n", cmd->extrA->num, cmd->extrB->num);
}

static int number_voisins(CellNoeud *liste) {
    int number = 0;
    for(; liste; liste = liste->suiv, number++);
    return number;
}

int nbLiaisons(Reseau *R) {

    if (!R) return 0;

    int number = 0;
    for(CellNoeud *noeud = R->noeuds; noeud; number += number_voisins(noeud->nd->voisins), noeud = noeud->suiv);
    return number / 2;
}

int nbCommodites(Reseau *R) {
    if(!R) return 0;

    int number = 0;
    for(CellCommodite *cmd = R->commodites; cmd; cmd = cmd->suiv, number++);
    return number;
}

void afficheReseauSVG(Reseau *R, char* nomInstance) {
    CellNoeud *courN, *courv;
    SVGwriter svg;
    double maxx = 0, maxy = 0, minx = 1e6, miny = 1e6;

    courN = R->noeuds;
    while (courN != NULL) {
        if (maxx < courN->nd->x) maxx = courN->nd->x;
        if (maxy < courN->nd->y) maxy = courN->nd->y;
        if (minx > courN->nd->x) minx = courN->nd->x;
        if (miny > courN->nd->y) miny = courN->nd->y;
        courN = courN->suiv;
    }

    SVGinit(&svg, nomInstance, 500, 500);
    
    courN = R->noeuds;
    
    while (courN != NULL) {
        SVGlineRandColor(&svg);
        SVGpoint(&svg, 500 * (courN->nd->x - minx) / (maxx - minx), 500 * (courN->nd->y - miny) / (maxy-miny));
        courv = courN->nd->voisins;
        while (courv != NULL) {
            if (courv->nd->num < courN->nd->num)
                SVGline(&svg, 500 * (courv->nd->x - minx) / (maxx-minx), 500 * (courv->nd->y - miny) / (maxy-miny),
                    500 * (courN->nd->x - minx) / (maxx-minx), 500 * (courN->nd->y - miny) / (maxy - miny));
            courv = courv->suiv;
        }
        courN = courN->suiv;
    }
    SVGfinalize(&svg);
}

void liberer_cell_noeuds(CellNoeud *cells, int rm) {

    CellNoeud *tmp = NULL;
    while(cells) {

        if (rm)
            liberer_noeud(cells->nd);

        tmp = cells->suiv;
        free(cells);
        cells = tmp;
    }
}

void liberer_noeud(Noeud *noeud) {
    if(!noeud)
        return;
    
    liberer_cell_noeuds(noeud->voisins, 0);
    free(noeud);
}

void liberer_commodites(CellCommodite *commodites) {
    CellCommodite *tmp = NULL;
    while(commodites) {
        tmp = commodites->suiv;
        free(commodites);
        commodites = tmp;
    }
}

void liberer_reseau(Reseau *reseau) {
    if(!reseau)
        return;
    
    liberer_commodites(reseau->commodites);
    liberer_cell_noeuds(reseau->noeuds, 1);
    free(reseau);
}