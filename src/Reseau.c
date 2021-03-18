#include <stdio.h>
#include <stdlib.h>

#include "Reseau.h"
#include "commun.h"

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

void ecrireReseau(Reseau *R, FILE *file);
int nbLiaisons(Reseau *R);
int nbCommodites(Reseau *R);
void afficheReseauSVG(Reseau *R, char* nomInstance);

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